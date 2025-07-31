<?php
session_start();
date_default_timezone_set("Africa/Kampala");

include "../classes/Db.php";
include "../classes/Field_calculate.inc";
include "../classes/BeforeAndAfter.inc";
include "../classes/AuditTrail.inc";
include "../classes/AccessRights.inc";
include "../classes/Efris.inc";
include ("../qrcode/qrlib.php");

$t = new BeforeAndAfter();

//error_reporting(null);
//var valuesToSend = "Col="+col+"&row="+row+"&Value="+toSave2+"&Tot="+totalSum;

$col = $_POST['col'];
$row = $_POST['row'];
$value = $_POST['value'];
$tot = $_POST['tot'];
$point = $_POST['metering_point'];
$customer_id = $_POST['customer_id'];
$year = $_POST['year'];
$month = $_POST['month'];
$expiry_date = strtotime($_POST['expiry_date']);

$peak = $_POST['peak'];
$off_peak = $_POST['off_peak'];
$shoulder = $_POST['shoulder'];


$db = new Db();
$select = $db->select("SELECT tb_id FROM time_band WHERE tb_customer_id = '$customer_id' AND tb_year = '$year' AND tb_month = '$month'");
if($db->num_rows()){
	extract($select[0][0]);
	$update = $db->update("time_band", [
		"tb_customer_id"=>$customer_id,
		"tb_peak"=>$peak,
		"tb_off_peak"=>$off_peak,
		"tb_shoulder"=>$shoulder,
		"tb_added_by"=>$user_id,
		"tb_date_added"=>time(),
		"tb_year"=>$year,
		"tb_month"=>$month,
		"tb_expiry_date"=>$expiry_date,
	],["tb_id"=>$tb_id]);
}else{
	$insert = $db->insert("time_band", [
		"tb_customer_id"=>$customer_id,
		"tb_peak"=>$peak,
		"tb_off_peak"=>$off_peak,
		"tb_shoulder"=>$shoulder,
		"tb_added_by"=>$user_id,
		"tb_date_added"=>time(),
		"tb_year"=>$year,
		"tb_month"=>$month,
		"tb_expiry_date"=>$expiry_date,
	]);
}

/*

for($i=1; $i<=3; $i++){
	${'firstname'.$i} = $_POST['firstname'.$i];
	${'othername'.$i} = $_POST['othername'.$i];
	${'date'.$i} = $_POST['date'.$i];
	${'time'.$i} = $_POST['time'.$i];
}
$db = new Db();
$select = $db->select("SELECT witness_id FROM witness WHERE witness_customer_id = '$customer_id' AND witness_year = '$year' AND witness_month = '$month'");
//if(0){
if($db->num_rows()){
	extract($select[0][0]);
	$update = $db->update("witness", [
		"witness_customer_id"=>$customer_id,

		"witness_firstname1"=>$firstname1,
		"witness_othername1"=>$othername1,
		"witness_date1"=>$date1,
		"witness_time1"=>$time1,

		"witness_firstname2"=>$firstname2,
		"witness_othername2"=>$othername2,
		"witness_date2"=>$date2,
		"witness_time2"=>$time2,

		"witness_firstname3"=>$firstname3,
		"witness_othername3"=>$othername3,
		"witness_date3"=>$date3,
		"witness_time3"=>$time3,

		"witness_added_by"=>$user_id,
		"witness_date_added"=>time(),
		"witness_year"=>$year,
		"witness_month"=>$month,
	],["witness_id"=>$witness_id]);
}else{
	$insert = $db->insert("witness", [
		
		"witness_month"=>$month,

		"witness_customer_id"=>$customer_id,

		"witness_firstname1"=>$firstname1,
		"witness_othername1"=>$othername1,
		"witness_date1"=>$date1,
		"witness_time1"=>$time1,

		"witness_firstname2"=>$firstname2,
		"witness_othername2"=>$othername2,
		"witness_date2"=>$date2,
		"witness_time2"=>$time2,

		"witness_firstname3"=>$firstname3,
		"witness_othername3"=>$othername3,
		"witness_date3"=>$date3,
		"witness_time3"=>$time3,

		"witness_added_by"=>$user_id,
		"witness_date_added"=>time(),
		"witness_year"=>$year,
		"witness_month"=>$month,
	]);

	//echo $db->error();
}*/

$time_readings = strtotime(date($year.'-'.$month.'-15'));
$db = new Db();
$select = $db->select("SELECT rea_id FROM r_reading WHERE rea_date = '$time_readings' AND rea_cus_id = '$customer_id'");
extract($select[0][0]);

$action_to_use = "";
if($rea_id){
	$action_to_use = "cha";
}else{
	$action_to_use = "add";
}

if($col>=1 && $col <=3){
	$variable = "rate_import_wh_".$col;
}elseif($col>=4 && $col <=6){	
	$variable = "rate_export_wh_".$col;
}elseif($col==7){	
	$variable = "rate_cum_imports";
}elseif($col==8){	
	$variable = "rate_cum_exports";
}elseif($col==9){	
	$variable = "rate_date_read";
}

$value = str_replace(",", "", $value);

if($col!=9){
	$value *= 1000;
}else{

}



if(strtolower($action_to_use)=="change" || strtolower($action_to_use) == "cha"){
	if($point != ""){
		$db = new Db();
		$select = $db->select("SELECT mp_id,mp_advance FROM metering_point WHERE mp_customer_id = '$customer_id' AND upper(mp_location) = upper('$point')");
		extract($select[0][0]);

		$swap = ($mp_advance == 2)?1:0;

		$select = $db->select("SELECT * FROM r_reading WHERE rea_date = '$time_readings' AND rea_cus_id = '$customer_id' AND rea_mp_id = '$mp_id'");

		echo $db->error();

		if($db->num_rows()){
			extract($select[0][0]);
			$rrr = $rea_id;

			//=====================================================================
			//checking which part to use for advance
			//=====================================================================
			$aaa = array();
			$aaa[$variable] = $value;			
			$aaa["rate_added_by"]=$user_id;
			$aaa["rate_date_added"]=time();
			$aaa["rate_swap"] = $swap;

			
			$tou = $t->tou($customer_id, $month, $year, 1);

			$peak = $tou[0];
			$shoulder = $tou[1];
			$off_peak = $tou[2];

			//imports ========================================================
			$rate_sfe = $t->rgf("r_rate", $rrr, "rate_reading_id", "rate_sfe");
			$v = $t->get_advance($customer_id, $mp_id, $month, $year, $rate_sfe);
			$reg = $t->rgf("metering_point", $mp_id, "mp_id", "mp_region_id");
			$reg = ($reg == 10 || $reg == 8) ? -1: 1;

			$g0 = ($v['AR1']);
			$g1 = ($v['AR2']);
			$g2 = ($v['AR3']);
			$g3 = ($v['ATOTAL']);

			$aaa["tisu"] = round((double)$g0*$reg,10);
			$aaa["tipu"] = round((double)$g1*$reg,10);
			$aaa["tiou"] = round((double)$g2*$reg,10);
			$aaa["tiu"] = round((double)$g3*$reg,10);

			$aaa["tisa"] = round((double)($g0*$shoulder*$reg),10);
			$aaa["tipa"] = round((double)($g1*$peak*$reg),10);
			$aaa["tioa"] = round((double)($g2*$off_peak*$reg),10);
			$aaa["tia"] = round((double)($g0*$shoulder*$reg),10) + round((double)($g1*$peak*$reg),10) + round((double)($g2*$off_peak *$reg),10);
			//export =====================================================			
			$rate_sfi = $t->rgf("r_rate", $rrr, "rate_reading_id", "rate_sfi");
			$ve = $t->get_advance($customer_id, $mp_id, $month, $year, $rate_sfi);

			$g0e = ($ve['AR1'])?($ve['AR1']):0;
			$g1e = ($ve['AR2'])?($ve['AR2']):0;
			$g2e = ($ve['AR3'])?($ve['AR3']):0;
			$g3e = ($ve['ATOTAL'])?($ve['ATOTAL']):0;

			$aaa["tesu"] = round((double)$g0e*$reg,10);
			$aaa["tepu"] = round((double)$g1e*$reg,10);
			$aaa["teou"] = round((double)$g2e*$reg,10);
			$aaa["teu"] = round((double)$g3e*$reg,10);

			$aaa["tesa"] = (double)($g0e*$shoulder*$reg);
			$aaa["tepa"] = (double)($g1e*$peak*$reg);
			$aaa["teoa"] = (double)($g2e*$off_peak*$reg);
			$aaa["tea"] = (double)($g0e*$shoulder*$reg) + (double)($g1e*$peak*$reg) + (double)($g2e*$off_peak*$reg) ;

			// //imports WHEELING CHARGE =====================================================
			$rate_wc_sfi = $t->rgf("r_rate", $rrr, "rate_reading_id", "rate_wc_sfi");
			$wc = $t->get_advanceWheeling($customer_id, $mp_id, $month, $year, $rate_wc_sfi,$customer_id);

			$aaa["wc_tisu"] = (double)$wc['AR1'];
			$aaa["wc_tipu"] = (double)$wc['AR2'];
			$aaa["wc_tiou"] = (double)$wc['AR3'];
			$aaa["wc_tiu"] = (double)$wc['ATOTAL'];

			$aaa["wc_tisa"] = (double)$wc['AMR1'];
			$aaa["wc_tipa"] = (double)$wc['AMR2'];
			$aaa["wc_tioa"] = (double)$wc['AMR3'];
			$aaa["wc_tia"] = (double)$wc['AMR1'] + (double)$wc['AMR2'] + (double)$wc['AMR3'];

			//===//==//print_r($aaa);

			$db = new Db();
			if(1){ //imports as advance
				$select = $db->update("r_rate", $aaa, ["rate_cus_id"=>$customer_id, "rate_reading_id"=>$rrr]);
				echo $db->error();
			}else{ //exports as advance
				// $select = $db->update("r_rate",[
				// 	//$variable=>$value,
				// 	"rate_added_by"=>$user_id,
				// 	"rate_date_added"=>time(),
				// ], ["rate_cus_id"=>$customer_id, "rate_reading_id"=>$rrr]);
				// echo $db->error();
			}
		}
	}
}


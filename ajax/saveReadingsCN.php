<?php
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
$user_id = $_POST['user_id'];

$peak = $_POST['peak'];
$off_peak = $_POST['off_peak'];
$shoulder = $_POST['shoulder'];
$narration = $_POST['narration'];
$expiry_date = strtotime($_POST['expiry_date']);

$db = new Db();
$select = $db->select("SELECT tb_id FROM time_band_cn WHERE tb_customer_id = '$customer_id' AND tb_year = '$year' AND tb_month = '$month'");
if($db->num_rows()){
	extract($select[0][0]);
	$update = $db->update("time_band_cn", [
		"tb_customer_id"=>$customer_id,
		"tb_peak"=>$peak,
		"tb_off_peak"=>$off_peak,
		"tb_shoulder"=>$shoulder,
		"tb_added_by"=>$user_id,
		"tb_date_added"=>time(),
		"tb_year"=>$year,
		"tb_month"=>$month,
		"tb_expiry_date"=>$expiry_date,
		"tb_narration"=>$narration,
	],["tb_id"=>$tb_id]);
}else{
	$insert = $db->insert("time_band_cn", [
		"tb_customer_id"=>$customer_id,
		"tb_peak"=>$peak,
		"tb_off_peak"=>$off_peak,
		"tb_shoulder"=>$shoulder,
		"tb_added_by"=>$user_id,
		"tb_date_added"=>time(),
		"tb_year"=>$year,
		"tb_month"=>$month,
		"tb_expiry_date"=>$expiry_date,
		"tb_narration"=>$narration,
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
$sql = "SELECT TOP 1 n_rea_id FROM r_reading_cn WHERE n_rea_date = '$time_readings' AND n_rea_cus_id = '$customer_id'";
$select = $db->select($sql);
extract($select[0][0]);

$action_to_use = "";
if($n_rea_id){
	$action_to_use = "cha";
}else{
	$action_to_use = "add";
}

if($col>=1 && $col <=3){
	$variable = "n_rate_import_wh_".$col;
}elseif($col>=4 && $col <=6){	
	$variable = "n_rate_export_wh_".$col;
}elseif($col==7){	
	$variable = "n_rate_cum_imports";
}elseif($col==8){	
	$variable = "n_rate_cum_exports";
}elseif($col==9){	
	$variable = "n_rate_date_read";
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

		$sql = "SELECT * FROM r_reading_cn WHERE n_rea_date = '$time_readings' AND n_rea_cus_id = '$customer_id' AND n_rea_mp_id = '$mp_id'";
		$select = $db->select($sql);

		echo $db->error();

		if($db->num_rows()){
			extract($select[0][0]);
			$rrr = $n_rea_id;

			//=====================================================================
			//checking which part to use for advance
			//=====================================================================
			$aaa = array();
			$aaa[$variable] = $value;			
			$aaa["n_rate_added_by"]=$user_id;
			$aaa["n_rate_date_added"]=time();
			$aaa["n_rate_swap"] = $swap;

			
			$tou = $t->tou($customer_id, $month, $year, 1);

			$peak = $tou[0];
			$shoulder = $tou[1];
			$off_peak = $tou[2];

			//imports ========================================================
			$rate_sfe = $t->rgf("r_rate_cn", $rrr, "n_rate_reading_id", "n_rate_sfe");
			$v = $t->get_advance($customer_id, $mp_id, $month, $year, $rate_sfe,1);
			$reg = $t->rgf("metering_point", $mp_id, "mp_id", "mp_region_id");
			$reg = ($reg == 10 || $reg == 8) ? -1: 1;

			$g0 = ($v['AR1']);
			$g1 = ($v['AR2']);
			$g2 = ($v['AR3']);
			$g3 = ($v['ATOTAL']);

			$aaa["n_tisu"] = round((double)$g0*$reg,10);
			$aaa["n_tipu"] = round((double)$g1*$reg,10);
			$aaa["n_tiou"] = round((double)$g2*$reg,10);
			$aaa["n_tiu"] = round((double)$g3*$reg,10);

			$aaa["n_tisa"] = round((double)($g0*$shoulder*$reg),10);
			$aaa["n_tipa"] = round((double)($g1*$peak*$reg),10);
			$aaa["n_tioa"] = round((double)($g2*$off_peak*$reg),10);
			$aaa["n_tia"] = round((double)($g0*$shoulder*$reg),10) + round((double)($g1*$peak*$reg),10) + round((double)($g2*$off_peak *$reg),10);
			//export =====================================================			
			$rate_sfi = $t->rgf("r_rate_cn", $rrr, "n_rate_reading_id", "n_rate_sfi");
			$ve = $t->get_advance($customer_id, $mp_id, $month, $year, $rate_sfi,1);

			$g0e = ($ve['AR1'])?($ve['AR1']):0;
			$g1e = ($ve['AR2'])?($ve['AR2']):0;
			$g2e = ($ve['AR3'])?($ve['AR3']):0;
			$g3e = ($ve['ATOTAL'])?($ve['ATOTAL']):0;

			$aaa["n_tesu"] = round((double)$g0e*$reg,10);
			$aaa["n_tepu"] = round((double)$g1e*$reg,10);
			$aaa["n_teou"] = round((double)$g2e*$reg,10);
			$aaa["n_teu"] = round((double)$g3e*$reg,10);

			$aaa["n_tesa"] = (double)($g0e*$shoulder*$reg);
			$aaa["n_tepa"] = (double)($g1e*$peak*$reg);
			$aaa["n_teoa"] = (double)($g2e*$off_peak*$reg);
			$aaa["n_tea"] = (double)($g0e*$shoulder*$reg) + (double)($g1e*$peak*$reg) + (double)($g2e*$off_peak*$reg) ;

// //imports WHEELING CHARGE =====================================================
			$rate_wc_sfi = $t->rgf("r_rate_cn", $rrr, "n_rate_reading_id", "n_rate_wc_sfi");
			$wc = $t->get_advanceWheeling($customer_id, $mp_id, $month, $year, $rate_wc_sfi,$customer_id);


			$n_rate_payable_imports = $t->rgf("r_rate_cn", $rrr, "n_rate_reading_id", "n_rate_payable_imports");
			$t->generateWheelingCharge($n_rate_payable_imports, $year, $month);
			// //imports WHEELING CHARGE =====================================================
			$rate_wc_sfi = $t->rgf("r_rate_cn", $rrr, "n_rate_reading_id", "n_rate_wc_sfi");
			$wc = $t->get_advanceWheeling($customer_id, $mp_id, $month, $year, $rate_wc_sfi,$customer_id);

			$aaa["n_wc_tisu"] = (double)$wc['AR1'];
			$aaa["n_wc_tipu"] = (double)$wc['AR2'];
			$aaa["n_wc_tiou"] = (double)$wc['AR3'];
			$aaa["n_wc_tiu"] = (double)$wc['ATOTAL'];

			$aaa["n_wc_tisa"] = (double)$wc['AMR1'];
			$aaa["n_wc_tipa"] = (double)$wc['AMR2'];
			$aaa["n_wc_tioa"] = (double)$wc['AMR3'];
			$aaa["n_wc_tia"] = (double)$wc['AMR1'] + (double)$wc['AMR2'] + (double)$wc['AMR3'];

			$db = new Db();
			if(1){ //imports as advance
				echo 'sdf: '.$rrr;
				$select = $db->update("r_rate_cn", $aaa, ["n_rate_cus_id"=>$customer_id, "n_rate_reading_id"=>$rrr]);
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


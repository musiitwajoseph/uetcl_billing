<?php
include "../classes/Db.php";

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
$note_details = $_POST['note_details'];

$db = new Db();
// $select = $db->select("SELECT tb_id FROM time_band_cn WHERE tb_customer_id = '$customer_id' AND tb_year = '$year' AND tb_month = '$month'");
// if($db->num_rows()){
// 	extract($select[0][0]);
// 	$update = $db->update("time_band_cn", [
// 		"tb_customer_id"=>$customer_id,
// 		"tb_peak"=>$peak,
// 		"tb_off_peak"=>$off_peak,
// 		"tb_shoulder"=>$shoulder,
// 		"tb_added_by"=>$user_id,
// 		"tb_date_added"=>time(),
// 		"tb_year"=>$year,
// 		"tb_month"=>$month,
// 	],["tb_id"=>$tb_id]);
// }else{
// 	$insert = $db->insert("time_band_cn", [
// 		"tb_customer_id"=>$customer_id,
// 		"tb_peak"=>$peak,
// 		"tb_off_peak"=>$off_peak,
// 		"tb_shoulder"=>$shoulder,
// 		"tb_added_by"=>$user_id,
// 		"tb_date_added"=>time(),
// 		"tb_year"=>$year,
// 		"tb_month"=>$month,
// 	]);
// }

$time_readings = strtotime(date($year.'-'.$month.'-15'));
$db = new Db();
$select = $db->select("SELECT n_rea_id FROM r_reading_cn_previous WHERE n_rea_date = '$time_readings' AND n_rea_cus_id = '$customer_id'");
extract($select[0][0]);

$action_to_use = "";
if($n_rea_id){
	$action_to_use = "cha";
}else{
	$action_to_use = "add";
}

if($col>=1 && $col <=3){
	$variable = "n_rate_import_wh_".$col;
}else{	
	$variable = "n_rate_export_wh_".$col;
}

$value = str_replace(",", "", $value);
$value *= 1000;




if(strtolower($action_to_use)=="change" || strtolower($action_to_use) == "cha"){
	if($point != ""){
		$db = new Db();
		$select = $db->select("SELECT mp_id,mp_advance FROM metering_point WHERE mp_customer_id = '$customer_id' AND mp_location = '$point'");
		extract($select[0][0]);

		$select = $db->select("SELECT * FROM r_reading_cn_previous WHERE n_rea_date = '$time_readings' AND n_rea_cus_id = '$customer_id' AND n_rea_mp_id = '$mp_id'");

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

			$db = new Db();
			if($mp_advance!=2){ //imports as advance
				$select = $db->update("r_rate_cn_previous", $aaa, ["n_rate_cus_id"=>$customer_id, "n_rate_reading_id"=>$rrr]);
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


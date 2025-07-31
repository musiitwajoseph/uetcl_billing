<?php
error_reporting(null);
include "Db.php";
include "init.inc";
include "statics.inc";
//ini_set('display_errors', 1);
	$year = trim($_POST['year']);
	$month = trim($_POST['month']);
	$fixed_component = trim($_POST['fixed_component']);
	$escalatable_component = trim($_POST['escalatable_component']);
	$cpim_3 = trim($_POST['cpim_3']);
	$cpib = trim($_POST['cpib']);
	$time = time();
	$user = user_id();
	$errors = array();
	echo ">>>>>>>>>>>>>>>>".$year;
	$xx = array();
	if(empty($errors)){
		$db = new Db();
		$db->insert("kplc_tariff_schedule",["ts_date_added"=>$time, "ts_year"=>$year, "ts_month"=>$month,"ts_fixed_component"=>$fixed_component,"ts_added_by"=>$user,"ts_escalatable_component"=>$escalatable_component,"ts_cpim_3"=>$cpim_3,"ts_cpib"=>$ts_cpib]);
		 $xx['message'] = 'Successfully Saved';
}

else{
	$xx['message']='Error';

}
          
echo json_encode($xx);


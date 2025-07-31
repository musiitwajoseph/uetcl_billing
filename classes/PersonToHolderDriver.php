<?php
include "Db.php";
include "init.inc";
include "statics.inc";

$driver_id = $_POST['driver_id'];

$db = new Db();
$select = $db->select("SELECT user_designation FROM sysuser where user_id = '$driver_id'");
extract($select[0][0]);
if($user_designation == static_person_to_holder()){
	echo '<div class="col-lg-12">';
	echo 'Select Vehicle:<span class="must"></span>';

	echo '<select name="vehicle" class="form-control">';
	$d = new Db();

	$all_holder_drivers = array();
	$ss = $d->select("SELECT pvtd_vehicle_id FROM person_vehicle_to_driver");
	foreach($ss[0] as $k){
		extract($k);
		$all_holder_drivers[] = $pvtd_vehicle_id;
	}
	echo '<option value=""> Select </option>';
	$vehicle_typeid = static_vehicle_type();
	$s = $d->select("SELECT * FROM vehicle WHERE vehicle_type = $vehicle_typeid ORDER BY vehicle_reg_no ASC");
	foreach($s[0] as $r){
		extract($r);

		if(!in_array($vehicle_id, $all_holder_drivers)){
			echo '<option value="'.$vehicle_id.'">'.($vehicle_reg_no).'</option>';
		}

	}

	echo '</select>';
	echo '</div>';
}else{
	
}



?>
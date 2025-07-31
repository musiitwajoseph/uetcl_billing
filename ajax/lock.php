<?php
include "../classes/Db.php";

error_reporting(null);

$customer_id = $_POST['customer_id'];
$year = $_POST['year'];
$month = $_POST['month'];

$db = new Db();

$select = $db->select("SELECT lock_id, lock_status FROM lock WHERE lock_customer_id = '$customer_id' AND lock_year = '$year' AND lock_month='$month' AND lock_status IS NOT NULL");

if($db->num_rows()){
	extract($select[0][0]);
	$lock_status = ($lock_status)?1:0;
	$select = $db->update("lock", [
		"lock_status"=>$peak,	
	], ["lock_id"=>$lock_id]);

	if($lock_status){
		echo '<i style="color:red;" class="fa fa-2x fa-unlock"></i>';
	}else{
		echo '<i style="color:black;" class="fa fa-2x fa-lock"></i>';
	}
}else{
	$insert = $db->insert("lock", [
		"lock_status"=>1,	
		"lock_year"=>$year,	
		"lock_month"=>$month,	
		"lock_customer_id"=>$customer_id,	
	]);

	echo '<i style="color:black;" class="fa fa-2x fa-lock"></i>';
}




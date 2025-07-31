<?php
include "../classes/Db.php";

error_reporting(null);

$category = $_POST['category'];

$db = new Db();

if($category==3){
	$select = $db->select("SELECT * FROM customer ORDER BY customer_full_name ASC");
	//echo $db->num_rows();
	foreach($select[0] as $row){
		extract($row);
		echo '<option value="'.$customer_id.'">'.$customer_full_name.'</option>';
	}
}elseif($category == 4){
	$select = $db->select("SELECT * FROM of_customer ORDER BY ofc_customer_name ASC");
	//echo $db->num_rows();
	foreach($select[0] as $row){
		extract($row);
		echo '<option value="'.$ofc_id.'">'.$ofc_customer_name.'</option>';
	}
}else{
	$sql = "SELECT * FROM receipt_category_item WHERE rci_category = '$category'";
	$select = $db->select($sql);
	$no = 1;
	if($db->num_rows()){
	foreach($select[0] as $row){
		extract($row);
		echo '<option value="'.$rci_id.'">'.$rci_name.'</option>';
	}
	}else{	
		echo '<option value="">No Customers</option>';
	}
}
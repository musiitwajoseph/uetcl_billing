<?php 
include "../classes/init.inc";


if(isset($_POST['category'])){

	//echo '<br/>not: '.
	$category = $_POST['category'];

	$db = new Db();
	$select = $db->select("SELECT * FROM customers where cust_category = '$category' ORDER BY cust_id ASC");
	foreach($select[0] as $row){
		extract($row);
		echo '<option value="'.$cust_id.'">'.$cust_name.' - '.$cust_type.'</option>';
	}
	
}
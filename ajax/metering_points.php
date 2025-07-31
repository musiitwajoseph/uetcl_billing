<?php
include "../classes/Db.php";

error_reporting(null);

$customer_id = $_POST['customer_id'];

$db = new Db();
$select = $db->select("SELECT mp_id, mp_customer_id, mp_location FROM metering_point WHERE mp_customer_id = '$customer_id';");
$a = "";

foreach($select[0] as $row){
	extract($row);
	$a .= '<option value="'.$mp_id.'">'.strtoupper($mp_location).'</option>';
}

if($db->num_rows()==0) $a .= '<option>No metering points</option>';

$b = $db->num_rows();

echo $b.'===='.$a;


<?php
include "../classes/init.inc";

$t = new BeforeAndAfter();

$invoice = $_POST['invoice'];
$kg = ($_POST['kg']);
$bag = ($_POST['bag']);
$received_on = strtotime($_POST['received_on']);
$received_at = ($_POST['received_at']);
echo $status = ($_POST['status']);

if(empty($invoice)) exit();

$db = new Db();

$update =  $db->update("invoice", [
	"inv_received"=>time(), 
	"inv_received_on"=>$received_on,
	"inv_received_at"=>$received_at,
	"inv_received_kg"=>$kg,
	"inv_received_bag"=>$bag,
	"inv_received_status"=>$status
], ["inv_id"=>$invoice]);

$consignment = $t->rgf('invoice', $invoice, "inv_id", "inv_consignment_number");

$sql = "SELECT * FROM invoice WHERE inv_consignment_number = '$consignment'";
$db->select($sql);
echo '>>'.$total = $db->num_rows();

$sql = "SELECT * FROM invoice WHERE inv_consignment_number = '$consignment' AND inv_received_status = 'Point of Sale'";
$db->select($sql);
echo '>>'.$p = $db->num_rows();

if($total==$p){
	$db->update("consignment",["con_completed"=>time()], ["con_reference"=>$consignment]);
}


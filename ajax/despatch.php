<?php
include "../classes/init.inc";

$invoice = $_POST['invoice'];
$date = strtotime($_POST['date']);
$arrival = strtotime($_POST['arrival']);
$notoNumber = ($_POST['notoNumber']);

if(empty($invoice)) exit();

$db = new Db();

$update =  $db->update("invoice", [
	"inv_despatch"=>time(), 
	"inv_despatch_number"=>$notoNumber,
	"inv_despatch_date"=>$date,
	"inv_despatch_arrival"=>$arrival,
], ["inv_id"=>$invoice]);

echo $db->error();

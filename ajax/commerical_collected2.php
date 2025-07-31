<?php 
include "../classes/init.inc";


if(isset($_POST['commericalInvoiceNumber'])){

	//echo '<br/>not: '.
	$commericalInvoiceNumber = $_POST['commericalInvoiceNumber'];
	$vehicleNumber = ($_POST['vehicleNumber']);
	$despatchNote = ($_POST['despatchNote']);
	$collectedOn = strtotime($_POST['collectedOn']);
	$collectedBy = ($_POST['collectedBy']);
		
	$db = new Db();
	$insert = $db->update("commerical_invoice",[
		"com_collected_by_by"=>user_id(),
		"com_collected_by"=>$collectedBy,
		"com_collected_on"=>$collectedOn,
		"com_despatch_note"=>$despatchNote,
		"com_vehicle_number"=>$vehicleNumber,
	],["com_id"=>$commericalInvoiceNumber]);


	if(!$db->error()){
		echo Feedback::success();
	}else{
		echo FeedBack::error($db->error());
	}
	
}
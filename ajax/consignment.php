<?php 
include "../classes/init.inc";


if(isset($_POST['tot'])){
	$tot = $_POST['tot'];

	//echo '<br/>not: '.
	echo $consignee = $_POST['consignee'];
	//echo '<br/>not: '.
	$notes = $_POST['notes'];
	//echo '<br/>shi: '.
	$shipper = $_POST['shipper'];
	//echo '<br/>shi: '.
	$shipmentMethod = $_POST['shipmentMethod'];
	//echo '<br/>del: '.
	$deliverTo = $_POST['deliverTo'];
	//echo '<br/>con: '.
	$consignmentNotes = $_POST['consignmentNotes'];
	//echo '<br/>rev: '.
	$revenueNotes = $_POST['revenueNotes'];
	//echo '<br/>by : '.
	$by = $_POST['by'];
	//echo '<br/>veh: '.
	$vehicleNumbers = $_POST['vehicleNumbers'];
	//echo '<br/>veh: '.
	$factory = $_POST['factory'];

	$from = $_POST['from'];

	$con_ref = time().user_id();
	$db = new Db();
	$insert = $db->insert("consignment",[
		"con_notes"=>$notes,
		"con_shipper"=>$shipper,
		"con_shipment_method"=>$shipmentMethod,
		"con_consignment_notes"=>$consignmentNotes,
		"con_revenue_notes"=>$revenueNotes,
		"con_by"=>$by,
		"con_deliver_to"=>$deliverTo,
		"con_vehicle_numbers"=>$vehicleNumbers,
		"con_factory"=>$factory,
		"con_date_added"=>time(),
		"con_added_by"=>user_id(),
		"con_reference"=>$con_ref,
		"con_consignee"=>$consignee,
	]);

	if(!$db->error()){
		for($i=1; $i<=$tot; $i++){
			$inv = $_POST['inv'.$i];
			$db->update("invoice", ["inv_consignment_number"=>$con_ref], ["inv_id"=>$inv]);
		}
	}



	if(!$db->error()){
		echo Feedback::success();
	}else{
		echo FeedBack::error($db->error());
	}
	
}
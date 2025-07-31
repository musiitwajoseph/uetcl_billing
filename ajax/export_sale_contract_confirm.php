<?php 
include "../classes/init.inc";


if(isset($_POST['buyer'])){
	$buyer = $_POST['buyer'];

	//echo '<br/>not: '.
	$endBuyer = $_POST['endBuyer'];
	$destinationPort = $_POST['destinationPort'];
	$finalDestination = $_POST['finalDestination'];
	$gross = str_replace(',', '', $_POST['gross']);
	$usPerUsh = str_replace(',', '', $_POST['usPerUsh']);
	$contractNumber = $_POST['contractNumber'];
	$contractDate = $_POST['contractDate'];
	$contractSaleDate = $_POST['contractSaleDate'];
	$invoiceID = $_POST['invoiceID'];
	$notes = $_POST['notes'];
	$paymentDate = $_POST['paymentDate'];
		
	$con_ref = time().user_id();
	$db = new Db();
	$insert = $db->insert("export_contract",[
		"contract_number"=>$contractNumber,
		"contract_date"=>strtotime($contractDate),
		"contract_sale_date"=>strtotime($contractSaleDate),
		"us_per_ush"=>$usPerUsh,
		"gross"=>$gross,
		"buyer"=>$buyer,
		"end_buyer"=>$endBuyer,
		"final_destination"=>$finalDestination,
		"destination_port"=>$destinationPort,
		"contract_ref"=>$con_ref,
		"contract_date_added"=>time(),
		"contract_added_by"=>user_id(),
		"payment_date"=>($paymentDate),
		"contract_invoice_id"=>($invoiceID),
		"notes"=>$notes,
	]);

	if(!$db->error()){
		$db->update("invoice", ["inv_ref"=>$con_ref, "inv_type"=>3], ["inv_id"=>$invoiceID]);		
	}

	if(!$db->error()){
		echo Feedback::success();
	}else{
		echo FeedBack::error($db->error());
	}
	
}else{

}

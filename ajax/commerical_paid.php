<?php 
include "../classes/init.inc";


if(isset($_POST['commericalInvoiceNumber'])){

	//echo '<br/>not: '.
	$commericalInvoiceNumber = $_POST['commericalInvoiceNumber'];
	$saleDate = strtotime($_POST['saleDate']);
	$payDate = strtotime($_POST['payDate']);
		
	$db = new Db();
	$insert = $db->update("commerical_invoice",[
		"com_paid"=>$payDate,
		"com_paid_added_by"=>user_id(),
	],["com_number"=>$commericalInvoiceNumber]);


	if(!$db->error()){
		echo Feedback::success();
	}else{
		echo FeedBack::error($db->error());
	}
	
}
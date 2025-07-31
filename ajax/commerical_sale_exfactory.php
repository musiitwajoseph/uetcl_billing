<?php 
include "../classes/init.inc";


if(isset($_POST['tot'])){
	$tot = $_POST['tot'];

	//echo '<br/>not: '.
	$commericalInvoiceNumber = $_POST['commericalInvoiceNumber'];
	//echo '<br/>not: '.
	$notes = $_POST['notes'];
	//echo '<br/>shi: '.
	$localbuyer = $_POST['localbuyer'];
	//echo '<br/>shi: '.
	$saleType = $_POST['saleType'];
	//echo '<br/>del: '.
	$saleDate = strtotime($_POST['saleDate']);
	//echo '<br/>con: '.
	$tax = $_POST['tax'];
	//echo '<br/>rev: '.
	
	$con_ref = time().user_id();
	$db = new Db();
	$insert = $db->insert("commerical_invoice",[
		"com_notes"=>$notes,
		"com_buyer"=>$localbuyer,
		"com_number"=>$commericalInvoiceNumber,
		"com_sale_type"=>$saleType,
		"com_sale_date"=>$saleDate,
		"com_tax"=>$tax,
		"com_added_by"=>user_id(),
		"com_date_added"=>time(),
		"com_ref"=>$con_ref,
		"com_type"=>4,
	]);

	if(!$db->error()){
		for($i=1; $i<=$tot; $i++){
			$inv = $_POST['inv'.$i];
			$price = $_POST['inv_price'.$i];
			$db->update("invoice", ["inv_ref"=>$con_ref, "inv_type"=>4, "inv_price"=>$price], ["inv_id"=>$inv]);
		}
	}



	if(!$db->error()){
		echo Feedback::success();
	}else{
		echo FeedBack::error($db->error());
	}
	
}
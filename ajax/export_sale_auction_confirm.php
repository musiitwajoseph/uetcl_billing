<?php 
include "../classes/init.inc";


if(isset($_POST['lotNo'])){
	$lotNo = $_POST['lotNo'];

	//echo '<br/>not: '.
	$auctionNumber = $_POST['auctionNumber'];
	$auctionSaleDate = $_POST['auctionSaleDate'];
	$auctionDate = $_POST['auctionDate'];
	$usPerUsh = str_replace(',', '', $_POST['usPerUsh']);
	$lotNo = $_POST['lotNo'];
	$buyer = $_POST['buyer'];
	$broker = $_POST['broker'];
	$gross = $_POST['gross'];
	$invoiceID = $_POST['invoiceID'];
	$notes = $_POST['notes'];
	
	$reason = $_POST['reason'];
	$reduceKgs = (int)$_POST['reduceKgs'];
	$unsold = (int)$_POST['unsold'];
	
	
	$db = new Db();
	$insert = $db->insert("unsold",[
		"u_invoice_id"=>$invoiceID,
		"u_auction_number"=>$auctionNumber,
		"u_reduce_kgs"=>$reduceKgs,
		"u_reason"=>$reason,
		"u_auction_sale_date"=>strtotime($auctionSaleDate),
		"u_date_added"=>time(),
		"u_added_by"=>user_id(),
		"u_lot_no"=>$lotNo,
		"u_buyer"=>$buyer,
		"u_broker"=>$broker,
		"u_status"=>$unsold,
	]);
		
	if(!$unsold){
		$con_ref = time().user_id();
		$db = new Db();
		$insert = $db->insert("export_auction",[
			"auction_number"=>$auctionNumber,
			"auction_date"=>strtotime($auctionDate),
			"auction_sale_date"=>strtotime($auctionSaleDate),
			"us_per_ush"=>$usPerUsh,
			"lot_no"=>$lotNo,
			"broker"=>$broker,
			"buyer"=>$buyer,
			"gross"=>$gross,
			"notes"=>$notes,
			"auction_ref"=>$con_ref,
			"auction_date_added"=>time(),
			"auction_added_by"=>user_id(),
		]);

		if(!$db->error()){
			$db->update("invoice", ["inv_ref"=>$con_ref, "inv_type"=>2], ["inv_id"=>$invoiceID]);		
		}
	}

	if(!$db->error()){
		echo Feedback::success();
	}else{
		echo FeedBack::error($db->error());
	}
	
}else{

}

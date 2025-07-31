<?php
include "../classes/init.inc";

$t = new BeforeAndAfter();

$invoice_date = $_POST['invoice_date'];
$grade = $_POST['grade'];
$from = $_POST['from'];
$Original_kg = $_POST['Original_kg'];
$bag_type = $_POST['bag_type'];
$bag_number = $_POST['bag_number'];
$invoice_note = $_POST['invoice_note'];
$id = $_POST['id'];

$comm_number = $_POST['comm_number'];
$sale_type = $_POST['sale_type'];
$buyer = $_POST['buyer'];
$sale_date = $_POST['sale_date'];
$comm_taxt = str_replace(',','',$_POST['comm_taxt']);
$comm_notes = $_POST['comm_notes'];
$comm_id = $_POST['comm_id'];

$ugx = str_replace(',','',$_POST['ugx']);
$date = $_POST['date'];
$despatch_note = $_POST['despatch_note'];
$vehicle_number = $_POST['vehicle_number'];
$collected_by = $_POST['collected_by'];
$invoice_type = $_POST['invoice_type'];

$consignee = $_POST['consignee'];
$con_id = $_POST['con_id'];
$transporter = $_POST['transporter'];
$vehicle_no = $_POST['vehicle_no'];
$notes = $_POST['notes'];
$by = $_POST['by'];
$ship_method = $_POST['ship_method'];
$consignt_note = $_POST['consignt_note'];

$broker = $_POST['broker'];
$auction_number = $_POST['auction_number'];
$auction_date = $_POST['auction_date'];
$shilling = str_replace(',','', $_POST['shilling']);
$gross = str_replace(',','',$_POST['gross']);
$auction_note = $_POST['auction_note'];
$export_id = $_POST['export_id'];
$lot_number = $_POST['lot_number'];
$sale_date = $_POST['sale_date'];

$buyer2 = $_POST['buyer2'];
$end_buyer = $_POST['end_buyer'];
$destination_port = $_POST['destination_port'];
$final_destination = $_POST['final_destination'];
$contract_number = $_POST['contract_number'];
$contract_date = $_POST['contract_date'];
$contract_sale_date = $_POST['contract_sale_date'];
$us_per_ush = str_replace(',','', $_POST['us_per_ush']);
$export_contract_id = $_POST['export_contract_id'];
$collection_detail = $_POST['collection_detail'];
$payment_detail = $_POST['payment_detail'];
$deliver_to = $_POST['deliver_to'];

//echo "consignee>>>>>>>>>".$consignee;
//echo "deliver_to>>>>>>>>>".$deliver_to;
$errors = array();
$db = new Db();
if(($invoice_type == '1' || $invoice_type == '4')|| ($payment_detail =='undefined')){
	$update =  $db->update("invoice", [
		"inv_date_added"=>strtotime($invoice_date), 
		"inv_from"=>$from, 
		"inv_grade"=>$grade, 
		"inv_kgs"=>$Original_kg, 
		"inv_bag_number"=>$bag_type, 
		"inv_bag_type"=>$bag_type,
		"inv_bag_number"=>$bag_number,
		"inv_notes"=>$invoice_note, 
	],["inv_id"=>$id]);

// $xx = $db->update("commerical_invoice", [
// 		"com_number"=>$comm_number, 
// 		 "com_sale_type"=>$sale_type, 
// 		 "com_buyer"=>$buyer, 
// 		 "com_sale_date"=>strtotime($sale_date), 
// 		 "com_tax"=>$comm_taxt, 
// 		"com_notes"=>$comm_notes,
// 	],["com_id"=>$comm_id]);
// if(!$db->error()){
// echo "saved";
// }else{
// 	echo "not saved".$db->error();
// }
}
if(($collection_detail=='undefined') || ($invoice_type == '1' || $invoice_type == '4')){
	$update =  $db->update("invoice", [
		"inv_date_added"=>strtotime($invoice_date), 
		"inv_from"=>$from, 
		"inv_grade"=>$grade, 
		"inv_kgs"=>$Original_kg, 
		"inv_bag_number"=>$bag_type, 
		"inv_bag_type"=>$bag_type,
		"inv_bag_number"=>$bag_number,
		"inv_notes"=>$invoice_note, 
		"inv_price"=>$ugx, 
	],["inv_id"=>$id]);


$xx = $db->update("commerical_invoice", [
		"com_number"=>$comm_number, 
		 "com_sale_type"=>$sale_type, 
		 "com_buyer"=>$buyer, 
		 "com_sale_date"=>strtotime($sale_date), 
		 "com_tax"=>$comm_taxt, 
		"com_notes"=>$comm_notes,
	],["com_id"=>$comm_id]);
if(!$db->error()){
echo "saved";
}else{
	echo "not saved".$db->error();
}
}
if($invoice_type == '1' || $invoice_type == '4'){
$update =  $db->update("invoice", [
		"inv_date_added"=>strtotime($invoice_date), 
		"inv_from"=>$from, 
		"inv_grade"=>$grade, 
		"inv_kgs"=>$Original_kg, 
		"inv_bag_number"=>$bag_type, 
		"inv_bag_type"=>$bag_type,
		"inv_bag_number"=>$bag_number,
		"inv_notes"=>$invoice_note, 
		"inv_price"=>$ugx, 
	],["inv_id"=>$id]);


$xx = $db->update("commerical_invoice", [
		"com_number"=>$comm_number, 
		"com_sale_type"=>$sale_type, 
		"com_buyer"=>$buyer, 
		"com_sale_date"=>strtotime($sale_date), 
		"com_tax"=>$comm_taxt, 
		"com_collected_by"=>$collected_by,
		"com_collected_on"=>strtotime($date),
		"com_despatch_note"=>$despatch_note,
		"com_notes"=>$comm_notes,
		"com_vehicle_number"=>$vehicle_number,
	],["com_id"=>$comm_id]);
if(!$db->error()){
echo "saved";
}else{
	echo "not saved".$db->error();
}

}
elseif($invoice_type == '2'){
if(empty($errors)){
$update =  $db->update("invoice", [
		"inv_date_added"=>strtotime($invoice_date), 
		"inv_from"=>$from, 
		"inv_grade"=>$grade, 
		"inv_kgs"=>$Original_kg, 
		"inv_bag_number"=>$bag_type, 
		"inv_bag_type"=>$bag_type,
		"inv_bag_number"=>$bag_number,
		"inv_notes"=>$invoice_note, 
	],["inv_id"=>$id]);

if(empty($errors)){
$xx = $db->update("consignment", [
		"con_shipper"=>$transporter, 
		"con_consignee"=>$consignee,
		"con_deliver_to"=>$deliver_to, 
		"con_vehicle_numbers"=>$vehicle_no, 
		"con_consignment_notes"=>$notes, 
		//"con_shipment_method"=>$ship_method, 
		"con_by"=>$by,
		"con_notes"=>$consignt_note,
	],["con_id"=>$con_id]);
if(!$db->error()){
echo "saved";
}else{
	echo "not saved".$db->error();
}
$xx = $db->update("export_auction", [
		"lot_no"=>$lot_number, 
		"broker"=>$broker, 
		"buyer"=>$buyer, 
		"auction_number"=>$auction_number, 
		"auction_date"=>strtotime($auction_date), 
		"auction_sale_date"=>strtotime($sale_date),
		"us_per_ush"=>$shilling,
		"gross"=>$gross,
		"notes"=>$auction_note,
	],["export_id"=>$export_id]);
if(!$db->error()){
echo "saved";
}else{
	echo "not saved".$db->error();
}
}
}
}elseif($invoice_type=='3'){
	if(empty($errors)){
$update =  $db->update("invoice", [
		"inv_date_added"=>strtotime($invoice_date), 
		"inv_from"=>$from, 
		"inv_grade"=>$grade, 
		"inv_kgs"=>$Original_kg, 
		"inv_bag_number"=>$bag_type, 
		"inv_bag_type"=>$bag_type,
		"inv_bag_number"=>$bag_number,
		"inv_notes"=>$invoice_note, 
	],["inv_id"=>$id]);

if(empty($errors)){
$xx = $db->update("consignment", [
		"con_shipper"=>$transporter, 
		"con_deliver_to"=>$deliver_to,
		"con_consignee"=>$consignee, 
		"con_vehicle_numbers"=>$vehicle_no, 
		"con_consignment_notes"=>$notes, 
		"con_by"=>$by,
		"con_notes"=>$consignt_note,
	],["con_id"=>$con_id]);
if(!$db->error()){
echo "saved";
}else{
	echo "not saved".$db->error();
}
}
if(empty($errors)){
$xx = $db->update("export_contract", [
		"buyer"=>$buyer2, 
		"end_buyer"=>$end_buyer, 
		"destination_port"=>$destination_port, 
		"final_destination"=>$final_destination, 
		"contract_number"=>$contract_number,
		"contract_date"=>strtotime($contract_date),
		"contract_sale_date"=>strtotime($contract_sale_date),
		"us_per_ush"=>$us_per_ush,
		"contract_date"=>strtotime($contract_date),
	],["export_contract_id"=>$export_contract_id]);
if(!$db->error()){
echo "saved";
}else{
	echo "not saved".$db->error();
}
}
}
}





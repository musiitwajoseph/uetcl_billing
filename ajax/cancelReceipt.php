<?php
include "../classes/Db.php";
include "../classes/Field_calculate.inc";
include "../classes/BeforeAndAfter.inc";
include "../classes/AuditTrail.inc";
include "../classes/AccessRights.inc";
include "../classes/Efris.inc";
include ("../qrcode/qrlib.php");

$t = new BeforeAndAfter();

$user_id = 1;//user_id();

$id = $_POST['id'];
$comment = $_POST['comment'];

if(1){
	
	$db = new Db();
	$select = $db->update("receipts",[
		"rec_cancelled"=>'1',
		"rec_cancellation_reason"=>$comment,
		"rec_cancelled_by"=>$user_id,
	], ["rec_id"=>$id]);

echo $db->error();

}
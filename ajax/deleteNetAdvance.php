<?php
include "../classes/Db.php";
include "../classes/Field_calculate.inc";
include "../classes/BeforeAndAfter.inc";
include "../classes/AuditTrail.inc";
include "../classes/AccessRights.inc";
include "../classes/Efris.inc";
include ("../qrcode/qrlib.php");

$t = new BeforeAndAfter();

$user_id = "";
$portion = "advance-schedule";

$id = $_POST['id'];
$comment = $_POST['comment'];

if(1){
	
	$db = new Db();
	$sql = "DELETE FROM readings_comment WHERE rc_id = '$comment'";
	$delete = $db->query($sql);

	$select = $db->update("r_rate",[
		"rate_advance_1"=>NULL,
		"rate_advance_2"=>NULL,
		"rate_advance_3"=>NULL,
		"rate_advance_4"=>NULL,
		"rate_advance_5"=>NULL,
		"rate_advance_6"=>NULL
	], ["rate_reading_id"=>$id]);


}
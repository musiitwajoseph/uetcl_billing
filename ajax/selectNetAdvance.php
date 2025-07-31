
<?php
include "../classes/Db.php";
include "../classes/Field_calculate.inc";
include "../classes/BeforeAndAfter.inc";
include "../classes/AuditTrail.inc";
include "../classes/AccessRights.inc";
include "../classes/Efris.inc";
include ("../qrcode/qrlib.php");

ini_set('display_errors', 1);
$t = new BeforeAndAfter();

$user_id = "";
$portion = "advance-schedule";

$id = $_POST['id'];
$comment = $_POST['comment'];


	
	$db = new Db();
	$select = $db->select("SELECT * FROM readings_comment WHERE rc_id = '$comment'");
	foreach($select[0] as $row){
			extract($row);
			//print_r($row);
    }
    $db = new Db();
	$select2 = $db->select("SELECT * FROM r_rate WHERE rate_reading_id = '$id'");
	foreach($select2[0] as $row2){
			extract($row2);
			//print_r($row2);
    }			

$xx['rate_idd'] = $rate_id;
$xx['comment_idd'] = $rc_id;
echo json_encode($xx);

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

$reading_id = $_POST['reading_id'];

$db = new Db();
$db->query("DELETE FROM r_rate WHERE rate_reading_id='$reading_id'");
$db = new Db();
$db->query("DELETE FROM r_reading where rea_id='$reading_id'");
//delete from r_reading where rea_id = ''
	
				

// $xx['rate_idd'] = $rate_id;
// $xx['comment_idd'] = $rc_id;
// echo json_encode($xx);
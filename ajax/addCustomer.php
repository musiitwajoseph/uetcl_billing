<?php 

session_start();

function user_id(){
	$x = @$_SESSION['UEDCL_USER_ID'];
	return $x;
	return 4;
}

include "../classes/Db.php";
include "../classes/BeforeAndAfter.inc";
include "../classes/AuditTrail.inc";
include "../classes/AccessRights.inc";
include "../classes/Efris.inc";
include ("../qrcode/qrlib.php");

error_reporting(0);
$t = new BeforeAndAfter();
$efris = new Efris();

$db = new Db();
$customer_name = $_POST['customer_name'];
$telephone = $_POST['telephone'];
$category = $_POST['category'];
$street = $_POST['street'];
$tin_number = $_POST['tin_number'];
$cust_type = $_POST['cust_type'];
$email_addres = $_POST['email_addres'];
$short_name = $_POST['short_name'];

$db->insert("customers", ["cust_name"=>$customer_name,"cust_short_name"=>$short_name,"cust_telephone"=>$telephone,"cust_email"=>$email_addres,"cust_street"=>$street,"cust_tin"=>$tin_number,"cust_category"=>$category,"cust_type"=>$cust_type,"cust_date_added"=>time(),"cust_added_by"=>user_id()]);
$json['success'] = "Added";
echo json_encode($json);

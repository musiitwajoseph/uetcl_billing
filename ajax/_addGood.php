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

//$rand = rand(1000, 9999);
$goodsCode = $_POST['goodCode'];
$goodsName = $_POST['goodName'];

// $ref = $_POST['goodID'];
$operationType = $_POST['OperationType'];
// $goodsCode = $_POST['goodCode'];
// $categoryID = $_POST['categoryID'];
$measureUnit = $_POST['uom'];
$unitPrice = $_POST['unitPrice'];
$categoryID = $_POST['categoryID'];
$stockPrewarning = $_POST['stockWarning'];
$currency = $_POST['currency'];
$haveExciseTax = $_POST['hasExciseTax'];

$db->insert("efris_goods", ["eg_code"=>$goodsCode, "eg_name"=>$goodsName,"eg_operation_type"=>$operationType,"eg_uom"=>$measureUnit,"eg_category_id"=>$categoryID,"eg_stock_warning"=>$stockPrewarning,"eg_currency"=>$currency,"eg_has_excise_tax"=>$haveExciseTax,"eg_unit_price"=>$unitPrice]);
$json['success'] = "Added";
echo json_encode($json);

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
$ref = $_POST['goodID'];
$operationType = $_POST['OperationType'];
$goodsCode = $_POST['goodCode'];
$goodsName = $_POST['goodName'];
$measureUnit = $_POST['uom'];
$unitPrice = $_POST['unitPrice'];
$commodityCategoryId = $_POST['categoryID'];
$stockPrewarning = $_POST['stockWarning'];
$currency = $_POST['currency'];
$haveExciseTax = $_POST['hasExciseTax'];

$x = array(
    "operationType"=>$operationType,
	"goodsName"=>$rand.trim($goodsName),
	"goodsCode"=>$rand.trim($goodsCode),
	"measureUnit"=>$measureUnit,
	"unitPrice"=>$unitPrice,
	"currency"=>$currency,
	"commodityCategoryId"=>$commodityCategoryId,
	"haveExciseTax"=>102,//$haveExciseTax,
	"description"=>$description,
	"stockPrewarning"=>$stockPrewarning,
	"pieceMeasureUnit"=>$pieceMeasureUnit,
	"havePieceUnit"=>102,//$havePieceUnit, 101=>YES , 102=>NO
	// "pieceUnitPrice"=>$pieceUnitPrice,
	// "packageScaledValue"=>$packageScaledValue,
	// "pieceScaledValue"=>$pieceScaledValue,
	// "exciseDutyCode"=>$exciseDutyCode,
);

$v = "[".json_encode($x)."]";
$vv = base64_encode($v);

$data_to_send = array(
    "interface"=>"T130",
    "request"=>$vv,
    "request_time"=>date('Y-m-d H:i:s'),
);

//print_r($data_to_send);
//$data = str_replace('\/', '/', $efris->data_to_send($data_to_send));
$data = str_replace('"@@#####@@"', '{}', $efris->data_to_send($data_to_send));

$response = $efris->send_and_receive($data, 'http://127.0.0.1:9880/efristcs/ws/tcsapp/getInformation');

$r = json_decode($response);
        
$error = $r->returnStateInfo->returnCode;
$error_message = $r->returnStateInfo->returnMessage;
ini_set('display_errors',1);
if((int)$error == 0){
	$reference = $r->globalInfo->extendField->referenceNo;
	$db = new Db();
	$insert = $db->update("efris_goods",["eg_mapped"=>1, "eg_date_added"=>time()],["eg_id"=>$ref]);

	//======================= Get Goods ID ===============================================

	$xx = array(
	    "goodsCode"=>"$goodsCode",
	);

	$xv = json_encode($xx);
	$xvv = base64_encode($xv);

	$xdata_to_send = array(
	    "interface"=>"T144",
	    "request"=>$xvv,
	    "request_time"=>date('Y-m-d H:i:s'),
	);

	//print_r($xdata_to_send);
	//$xdata = str_replace('\/', '/', $xefris->data_to_send($xdata_to_send));
	$xdata = str_replace('"@@#####@@"', '{}', $efris->data_to_send($xdata_to_send));

	$xresponse = $efris->send_and_receive($xdata, 'http://127.0.0.1:9880/efristcs/ws/tcsapp/getInformation');

	$xr = json_decode($xresponse);

	$xcontent = $xr->data->content;
	$xc = base64_decode($xcontent);
	$xc = json_decode($xc);
	//var_dump($xc);
	$goodsID = $xc[0]->id;	        
	
	//ini_set('display_errors',1);
	if($goodsID){	
		//$insert = $db->update("efris_goods",["eg_good_id"=>$goodsID],["eg_id"=>$ref]);

$db->update("efris_goods", ["eg_code"=>$goodsCode,"eg_good_id"=>$goodsID,"eg_name"=>$goodsName,"eg_operation_type"=>$operationType,"eg_uom"=>$measureUnit,"eg_category_id"=>$commodityCategoryId,"eg_stock_warning"=>$stockPrewarning,"eg_currency"=>$currency,"eg_has_excise_tax"=>$haveExciseTax,"eg_unit_price"=>$unitPrice],["eg_id"=>$ref]);
	}
	//============================================================================
}

$json = array();
$json['status'] = ((int)$error != 0)? "error" : "success";
$json['error'] = '<b style="color:red;font-weight:bold;">Error: '.$error.' '.$error_message.'</b><br/>'.$v.'<br/>'.$xresponse.'<br/>'.$response;
$json['response'] = '<br/><br/>'.$xresponse.'<br/>'.$response;
$json['reference'] = "Reference: <b>".$reference."</b>";
$json['total'] = "Pushed: <b>".count($goods)."</b>";
$json['allpushed'] = $allchecked;

echo json_encode($json);

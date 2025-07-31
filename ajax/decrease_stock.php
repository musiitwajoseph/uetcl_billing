<?php 

$good = $_POST['good'];
$quantity = $_POST['quantity'];
$adjustType = $_POST['adjustType'];
$comment = $_POST['comment'];

error_reporting(1);
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
$t = new BeforeAndAfter();
$efris = new Efris();

$db = new Db();

$goods = array();
$goods[] = $efris->good($good, $quantity, 111.2);

$x = array(
    "goodsStockIn"=>$efris->goodsStockOut($adjustType, $comment),
    "goodsStockInItem"=>$goods,
);

$v = json_encode($x);
$vv = base64_encode($v);

$data_to_send = array(
    "interface"=>"T131",
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
	
	for($i=0; $i<count($ids); $i++){
		$db = new Db();
		$time = time();
		$d = $ids[$i];
		$db->update("produce",["pro_efris_reference_no"=>$reference,"pro_upload_date"=>$time],["pro_id"=>$d]);
	}
}

$json = array();
$json['status'] = ((int)$error != 0)? "error" : "success";
$json['error'] = '<b style="color:red;font-weight:bold;">Error: '.$good.'...'.$error.' '.$error_message.'</b><br/>'.$response;
$json['response'] = '<br/><br/>'.$v.'<br/>'.$response;
$json['reference'] = "Reference: <b>".$reference."</b>";
$json['total'] = "Pushed: <b>".count($goods)."</b>";
$json['allpushed'] = $allchecked;

echo json_encode($json);

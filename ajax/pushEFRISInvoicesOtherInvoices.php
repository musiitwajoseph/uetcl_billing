<?php 

error_reporting(0);
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

error_reporting(0);
$t = new BeforeAndAfter();
$efris = new Efris();

$db = new Db();

$goods_sum = array();
$goods_sum[$_POST['good']] += $_POST['quantity'];


foreach($goods_sum as $g=>$q){
    $goods[] = $efris->good($g, $q, 1.2);
}

$x = array(
    "goodsStockIn"=>$efris->goodsStockIn(date('Y-m-d')),
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

if((int)$error == 0){
	$reference = $r->globalInfo->extendField->referenceNo;		
}

$json = array();
$json['status'] = ((int)$error != 0)? "error" : "success";
$json['error'] = '<b style="color:red;font-weight:bold;">Error: '.$error.' '.$error_message.'</b><br/>'.$response;
$json['response'] = '<br/><br/>'.$response;
$json['reference'] = "Reference: <b>".$reference."</b>";
$json['total'] = "Quantity Pushed:".$_POST['quantity'];
$json['allpushed'] = 0;

echo json_encode($json);

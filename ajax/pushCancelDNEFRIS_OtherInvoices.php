<?php 

$ori_fdn = $_POST['ori_fdn'];
$cn_id = $_POST['cn_id'];
$id = $_POST['id'];
$reason_code = $_POST['reason_code'];
$reason = $_POST['reason'];
$appCatCode = $_POST['appCatCode'];

error_reporting(NULL);
session_start();

//ini_set('display_errors',1);

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

$t = new BeforeAndAfter();
$efris = new Efris();


$db = new Db();

$id = $t->rgf("dn_other_invoice", $ori_fdn, "oi_efris_cn_ori_invoice_no", "oi_efris_cn_id");

	$x = array(
		"oriInvoiceId"=>$id,
		"invoiceNo"=>$ori_fdn,
		"reason"=>"",
		"reasonCode"=>$reason_code,
		"invoiceApplyCategoryCode"=>103,
	);


$v = json_encode($x);
$vfg = $v;
$v = str_replace('"0":{', '', $v);
$v = str_replace(',"%%%":"%%%"}', '', $v);
$vv = base64_encode($v);


$data_to_send = array(
    "interface"=>"T114",
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
	$errorz2[] = '<b>'.'-'.date('Y', ($r_date)).'-'.date('m', ($r_date)).'</b> Successfully pushed.';
	$content = $r->data->content;
	$c = base64_decode($content);

	$c = json_decode($c);

	$reference = $c->referenceNo;
	
	//$qr = $efris->generateQrCode($qr, $inv_no);
	
	$db = new Db();

	//if(empty($content))
	{
		$db->update("dn_other_invoice",["oi_status"=>101010],["oi_efris_cn_ori_invoice_no"=>$ori_fdn]);
	}

	
	// $db->select("select * from efris_invoice");
	// $num = $db->num_rows();
}

$json = array();
$json['status'] = ((int)$error != 0)? "error" : "success";
$json['error'] = '<b style="color:red;font-weight:bold;">Error: '.$error.' '.$error_message.'</b><br/> '.$vfg.'<br/>'.$response.$cc;
$json['response'] = '<br/>'.$vfg.'<br/>'.$response.$cc;
$json['reference'] = "Reference: <b>".$reference."</b>";
$json['total'] = "Pushed: <b>".count($goods)."</b>";
$json['allpushed'] = $allchecked;


echo json_encode($json);

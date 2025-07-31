<?php 

$tin = $_POST['tin'];

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

$x = array(
	"tin"=>$tin,
);
$v = json_encode($x);
$vfg = $v;
$v = str_replace('"0":{', '', $v);
$v = str_replace(',"%%%":"%%%"}', '', $v);
$vv = base64_encode($v);


$data_to_send = array(
    "interface"=>"T119",
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

	$address = $c->taxpayer->address;	
	$businessName = $c->taxpayer->businessName;
	$contactEmail = $c->taxpayer->contactEmail;
	$contactNumber = $c->taxpayer->contactNumber;
	$legalName = $c->taxpayer->legalName;
	$taxpayerStatus = $c->taxpayer->taxpayerStatus;
	$taxpayerType = $c->taxpayer->taxpayerType;	
	$tin = $c->taxpayer->tin;	
	
	$values = $address.'@@@'.$businessName.'@@@'.$contactEmail.'@@@'.$contactNumber.'@@@'.$legalName.'@@@'.$taxpayerStatus.'@@@'.$taxpayerType.'@@@'.$tin;
	$table = '<input id="values" type="hidden" value="'.$values.'"/>';

	$table .= '<table border="1" cellspacing="0" cellpadding="1" id="table">';
	
	$table .= '<tr>';
	$table .= '<td>Address:</td>';
	$table .= '<td>'.$address.'</td>';
	$table .= '</tr>';
	
	$table .= '<tr>';
	$table .= '<td>Business Name:</td>';
	$table .= '<td>'.$businessName.'</td>';
	$table .= '</tr>';
	
	$table .= '<tr>';
	$table .= '<td>Contact Email:</td>';
	$table .= '<td>'.$contactEmail.'</td>';
	$table .= '</tr>';
	
	$table .= '<tr>';
	$table .= '<td>Contact Number:</td>';
	$table .= '<td>'.$contactNumber.'</td>';
	$table .= '</tr>';
	
	$table .= '<tr>';
	$table .= '<td>LegalName:</td>';
	$table .= '<td>'.$contactEmail.'</td>';
	$table .= '</tr>';
	
	$table .= '<tr>';
	$table .= '<td>Taxpayer Status:</td>';
	$table .= '<td>'.$taxpayerStatus.'</td>';
	$table .= '</tr>';
	
	$table .= '<tr>';
	$table .= '<td>Taxpayer Type:</td>';
	$table .= '<td>'.$taxpayerType.'</td>';
	$table .= '</tr>';
	
	$table .= '<tr>';
	$table .= '<td>TIN:</td>';
	$table .= '<td>'.$tin.'</td>';
	$table .= '</tr>';

	$table .= '<table>';
	$table .= '<br/>';
	$table .= '<button type="button" class="btn btn-xs btn-primary" id="auto_fill"><i class="fa fa-fw fa-flask"></i>Auto Fill Details</button>';
	
}

$json = array();
$json['status'] = ((int)$error != 0)? "error" : "success";
$json['error'] = '<b style="color:red;font-weight:bold;">Error: '.$error.' '.$error_message.'</b><br/> '.$vfg.'<br/>'.$response.$cc;
$json['response'] = '<br/>'.$vfg.'<br/>'.$response.$cc.$table;
$json['reference'] = "Reference: <b>".$reference."</b>";
$json['total'] = "Pushed: <b>".count($goods)."</b>";
$json['allpushed'] = $allchecked;


echo json_encode($json);

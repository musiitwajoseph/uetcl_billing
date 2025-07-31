<?php 

//$id = $_POST['id'];
error_reporting(null);
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

$tin = $_POST['tin'];
$ccc = $_POST['ccc'];

$x = array(
    "invoiceNo"=>$tin,
    //"commodityCategoryCode"=>$ccc,
);

$v = json_encode($x);
$vv = base64_encode($v);

$data_to_send = array(
    "interface"=>"T108",
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
	$returned = $r->data->content;	
	$returned = base64_decode($returned);
	$returned = json_decode($returned);

	//var_dump($returned);

$x = array(
	'101'=>'normal taxpayer',
	'102'=>'exempt taxpayer', 
	'103'=>'Deemed taxpayer'
);

	echo '<table id="table" border="1">';
	echo '<tr>';
	echo '<th>Commodity Category Code</th>';
	echo '<th>Name</th>';
	echo '<th>Taxpayer status Code</th>';
	echo '<th>Taxpayer Status</th>';
	echo '</tr>';
	foreach($returned as $r){
		//var_dump($r);
		foreach($r as $rr){

			$t = new BeforeAndAfter();
			echo '<tr>';
			echo '<td>'.$rr->commodityCategoryCode.'</td>';
			echo '<td>'.$t->rgf("efris_goods",$rr->commodityCategoryCode, "eg_good_id", "eg_name").' ('.$t->rgf("efris_goods",$rr->commodityCategoryCode, "eg_good_id", "eg_code").')</td>';
			echo '<td>'.$rr->commodityCategoryTaxpayerType.'</td>';
			echo '<td>'.$x[$rr->commodityCategoryTaxpayerType].'</td>';
			echo '</tr>';
		}
	}

	echo '</table>';
}

//echo $r;

$json = array();
$json['status'] = ((int)$error != 0)? "error" : "success";

echo '<br/><Br/>'.$json['error'] = $response;
$json['response'] = '';//'<br/>'.$id.'<br/>'.$response;
$json['returned'] = $rr;
$json['returned_warning'] = "<b>".$returned_warning."</b>";
$json['total'] = "Pushed: <b>".count($goods)."</b>";
$json['allpushed'] = $allchecked;

//echo json_encode($json);

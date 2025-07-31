<?php 
error_reporting(NULL);
session_start();

$qty = $_POST['qty'];
$unitPrice = $_POST['unitPrice'];
$currency = $_POST['currency'];
$id = $_POST['id'];

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
$sql = "SELECT * FROM efris_invoice where ei_id = '$id'";
$select = $db->select($sql);
if($db->num_rows())
extract($select[0][0]);
$ref = $ei_ref.' CREDIT NOTE';
//echo $ei_invoice_id.'@@'.$ei_inv_no;

$name = $t->rgf("valid_names", $ei_buyer, "valid_name_id", "name");
$customer_type = $t->rgf("valid_names", $ei_buyer, "valid_name_id", "customer_type");
if(empty($customer_type)) $customer_type = "B2B";
$tin = $t->rgf("valid_names", $ei_buyer, "valid_name_id", "tin");

$estateName = strtolower($t->rgf("valid_names", $ei_item, "valid_name_id", "name"));

$good = $efris->stockGoods();

$items = array();
$v=0;
$items[] = $efris->item($good[$estateName][1],$good[$estateName][3], $qty, $unitPrice, '50201710', $good[$estateName][2],$v++);
$remarks ="ACCOUNT SALE $estateName $ref";
$code ="tset";


$x = array(
	//"sellerDetails"=>$efris->sellers_info($ei_ref),
	"basicInformation"=>$efris->basic_information($ei_ref, date('Y-m-d H:i:s'), 'UGX', $t->full_name(user_id())),
	"buyerDetails"=>$efris->buyer_details($customer_type, $tin, $name ),
	$efris->creditNote(
		$ei_invoice_id, 
		$ei_inv_no, 
		'CREDIT NOTE', 
		$name, 
		$cust_telephone, 
		$cust_email, 
		$remarks, 
		'CREDIT NOTE '.$ei_ref,
		$currency,
	),	
	"goodsDetails"=>$items,
	"taxDetails"=>$efris->taxDetails($items, $currency),
	"summary"=>$efris->summary($items, $remarks, $code),
	"payWay"=>$efris->payWay(1, $items),
);


$v = json_encode($x);
$v = str_replace('"0":{', '', $v);
$v = str_replace(',"%%%":"%%%"}', '', $v);
$vfg = $v;
$vv = base64_encode($v);

$data_to_send = array(
    "interface"=>"T110",
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
	$errorz2[] = '<b>'.$cust_meter_no.'-'.date('Y', ($r_date)).'-'.date('m', ($r_date)).'</b> Successfully pushed.';
	$content = $r->data->content;
	$c = base64_decode($content);
	$c = json_decode($c);
	
	$reference = $c->referenceNo;	
	if(1){
		$ecn_reference = $reference;
		$db = new Db();
		$insert = $db->insert("efris_credit_note", [
			"ecn_reference"=>$ecn_reference, 
			"ecn_ori_id"=>$ei_id, 
			"ecn_qty"=>$qty, 
			"ecn_unit_price"=>$unitPrice,
			"ecn_date_added"=>time(),
			"ecn_added_by"=>user_id(),
			"ecn_ref"=>$ref,
		]);
	}
	
	if($ecn_reference != ""){ 
		//T111
		$xq = array(
			"referenceNo"=>$ecn_reference,
			"queryType"=>1,
		);		
		
		$v = json_encode($xq);
		$vv = base64_encode($v);
		
		$data_to_send = array(
			"interface"=>"T111",
			"request"=>$vv,
			"request_time"=>date('Y-m-d H:i:s'),
		);
		//$data = str_replace('\/', '/', $efris->data_to_send($data_to_send));
		$data = str_replace('"@@#####@@"', '{}', $efris->data_to_send($data_to_send));
					
		$response = $efris->send_and_receive($data, 'http://127.0.0.1:9880/efristcs/ws/tcsapp/getInformation');
		
		$r = json_decode($response);
				
		$error = $r->returnStateInfo->returnCode;
		$error_message = $r->returnStateInfo->returnMessage;
		
		if((int)$error != 0){
			// $error_message;
		}else{			
			$content = $r->data->content;
			$c = base64_decode($content);
			$ca = $c;
			//echo $c;
			$c = json_decode($c);
			$no = $c->records[0]->id;
			$status = $c->records[0]->approveStatus;
		}
		}else{
			$no = $ecn_no;
			$status = $ecn_status;
		}
		//T108
		
		$insert = $db->update("efris_credit_note", [
			"ecn_no"=>$no, 
			"ecn_status"=>$status,
		],["ecn_ori_id"=>$ei_id]);
	}
	
	if($ecn_status == 101 && empty($ecn_fdn)){
		//T108
		$xq = array(
			"invoiceNo"=>$no,
		);
		
		$v = json_encode($xq);
		$vv = base64_encode($v);
		
		$data_to_send = array(
			"interface"=>"T108",
			"request"=>$vv,
			"request_time"=>date('Y-m-d H:i:s'),
		);
		//$data = str_replace('\/', '/', $efris->data_to_send($data_to_send));
		$data = str_replace('"@@#####@@"', '{}', $efris->data_to_send($data_to_send));
	
		$response = $efris->send_and_receive($data, 'http://127.0.0.1:9880/efristcs/ws/tcsapp/getInformation');
		
		$r = json_decode($response);
				
		$error = $r->returnStateInfo->returnCode;
		$error_message = $r->returnStateInfo->returnMessage;
		if((int)$error != 0){
			echo $error_message;
		}else{			
			$content = $r->data->content;
			$c = base64_decode($content);
			//echo $c;
			$c = json_decode($c);
			
			//echo '<pre>';
			//print_r($c);
			//echo '</pre>';
			
			//$afc = $c->basicInformation->antifakeCode;
			$inv_no = $c->basicInformation->invoiceNo;
			$qrcode = $c->summary->qrCode;
			$vcode = $c->basicInformation->antifakeCode;
		
			$insert = $db->update("efris_credit_note", [
				"ecn_inv_no"=>$inv_no, 
				"ecn_qr"=>$qrcode,
				"ecn_vcode"=>$vcode,
			],["ecn_ori_id"=>$ei_id]);
		}
	}
	
	$db = new Db();
	/*$insert = $db->insert("efris_invoice", [
	"ei_afc"=>$afc, 
	"ei_inv_no"=>$inv_no, 
	"ei_qr"=>$qr, 
	"ei_invoice_id"=>$id, 
	"ei_ref"=>$ref, 
	"ei_buyer"=>$buyer, 
	"ei_item"=>$item, 
	"ei_qty"=>$qty, 
	"ei_unit_price"=>$unitPrice,
	"ei_market"=>"AUCTION",
	"ei_date_added"=>time(),
	"ei_added_by"=>user_id(),
	"ei_tax_rate"=>$tax_rate,
	"ei_currency"=>$currency,
	"ei_tax_rate_name"=>$tax_rate_name,
	]);
	*/
	
	$db->select("select * from efris_invoice");
	$num = $db->num_rows();

$vfg = $ca;
$json = array();
$json['status'] = ((int)$error != 0)? "error" : "success";
$json['error'] = '<b style="color:red;font-weight:bold;">Error: '.$vfg.' '.$error.' '.$error_message.'</b><br/>'.$response;
$json['response'] = '<br/>'.$vfg.'<br/>'.$response;
$json['reference'] = "Reference: <b>".$reference."</b>";
$json['total'] = "Pushed: <b>".count($goods)."</b>";
$json['allpushed'] = $allchecked;


echo json_encode($json);

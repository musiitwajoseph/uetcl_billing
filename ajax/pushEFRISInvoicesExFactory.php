<?php 

$number = $_POST['items'];
$buyer = $_POST['buyer'];
$id_collector = $_POST['id_collector'];
$currency = $_POST['currency'];
$ref = $_POST['ref'];

$ref = 'TEST'.$_POST['ref'];

error_reporting(1);
session_start();

ini_set('display_errors',1);

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

error_reporting(1);

$t = new BeforeAndAfter();
$efris = new Efris();

$db = new Db();

$name = $t->rgf("valid_names", $buyer, "valid_name_id", "name");
$customer_type = $t->rgf("valid_names", $buyer, "valid_name_id", "customer_type");
if(empty($customer_type)) $customer_type = "B2B";
$tin = $t->rgf("valid_names", $buyer, "valid_name_id", "tin");


$good = $efris->stockGoods();

$items = array();
$v=0;

for($i=1; $i<=$number; $i++){
	
	$item = $_POST['item'.$i];
	$qty = str_replace(',','',$_POST['qty'.$i]);
	$unitPrice = str_replace(',','',$_POST['unitPrice'.$i]);
	
	$estateName = strtolower($t->rgf("valid_names", $item, "valid_name_id", "name"));
	$items[] = $efris->item($good[$estateName][1],$good[$estateName][3], $qty, $unitPrice, '50201710', $good[$estateName][2],$v++,0);
}


$remarks ="ACCOUNT SALE $ref ".strtoupper($estateName);
$code ="tset";


$x = array(
	"sellerDetails"=>$efris->sellers_info('AS '.$ref),
	"basicInformation"=>$efris->basic_information('AS '.$ref, date('Y-m-d H:i:s'), $currency, $t->full_name(user_id())),
	"buyerDetails"=>$efris->buyer_details($customer_type, $tin, $name ),		
	"goodsDetails"=>$items,
	"taxDetails"=>$efris->taxDetails($items, $currency),
	"summary"=>$efris->summary($items, $remarks, $code),
	"payWay"=>$efris->payWay(1, $items),
);

$v = json_encode($x);
$vfg = $v;
$vv = base64_encode($v);

$data_to_send = array(
    "interface"=>"T109",
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

	$afc = $c->basicInformation->antifakeCode;
	$inv_no = $c->basicInformation->invoiceNo;
	$qr = $c->summary->qrCode;
	$id = $c->basicInformation->invoiceId;
	
	$tax_rate = $c->taxDetails[0]->taxRate;
	$tax_rate_name = $c->taxDetails[0]->taxRateName;
	$currency = $c->taxDetails[0]->exciseCurrency;
	
	$qr = $efris->generateQrCode($qr);
	
	$db = new Db();
	$insert = $db->insert("efris_invoice", [
	"ei_afc"=>$afc, 
	"ei_inv_no"=>$inv_no, 
	"ei_qr"=>$qr, 
	"ei_invoice_id"=>$id, 
	"ei_ref"=>'AS '.$ref, 
	"ei_buyer"=>$buyer, 
	"ei_item"=>$item, 
	"ei_qty"=>$qty, 
	"ei_unit_price"=>$unitPrice,
	"ei_market"=>"EXFACTORY",
	"ei_date_added"=>time(),
	"ei_added_by"=>user_id(),
	"ei_tax_rate"=>$tax_rate,
	"ei_currency"=>$currency,
	"ei_tax_rate_name"=>$tax_rate_name,
	]);
	
	$db = new Db();
	
		
	for($i=1; $i<=$number; $i++){		
		$item = $_POST['item'.$i];
		$qty = str_replace(',','',$_POST['qty'.$i]);
		$unitPrice = str_replace(',','',$_POST['unitPrice'.$i]);
	
		$insert = $db->insert("efris_invoice_details", [
		"eid_inv_no"=>$inv_no, 
		"eid_item"=>$item, 
		"eid_qty"=>$qty, 
		"eid_unit_price"=>$unitPrice,
		]);
	}
	
	$db = new Db();
	$id_collector = explode(',',$id_collector);
	foreach($id_collector as $id){
		$db->update("invoice",["inv_efris_fdn"=>$inv_no],["inv_id"=>$id]);
	}
	
	
	$db->select("select * from efris_invoice");
	$num = $db->num_rows();
}

$json = array();
$json['status'] = ((int)$error != 0)? "error" : "success";
$json['error'] = '<b style="color:red;font-weight:bold;">Error: '.$error.' '.$error_message.'</b><br/> '.$vfg.'<br/>'.$response;
$json['response'] = '<br/>'.$vfg.'<br/>'.$response;
$json['reference'] = "Reference: <b>".$reference."</b>";
$json['total'] = "Pushed: <b>".count($goods)."</b>";
$json['allpushed'] = $allchecked;


echo json_encode($json);

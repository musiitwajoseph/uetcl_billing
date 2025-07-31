<?php 

$number = $_POST['items'];
$date = $_POST['date'];
$buyer = $_POST['buyer'];
$id_collector = $_POST['id'];
$currency = $_POST['currency'];
$ref = ''.$_POST['ref'];//rand(1000, 9999);
$efris_id = $_POST['efris_id'];

$vat = $_POST['vat'];

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

$name = $t->rgf("customers",$buyer,"cust_id","cust_name");
$customer_type = $t->rgf("customers",$buyer,"cust_id","cust_type");
if(empty($customer_type)) $customer_type = "B2B";
$tin = $t->rgf("customers",$buyer,"cust_id","cust_tin");
$telephone = $t->rgf("customers",$buyer,"cust_id","cust_telephone");
$email = $t->rgf("customers",$buyer,"cust_id","cust_email");

$efris_fdn = $t->rgf("other_invoice",$efris_id,"oi_invoice_id","oi_fdn");
$Coi_date = $t->rgf("other_invoice",$efris_id,"oi_invoice_id","oi_date");
$Coi_id = $t->rgf("other_invoice",$efris_id,"oi_invoice_id","oi_id");

//$ref = ''.date('y-m/', $Coi_date).str_pad($Coi_id, 5, '0', STR_PAD_LEFT);

$currency = $t->rgf("dictionary", $currency, "d_code", "d_name");

$items = array();
$v=0;

for($i=1; $i<=$number; $i++){
	
	$item = $_POST['item'.$i];
	$qty = 1*str_replace(',','',$_POST['qty'.$i]);
	$unitPrice = str_replace(',','',$_POST['unitPrice'.$i]);
	
	$estateName = "";//$item; //strtolower($t->rgf("valid_names", $item, "valid_name_id", "name"));
	//($item_code, $item_name, $qty, $amount, $item_id, $uom, $orderNumber, $tax=0)

	$tax_rate =($vat == "RES" || $vat == 'NRS' || $vat == 'IMP' || $vat == 'IMPWHT') ? 18 : 0;
	
	if($tax_rate==18){
		$items[] = $efris->item(
			$t->rgf("efris_goods", $item, "eg_id", "eg_code"),
			$t->rgf("efris_goods", $item, "eg_id", "eg_name"),
			$qty,
			$unitPrice,
			$t->rgf("efris_goods", $item, "eg_id", "eg_category_id"),
			$t->rgf("efris_goods", $item, "eg_id", "eg_uom"),
			$v++,
			18
		);
	}else{
		if($vat == 'EXP'){
			$items[] = $efris->item(
				$t->rgf("efris_goods", $item, "eg_id", "eg_code"),
				$t->rgf("efris_goods", $item, "eg_id", "eg_name"),
				$qty,
				$unitPrice,
				$t->rgf("efris_goods", $item, "eg_id", "eg_category_id"),
				$t->rgf("efris_goods", $item, "eg_id", "eg_uom"),
				$v++,
				'zero'
			);
		}else{
			$items[] = $efris->item(
				$t->rgf("efris_goods", $item, "eg_id", "eg_code"),
				$t->rgf("efris_goods", $item, "eg_id", "eg_name"),
				$qty,
				$unitPrice,
				$t->rgf("efris_goods", $item, "eg_id", "eg_category_id"),
				$t->rgf("efris_goods", $item, "eg_id", "eg_uom"),
				$v++,
			);
		}
		//$items[] = $efris->item($good[$estateName][1],$good[$estateName][3], $qty, $unitPrice, $good[$estateName][5], $good[$estateName][2],$v++);
	}
}


$remarks =" $ref "; //.strtoupper($estateName);
$code ="";

if($tax_rate==18){
	if($vat == 'IMP' || $vat == 'IMPWHT'){		
		$x = array(
			"sellerDetails"=>$efris->sellers_info($ref),
			"basicInformation"=>$efris->basic_information2($ref, date('Y-m-d', $date).date(' H:i:s'), $currency, $t->full_name(user_id()),1,4,$efris_id),
			"buyerDetails"=>$efris->buyer_details(B2B, '1000025097', 'UETCL', 104),		
			$efris->debitNote($efris_id, 
				$efris_fdn, 
				'DEBIT NOTE', 
				$name, 
				$telephone, 
				$email, 
				$remarks, 
				'DN-'.$ref,
				$currency
			),	
			"goodsDetails"=>$items,
			"taxDetails"=>$efris->taxDetailsVAT18($items, $currency),
			"summary"=>$efris->summary($items, $remarks, $code),
			"payWay"=>$efris->payWay(1, $items),
			"importServicesSeller"=>$efris->importServicesSeller($name, $email, $tel, $address, date('Y-m-d ', $date).date('H:i:s')),
			"extend"=>$efris->extend2(101),
		);		
	}else{		
		$x = array(
			"sellerDetails"=>$efris->sellers_info($ref),
			"basicInformation"=>$efris->basic_information2($ref, date('Y-m-d', $date).date(' H:i:s'), $currency, $t->full_name(user_id()),1,4,$efris_id),
			"buyerDetails"=>$efris->buyer_details($customer_type, $tin, $name ),	
			$efris->debitNote($efris_id, 
				$efris_fdn, 
				'DEBIT NOTE', 
				$name, 
				$telephone, 
				$email, 
				$remarks, 
				'DN-'.$ref,
				$currency
			),	
			"goodsDetails"=>$items,
			"taxDetails"=>$efris->taxDetailsVAT18($items, $currency),
			"summary"=>$efris->summary($items, $remarks, $code),
			"payWay"=>$efris->payWay(1, $items),
			"extend"=>$efris->extend2(101),
		);		
	}
}else{
	if($vat == 'EXP'){
		$x = array(
			"sellerDetails"=>$efris->sellers_info($ref),
			"basicInformation"=>$efris->basic_information2($ref, date('Y-m-d', $date).date(' H:i:s'), $currency, $t->full_name(user_id()),1,4,$efris_id),
			"buyerDetails"=>$efris->buyer_details($customer_type, $tin, $name ),
			$efris->debitNote($efris_id, 
				$efris_fdn, 
				'DEBIT NOTE', 
				$name, 
				$telephone, 
				$email, 
				$remarks, 
				'DN-'.$ref,
				$currency
			),		
			"goodsDetails"=>$items,
			"taxDetails"=>$efris->taxDetailsZERO($items, $currency),
			"summary"=>$efris->summary($items, $remarks, $code),
			"payWay"=>$efris->payWay(1, $items),
			"extend"=>$efris->extend2(101),
		);
	}else if($customer_type != 'EXP'){
		$x = array(
			"sellerDetails"=>$efris->sellers_info($ref),
			"basicInformation"=>$efris->basic_information2($ref, date('Y-m-d', $date).date(' H:i:s'), $currency, $t->full_name(user_id()),1,4,$efris_id),
			"buyerDetails"=>$efris->buyer_details($customer_type, $tin, $name ),
			$efris->debitNote($efris_id, 
				$efris_fdn, 
				'DEBIT NOTE', 
				$name, 
				$telephone, 
				$email, 
				$remarks, 
				'DN-'.$ref,
				$currency
			),		
			"goodsDetails"=>$items,
			"taxDetails"=>$efris->taxDetails($items, $currency),
			"summary"=>$efris->summary($items, $remarks, $code),
			"payWay"=>$efris->payWay(1, $items),
			"extend"=>$efris->extend2(101),
		);
	}else{
		$x = array(
			"sellerDetails"=>$efris->sellers_info($ref),
			"basicInformation"=>$efris->basic_information2($ref, date('Y-m-d', $date).date(' H:i:s'), $currency, $t->full_name(user_id()),1,4,$efris_id),
			"buyerDetails"=>$efris->buyer_details($customer_type, $tin, $name ),
			$efris->debitNote($efris_id, 
				$efris_fdn, 
				'DEBIT NOTE', 
				$name, 
				$telephone, 
				$email, 
				$remarks, 
				'DN-'.$ref,
				$currency
			),		
			"goodsDetails"=>$items,
			"taxDetails"=>$efris->taxDetailsZERO($items, $currency),
			"summary"=>$efris->summary($items, $remarks, $code),
			"payWay"=>$efris->payWay(1, $items),
			"extend"=>$efris->extend2(101),
		);		
	}
}

$v = json_encode($x);
$v = str_replace('"0":{', '', $v);
$v = str_replace(',"%%%":"%%%"}', '', $v);
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
	$errorz2[] = '<b>'.'-'.date('Y', ($r_date)).'-'.date('m', ($r_date)).'</b> Successfully pushed.';
	$content = $r->data->content;
	$c = base64_decode($content);

	$c = json_decode($c);

	$reference = $c->sellerDetails->referenceNo;
	
	$afc = $c->basicInformation->antifakeCode;
	$invoiceNo = $c->basicInformation->invoiceNo;
	$oriInvoiceNo = $c->basicInformation->oriInvoiceNo;
	$qrcode = $c->summary->qrCode;
	$id = $c->basicInformation->invoiceId;	
	$vcode = $c->basicInformation->antifakeCode;

	$qr = $efris->generateQrCode($qrcode, $oriInvoiceNo);
	
	$db = new Db();

	{
		//$db->update("dn_other_invoice",["oi_reference_no"=>$reference],["oi_id"=>$id_collector]);
		$db->update("dn_other_invoice",["oi_reference_no"=>$reference, "oi_efris_cn_app_cat_code"=>$afc, "oi_efris_cn_ori_invoice_no"=>$oriInvoiceNo, "oi_efris_cn_id"=>$id, "oi_status"=>101, "oi_cn_fdn"=>$invoiceNo, "oi_cn_vc"=>$vcode, "oi_cn_qr_code"=>$qr],["oi_id"=>$id_collector]);
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

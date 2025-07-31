<?php 

$number = $_POST['items'];
$date = $_POST['date'];
$buyer = $_POST['buyer'];
$id_collector = $_POST['id'];
$currency = $_POST['currency'];
$ref = $_POST['ref'];//'TEST'.$_POST['ref'].rand(1000, 9999);
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
	$qty = str_replace(',','',$_POST['qty'.$i]);
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

// if($vat=='VATABLE(18%)'){
// 	$x = array(
// 		"sellerDetails"=>$efris->sellers_info('AS '.$ref),
// 		"basicInformation"=>$efris->basic_information($ref, date('Y-m-d H:i:s', $date), $currency, $t->full_name(user_id())),
// 		"buyerDetails"=>$efris->buyer_details($customer_type, $tin, $name ),
// 		$efris->creditNote($efris_id, 
// 			$efris_fdn, 
// 			'CREDIT NOTE', 
// 			$name, 
// 			$telephone, 
// 			$email, 
// 			$remarks, 
// 			'CN-'.$ref,
// 			$currency
// 		),		
// 		"goodsDetails"=>$items,
// 		"taxDetails"=>$efris->taxDetailsVAT18($items, $currency),
// 		"summary"=>$efris->summary($items, $remarks, $code),
// 		"payWay"=>$efris->payWay(1, $items),
// );
// }else{
// 	$x = array(
// 		"sellerDetails"=>$efris->sellers_info(''.$ref),
// 		"basicInformation"=>$efris->basic_information(''.$ref, date('Y-m-d H:i:s', $date), $currency, $t->full_name(user_id())),
// 		"buyerDetails"=>$efris->buyer_details($customer_type, $tin, $name ),	
// 		$efris->creditNote($efris_id, 
// 			$efris_fdn, 
// 			'CREDIT NOTE', 
// 			$name, 
// 			$telephone, 
// 			$email, 
// 			$remarks, 
// 			'CN-'.$ref,
// 			$currency
// 		),	
// 		"goodsDetails"=>$items,
// 		"taxDetails"=>$efris->taxDetails($items, $currency),
// 		"summary"=>$efris->summary($items, $remarks, $code),
// 		"payWay"=>$efris->payWay(1, $items),
// 	);
// }


if($tax_rate==18){
	if($vat == 'IMP' || $vat == 'IMPWHT'){		
		$x = array(
			"sellerDetails"=>$efris->sellers_info($ref),
			"basicInformation"=>$efris->basic_information($ref, date('Y-m-d ', $date).date('H:i:s'), $currency, $t->full_name(user_id())),
			"buyerDetails"=>$efris->buyer_details(B2B, '1000025097', 'UETCL' ),		
			"goodsDetails"=>$items,
			"taxDetails"=>$efris->taxDetailsVAT18($items, $currency),
			"summary"=>$efris->summary($items, $remarks, $code),
			"payWay"=>$efris->payWay(1, $items),
			"importServicesSeller"=>$efris->importServicesSeller($name,$email, $tel, $address, date('Y-m-d ', $date).date('H:i:s')),
		);		
	}else{		
		$x = array(
			"sellerDetails"=>$efris->sellers_info(''.$ref),
			"basicInformation"=>$efris->basic_information($ref, date('Y-m-d H:i:s', $date), $currency, $t->full_name(user_id())),
			"buyerDetails"=>$efris->buyer_details($customer_type, $tin, $name ),
			$efris->creditNote($efris_id, 
				$efris_fdn, 
				'CREDIT NOTE', 
				$name, 
				$telephone, 
				$email, 
				$remarks, 
				$ref,
				$currency
			),		
			"goodsDetails"=>$items,
			"taxDetails"=>$efris->taxDetailsVAT18($items, $currency),
			"summary"=>$efris->summary($items, $remarks, $code),
			"payWay"=>$efris->payWay(1, $items),
		);	
	}
}else{
	if($vat == 'EXP'){
		
		$x = array(
			"sellerDetails"=>$efris->sellers_info(''.$ref),
			"basicInformation"=>$efris->basic_informationZERO($ref, date('Y-m-d ', $date).date('H:i:s'), $currency, $t->full_name(user_id())),
			"buyerDetails"=>$efris->buyer_details($customer_type, $tin, $name ),	
			$efris->creditNote($efris_id, 
				$efris_fdn, 
				'CREDIT NOTE', 
				$name, 
				$telephone, 
				$email, 
				$remarks, 
				'CN-'.$ref,
				$currency
			),	
			"goodsDetails"=>$items,
			"taxDetails"=>$efris->taxDetailsZERO($items, $currency),
			"summary"=>$efris->summary($items, $remarks, $code),
			"payWay"=>$efris->payWay(1, $items),
		);
	}else if($customer_type != 'EXP'){
		$x = array(
			"sellerDetails"=>$efris->sellers_info(''.$ref),
			"basicInformation"=>$efris->basic_information($ref, date('Y-m-d H:i:s', $date), $currency, $t->full_name(user_id())),
			"buyerDetails"=>$efris->buyer_details($customer_type, $tin, $name ),
			$efris->creditNote($efris_id, 
				$efris_fdn, 
				'CREDIT NOTE', 
				$name, 
				$telephone, 
				$email, 
				$remarks, 
				$ref,
				$currency
			),		
			"goodsDetails"=>$items,
			"taxDetails"=>$efris->taxDetailsVAT18($items, $currency),
			"summary"=>$efris->summary($items, $remarks, $code),
			"payWay"=>$efris->payWay(1, $items),
		);
	}else{
		$x = array(
			"sellerDetails"=>$efris->sellers_info($ref),
			"basicInformation"=>$efris->basic_information($ref, date('Y-m-d ', $date).date('H:i:s'), $currency, $t->full_name(user_id())),
			"buyerDetails"=>$efris->buyer_details($customer_type, $tin, $name ),		
			"goodsDetails"=>$items,
			"taxDetails"=>$efris->taxDetailsZERO($items, $currency),
			"summary"=>$efris->summary($items, $remarks, $code),
			"payWay"=>$efris->payWay(1, $items),
		);		
	}
}

$v = json_encode($x);
$vfg = $v;
$v = str_replace('"0":{', '', $v);
$v = str_replace(',"%%%":"%%%"}', '', $v);
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
	$errorz2[] = '<b>'.'-'.date('Y', ($r_date)).'-'.date('m', ($r_date)).'</b> Successfully pushed.';
	$content = $r->data->content;
	$c = base64_decode($content);

	$c = json_decode($c);

	$reference = $c->referenceNo;
	
	//$qr = $efris->generateQrCode($qr, $inv_no);
	
	$db = new Db();

	{
		$db->update("cn_other_invoice",["oi_reference_no"=>$reference],["oi_id"=>$id_collector]);
	}

	if($reference){
		$xq = array(
			"referenceNo"=>$reference,
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
			echo $error_message;
		}else{			
			$content = $r->data->content;
			$c = base64_decode($content);
			$cc = '<br/>@@@: '.$c;
			$c = json_decode($c);
			
			$fdn = $c->records[0]->oriInvoiceNo;
			$id = $c->records[0]->id;
			$app_code = $c->records[0]->invoiceApplyCategoryCode;
			$status = $c->records[0]->approveStatus;

			$db->update("cn_other_invoice",["oi_efris_cn_app_cat_code"=>$app_code, "oi_efris_cn_ori_invoice_no"=>$fdn, "oi_efris_cn_id"=>$id, "oi_status"=>$status],["oi_id"=>$id_collector]);
		}
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

<?php 

$number = $_POST['items'];
$date = $_POST['date'];
$buyer = $_POST['buyer'];
$id_collector = $_POST['id'];
$currency = $_POST['currency'];
$ref = ''.$_POST['ref'];//.rand(1000, 9999);
$remarks = $_POST['remarks'];

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
$address = 'KAMPALA';//$t->rgf("customers",$buyer,"cust_id","cust_address");
$customer_type = $t->rgf("customers",$buyer,"cust_id","cust_type");
if(empty($customer_type)) $customer_type = "B2B";
$tin = $t->rgf("customers",$buyer,"cust_id","cust_tin");

$Coi_date = $t->rgf("other_invoice",$id_collector,"oi_id","oi_date");
$Coi_id = $id_collector;//$t->rgf("other_invoice",$id_collector,"oi_invoice_id","oi_id");

//$ref = 'CN '.date('y-m/', $Coi_date).str_pad($Coi_id, 5, '0', STR_PAD_LEFT);


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

//$remarks =" $ref "; //.strtoupper($estateName);
$code ="";
$currency = $t->rgf("dictionary", $currency, "d_code", "d_name");
if($tax_rate==18){
	if($vat == 'IMP' || $vat == 'IMPWHT'){		
		$x = array(
			"sellerDetails"=>$efris->sellers_info($ref),
			"basicInformation"=>$efris->basic_information($ref, date('Y-m-d ', $date).date('H:i:s'), $currency, $t->full_name(user_id()),1,1, 104, 106),
			"buyerDetails"=>$efris->buyer_details(B2B, '1000025097', 'UETCL', 104),		
			"goodsDetails"=>$items,
			"taxDetails"=>$efris->taxDetailsVAT18($items, $currency),
			"summary"=>$efris->summary($items, $remarks, $code),
			"payWay"=>$efris->payWay(1, $items),
			"importServicesSeller"=>$efris->importServicesSeller($name, $email, $tel, $address, date('Y-m-d ', $date)),
		);		
	}else{		
		$x = array(
			"sellerDetails"=>$efris->sellers_info($ref),
			"basicInformation"=>$efris->basic_information($ref, date('Y-m-d ', $date).date('H:i:s'), $currency, $t->full_name(user_id())),
			"buyerDetails"=>$efris->buyer_details($customer_type, $tin, $name ),		
			"goodsDetails"=>$items,
			"taxDetails"=>$efris->taxDetailsVAT18($items, $currency),
			"summary"=>$efris->summary($items, $remarks, $code),
			"payWay"=>$efris->payWay(1, $items),
		);		
	}
}else{
	if($vat == 'EXP'){
		
		$x = array(
			"sellerDetails"=>$efris->sellers_info($ref),
			"basicInformation"=>$efris->basic_informationZERO($ref, date('Y-m-d ', $date).date('H:i:s'), $currency, $t->full_name(user_id())),
			"buyerDetails"=>$efris->buyer_details($customer_type, $tin, $name ),		
			"goodsDetails"=>$items,
			"taxDetails"=>$efris->taxDetailsZERO($items, $currency),
			"summary"=>$efris->summary($items, $remarks, $code),
			"payWay"=>$efris->payWay(1, $items),
		);
	}else if($customer_type != 'EXP'){
		$x = array(
			"sellerDetails"=>$efris->sellers_info($ref),
			"basicInformation"=>$efris->basic_information($ref, date('Y-m-d ', $date).date('H:i:s'), $currency, $t->full_name(user_id())),
			"buyerDetails"=>$efris->buyer_details($customer_type, $tin, $name ),		
			"goodsDetails"=>$items,
			"taxDetails"=>$efris->taxDetails($items, $currency),
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
$v234 = '{
  "sellerDetails": {
		"tin": "1000025097",
		"legalName": "UGANDA ELECTRICITY TRANSMISSION COMPANY LIMITED",
		"businessName": "UGANDA ELECTRICITY TRANSMISSION COMPANY LIMITED",
		"mobilePhone": "256772705662",
		"linePhone": "2560778497936",
		"emailAddress": "nthakkar@ura.go.ug",
		"placeOfBusiness": "10 HANNINGTON ROAD UETCL KAMPALA CENTRAL DIVISION NAKASERO I",
		"referenceNo": "HEXGROUP DEBUG ERROR 02",
		"address": "10 HANNINGTON ROAD UETCL KAMPALA CENTRAL DIVISION NAKASERO I"
	},
   "basicInformation":{
      "deviceNo":"TCS5a2ce23154445074",
      "issuedDate":"2022-07-21 04:00:00",
      "operator":"joseph musiitwa",
      "currency":"UGX",
      "oriInvoiceId":"",
      "invoiceType":"1",
      "invoiceKind":"1",
      "dataSource":"103",
      "invoiceIndustryCode":"102"
   },
    "buyerDetails": {
        "buyerTin": "1000025760",
        "buyerLegalName": "UMEME LIMITED",
        "buyerBusinessName": "UMEME LIMITED",
        "buyerEmail": "josemusiitwa@gmail.com",
        "buyerType": "0",
        "buyerSector": "1",
        "invoiceIndustryCode": "102"
    },
    "goodsDetails": [
        {
            "item": "ENERGY BILL",
            "itemCode": "POWER",
            "qty": "12.57000",
            "unitOfMeasure": "PCE",
            "unitPrice": "401.20000",
            "total": "5043.08",
            "taxRate": 0.18,
            "tax": "769.28",
            "orderNumber": 0,
            "discountFlag": "2",
            "deemedFlag": "2",
            "exciseFlag": "2",
            "goodsCategoryId": "83101804"
        },
        {
            "item": "Power distribution service charge",
            "itemCode": "6005",
            "qty": "1.00000",
            "unitOfMeasure": "PCE",
            "unitPrice": "1000.00000",
            "total": "1000.00",
            "taxRate": 0.18,
            "tax": "152.5423728813559",
            "orderNumber": 1,
            "discountFlag": "2",
            "deemedFlag": "2",
            "exciseFlag": "2",
            "goodsCategoryId": "83101804"
        }
    ],
    "taxDetails": [
        {
            "taxCategoryCode": "01",
            "taxCategory": "Standard",
            "taxRateName": "Standard",
            "taxRate": "0.18",
            "grossAmount": "6043.08",
            "taxAmount": "921.82",
            "netAmount": "5121.26",
            "exciseUnit": "101",
            "exciseCurrency": "UGX"
        }
    ],
    "summary": {
        "netAmount": "5121.26",
        "taxAmount": "921.82",
        "grossAmount": "6043.08",
        "itemCount": 2,
        "modeCode": "0",
        "remarks": "TEST",
        "qrCode": "Amount: 6043.08, "
    },
    "payWay": [
        {
            "paymentMode": "101",
            "paymentAmount": "6043.08",
            "orderNumber": 1
        }
    ]
}';

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

	$afc = $c->basicInformation->antifakeCode;
	$inv_no = $c->basicInformation->invoiceNo;
	$qr = $c->summary->qrCode;
	$id = $c->basicInformation->invoiceId;
	
	$tax_rate = $c->taxDetails[0]->taxRate;
	$tax_rate_name = $c->taxDetails[0]->taxRateName;
	$currency = $c->taxDetails[0]->exciseCurrency;

	$gross = $c->taxDetails[0]->grossAmount;
	$net = $c->taxDetails[0]->netAmount;
	$vat_amount = $c->taxDetails[0]->taxAmount;
	
	$qr = $efris->generateQrCode($qr, $inv_no);
	$post_date = $t->rgf("other_invoice",$id_collector,"oi_id","oi_date_added");
	$db = new Db();

	{
		$db->update("other_invoice",["oi_fdn"=>$inv_no, "oi_vc"=>$afc, "oi_qr"=>$qr, "oi_invoice_id"=>$id,
			    "oi_peak" =>0,
				"oi_shoulder" =>0,
				"oi_off_peak" =>0,
				"oi_payable" =>0,
				"oi_collectable" => 0,
				"oi_gross" => $gross,
				"oi_vat_amount" => $vat_amount,
				"oi_net" => $net,
				"oi_post_date"=>$post_date,
				"oi_invoice_date"=>$post_date,
				"oi_status" => 1,
				"oi_counter" => 1
		 ],["oi_id"=>$id_collector]);
	}
	
	
	// $db->select("select * from efris_invoice");
	// $num = $db->num_rows();
}

$response .= '<br/><br/>'.$data;

$json = array();
$json['status'] = ((int)$error != 0)? "error" : "success";
$json['error'] = '<b style="color:red;font-weight:bold;">Error: '.$error.' '.$error_message.'</b><br/> '.$vfg.'<br/>'.$response;
$json['response'] = '<br/>'.$vfg.'<br/>'.$response;
$json['reference'] = "Reference: <b>".$reference."</b>";
$json['total'] = "Pushed: <b>".count($goods)."</b>";
$json['allpushed'] = $allchecked;


echo json_encode($json);

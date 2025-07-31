<?php 
ini_set('display_errors', 0);
include "../classes/Db.php";
include "../classes/BeforeAndAfter.inc";
include "../classes/AuditTrail.inc";
include "../classes/AccessRights.inc";
include "../classes/Efris.inc";
include ("../qrcode/qrlib.php");

$t = new BeforeAndAfter();
$efris = new Efris();
function user_id(){
	$x = @$_SESSION['UEDCL_USER_ID'];
	return $x;
	return 4;
}
$item = $_POST['items'];
$date = strtotime($_POST['date']);
$buyer = $_POST['buyer'];
$id_collector = $_POST['id'];
$invoice_no = $_POST['id'];
$currencyx = $_POST['currency'];
$ref = $_POST['id'];
$exchange = $_POST['exchange'];

$unitPrice = str_replace(",","",$_POST['unitPrice']);
$qty = $_POST['qty'];

if(strtoupper($currencyx) =='UGX'){
	$currency =101;
}else{
	$currency =102;
}


$comment = $_POST['comment'];
//$options = $_POST['options'];
//$cid = $options

$part = $_POST['part'];
$parc_id = $_POST['parc_id'];
$tc = $part;
$customer = $_POST['customer'];
$gi = $_POST['gi'];

$a_amount = str_replace(",", "",$_POST['a_amount']);
$amount = round($a_amount);
$total_vat = $_POST['total_vat'];
$total_vat_ex = $_POST['total_vat_ex'];
//echo ">>>>>".$gi;

$month = $_POST['month'];
$year = $_POST['year'];
//ini_set('display_errors', 1);

		



$vat = $_POST['vat'];
if($vat == "0.18") $vat = '18';

error_reporting(NULL);
session_start();

//ini_set('display_errors',1);




//echo ">>>>>>>>>>>>>>>>>".$item;
$db = new Db();
//$items = $t->rgf("efris_goods",$item,"eg_id","eg_name");
$name = $t->rgf("of_customer",$buyer,"ofc_id","ofc_customer_name");
$address = 'KAMPALA';
if(empty($customer_type)) $customer_type = "B2B";
$tin = $t->rgf("of_customer",$buyer,"ofc_id","ofc_tin_no");

$customer_type = $t->rgf("of_customer",$buyer,"ofc_id","ofc_cust_type");
$Coi_date = time();
$Coi_id = $id_collector;
$cusomer_short_name = $t->rgf("of_customer",$buyer,"ofc_id","ofc_short_name");
//echo '>>>>>>>>>>>>>>>>>>>>'.$cusomer_short_name;
$items = array();
$v=0;

for($i=1; $i<=1; $i++){
	
	//$item = $_POST['items'.$i];
	//$qty = str_replace(',','',$_POST['qty'.$i]);
	//$unitPrice = str_replace(',','',$_POST['unitPrice'.$i]);
	
	$estateName = "";
	
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

if(strtoupper($currencyx) =='UGX'){
	$currency =101;
}else{
	$currency =102;
}

$currency_oi = $currency;

$code ="";
//echo ">>>>>".$currency;
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
	}else if($vat != 'EXP'){
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
	
	$db = new Db();
	$rel = time().user_id();
	$time =time();
	$post_date = $t->rgf("other_invoice",$invoice_no,"oi_ref_no","oi_date_added");

	{

		$currency = $currency_oi;
			
			$db = new Db();
			$db->insert("other_invoice",["oi_customer"=>$buyer,"oi_customer_id"=>$buyer,"oi_pay_template"=>1,"oi_currency"=>$currency,"oi_added_by"=>user_id(),"oi_date_added"=>$time,"oi_date"=>time(),"oi_rel"=>$rel,"oi_ref_no"=>$invoice_no, "oi_vat"=>$vat,"oi_fdn"=>$inv_no, "oi_vc"=>$afc, "oi_qr"=>$qr, "oi_invoice_id"=>$id,"oi_source"=>'FIBRE',"oi_exchange_rate"=>$exchange,"oi_invoice_date"=>$date,

				"oi_peak" =>0,
				"oi_shoulder" =>0,
				"oi_off_peak" =>0,
				"oi_payable" =>0,
				"oi_collectable" => 0,
				"oi_gross" => $gross,
				"oi_vat_amount" => $vat_amount,
				"oi_net" => $net,
				"oi_post_date"=>$date,
				"oi_status" => 1,
				"oi_counter" => 1
		]);

			  $db->insert("invoice_item_description",["ii_amount"=>$unitPrice,"ii_quantity"=>$qty,"ii_date_added"=>time(), "ii_added_by"=>user_id(), "ii_rel"=>$rel,"ii_description"=>$item]); 


			  $com = array();
	          $com["arc_year"] = $year;
			  $com["arc_month"] = $month;
			  $com["arc_description"] = $comment;
			  $com["arc_date_added"] = time();
			  $com["arc_added_by"] = user_id();
			  $com["arc_customer"] = $customer;
			  $com["arc_part"] = $part;
		      $com["arc_type"] = "FIBRE";
			  $d = new Db();
			//$insert = $d->insert("all_readings_comments", $com);
			//echo $d->error();

			$ind = array();
			$ind["not_app3"] = 1;
			$ind['not_date_added'] = time();
			$ind['not_added_by'] = user_id();
			$ind['not_selected1'] = $t->rgf("sysuser", user_id(), "user_id", "user_designation");
			$d = new Db();
			//$insert = $d->update("notification", $ind, ["not_id"=>$gi]);

			$year = date('Y');
			$month = date('m');
			$db = new Db();
			$db->insert("invoice_track",["it_year"=>$year,"it_month"=>$month,"it_total_amount"=>$amount,"it_total_vat"=>$total_vat, "it_total_vat_ex_tax"=>$total_vat_ex,"it_invoice_id"=>$invoice_no,"it_type"=>"FIBRE","it_currency"=>$currency,"it_customer"=>$customer]);
			 //echo 'year '.$year.'month '.$month.'amount '.$amount.'total_vat '.$total_vat.'total_vat_ex '.$total_vat_ex.'invoice_no '.$invoice_no.'customer '.$customer;

			echo $db->error();
				

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

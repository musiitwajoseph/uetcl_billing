<?php 
error_reporting(NULL);
session_start();

$ori_fdn = $_POST['ori_fdn'];
$ecn_reference = $_POST['cn_id'];

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

if($ecn_reference != ""){ 
		//T111
		$xq = array(
			"id"=>$ecn_reference,
			//"referenceNo"=>$ecn_reference,
			//"queryType"=>3,
			//""
			//"pageNo"=>1,
			//"pageSize"=>1,
		);		
		
		$v = json_encode($xq);
		echo $vv = base64_encode($v);

		echo '<br/><br/>';
		
		$data_to_send = array(
			"interface"=>"T112",
			"request"=>$vv,
			"request_time"=>date('Y-m-d H:i:s'),
		);
		//$data = str_replace('\/', '/', $efris->data_to_send($data_to_send));
		$data = str_replace('"@@#####@@"', '{}', $efris->data_to_send($data_to_send));
					
		$t112 = $response = $efris->send_and_receive($data, 'http://127.0.0.1:9880/efristcs/ws/tcsapp/getInformation');
	
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
			$no = $c->refundInvoiceNo;
			$status = $c->approveStatusCode;
		}
		
		if(empty($no)){
			echo '<b>Not yet approved<br/>';
		}
		
		$insert = $db->update("cn_other_invoice", [
			"oi_status"=>$status, 
			"oi_cn_fdn"=>$no,
		],["oi_efris_cn_id"=>$ecn_reference]);
	}
	


if($status == 101 && !empty($no)){
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
	
		$t108 = $response = $efris->send_and_receive($data, 'http://127.0.0.1:9880/efristcs/ws/tcsapp/getInformation');
		
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

			$qr = $efris->generateQrCode($qrcode, $no);
		
			$insert = $db->update("cn_other_invoice", [
				"oi_cn_qr_code"=>$qr, 
				"oi_cn_vc"=>$vcode,
			],["oi_efris_cn_id"=>$ecn_reference]);



			if(!empty($no)){
				echo "<b>Credit FDN</b>: $inv_no <br/>";
				echo "<b>Credit Verification Code</b>: $vcode <br/>";
				echo "<b>Credit QR code</b>: $qr <br/>";
				echo '<img style="align:right;" src="../../efris_qr_images/'.$qr.'" alt="'.$qr.'"/>';
			}
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


//echo json_encode($json);

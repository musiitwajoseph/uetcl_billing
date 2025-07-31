<?php 
set_time_limit(0);
//include "classes/Db.php";
date_default_timezone_set("Africa/Kampala");
error_reporting(1);
ini_set('display_errors',1);
Class Efris extends Db{
	
	public $latitude = "0.4061957";
	public $longitude = "32.643798";
	public $device_number = "TCS5a2ce23154445074";
	
	public $device_MAC = "TCS2a80082879377106";
	public $tin = "1000025097";
	public $businessName = 'UGANDA ELECTRICITY TRANSMISSION COMPANY LIMITED';

	public function __construct(){
		$access = new AccessRights();
		//$access->pageAccess(user_id(), $this->page, 'V');
	}
	
	public function getLinks(){
		$page = "TEA";
		$links = array(
			array(
				"link_name"=>"Add Tea", 
				"link_address"=>"tea/add-tea",
				"link_icon"=>"fa-plus",
				"link_page"=>$page,
				"link_right"=>"A",
			),
		);
	}

	public function tax_cat($cat){
		$cat = strtoupper($cat);
		if($cat == 'V' || $cat == '18')
			return 18;
		else if($cat == 'Z' || $cat == '0')
			return 0;
		else if($cat == 'E' || $cat == '-')
			return 0;
		else return (int)$cat;
	}

	public function data_to_send($data=array()){
				
		return json_encode(array(
			"data"=> array(
				"content"=> $data['request'], 
				"signature"=> "",
				"dataDescription"=> array(
					"codeType"=> "0",
					"encryptCode"=> "0",
					"zipCode"=> "0"
				)
			),
			
			"globalInfo"=> array(
				"appId"=> "AP04",
				"brn"=> "",
				"dataExchangeId"=> "9230489223014123",
				"deviceMAC"=> '123',
				"deviceNo"=> $this->device_number,
				"extendField"=> "@@#####@@",
				"longitude"=> $this->longitude,
				"latitude"=> $this->latitude,
				"interfaceCode"=> $data['interface'],
				"requestCode"=> "TP",
				"requestTime"=> $data['request_time'],
				"responseCode"=> "TA",
				"taxpayerID"=> "723542954718704352",				
				"userName"=> "admin",
				"tin"=> $this->tin,
				"version"=> "1.1.20191201"
			),
			"returnStateInfo"=> array(
				"returnCode"=> "",
				"returnMessage"=> ""
			)
		));
	}

	public function send_and_receive($data_to_send, $url){
		
		$ch = curl_init($url);
		
		// Attach encoded JSON string to the POST fields
curl_setopt($ch, CURLOPT_POSTFIELDS, $data_to_send);

// Set the content type to application/json
curl_setopt($ch, CURLOPT_HTTPHEADER, array('Content-Type:application/json'));

// Return response instead of outputting
curl_setopt($ch, CURLOPT_RETURNTRANSFER, true);

// Execute the POST request
 return $result = curl_exec($ch);

// Close cURL resource
curl_close($ch);


	}

	public function invoice_details($invoice){
		return array("invoiceNo"=>$invoice);
	}
	public function nf($number, $dp){
		//$dp = 5;
		return str_replace(',','',number_format($number, $dp));
	}
	public function creditNote($original_id, $original_no, $reason, $contact_name, $contact_number, $contact_email, $remarks, $invoice_no, $currency){
		
		$creditNote = array(
			"oriInvoiceId"=> $original_id,
			"oriInvoiceNo"=> $original_no,
			"reasonCode"=> "105",
			"reason"=> $reason,
			"applicationTime"=> date('Y-m-d H:i:s'),
			"invoiceApplyCategoryCode"=> 101,
			"currency"=> $currency,
			"contactName"=> $contact_name,
			"contactMobileNum"=> $contact_no,
			"contactEmail"=> $contact_email,
			"source"=> "101",
			"remarks"=> $remarks,
			"sellersReferenceNo"=>'TEST'.$invoice_no,
			"%%%"=>"%%%"
		);
		$x = array();
		foreach($creditNote as $key=>$value){
			$x[$key] = $value;
		}
		//echo $x = implode(', ', $x);
		return $x;
	}

	public function extend2($code){
		$extend = array(
			"reason"=>"",
			"reasonCode"=>$code,
		);

		$x = array();
		foreach($extend as $key=>$value){
			$x[$key] = $value;
		}
		//echo $x = implode(', ', $x);
		return $x;
	}

	public function debitNote($original_id, $original_no, $reason, $contact_name, $contact_number, $contact_email, $remarks, $invoice_no, $currency){
		
		$creditNote = array(
			"oriInvoiceId"=> $original_id,
			"oriInvoiceNo"=> $original_no,
			"reasonCode"=> "105",
			"reason"=> $reason,
			"applicationTime"=> date('Y-m-d H:i:s'),
			"invoiceApplyCategoryCode"=> 101,
			"currency"=> $currency,
			"contactName"=> $contact_name,
			"contactMobileNum"=> $contact_no,
			"contactEmail"=> $contact_email,
			"source"=> "101",
			"remarks"=> $remarks,
			"sellersReferenceNo"=>'TEST'.$invoice_no,
			"%%%"=>"%%%"
		);
		$x = array();
		foreach($creditNote as $key=>$value){
			$x[$key] = $value;
		}
		//echo $x = implode(', ', $x);
		return $x;
	}


	public function sellers_info2($invoice_no=0){

		$select = $this->select("SELECT * FROM kis_efris_info");
		extract($select[0]);
	
		$seller_info = array(
			"tin"=> $this->tin,
			"ninBrn"=> '26729B',
			"legalName"=> 'UGANDA ELECTRICITY TRANSMISSION COMPANY LIMITED',
			"businessName"=> $this->businessName,
			"address"=>"10 HANNINGTON ROAD UETCL KAMPALA CENTRAL DIVISION NAKASERO I",
			"mobilePhone"=> '256772705662',
			"linePhone"=> '2560778497936',
			"emailAddress"=> 'nthakkar@ura.go.ug',
			"placeOfBusiness"=> '10 HANNINGTON ROAD UETCL KAMPALA CENTRAL DIVISION NAKASERO I',
			//"referenceNo"=> $invoice_no,
		);

		return $seller_info;
	}


	public function sellers_info($invoice_no=0){

		$select = $this->select("SELECT * FROM kis_efris_info");
		extract($select[0]);
	
		$seller_info = array(
			"tin"=> $this->tin,
			//"ninBrn"=> '26729B',
			"legalName"=> 'UGANDA ELECTRICITY TRANSMISSION COMPANY LIMITED',
			"businessName"=> $this->businessName,
			"mobilePhone"=> '256772705662',
			"linePhone"=> '2560778497936',
			"emailAddress"=> 'nthakkar@ura.go.ug',
			"placeOfBusiness"=> '10 HANNINGTON ROAD UETCL KAMPALA CENTRAL DIVISION NAKASERO I',
			"referenceNo"=> $invoice_no,
			"address"=>"10 HANNINGTON ROAD UETCL KAMPALA CENTRAL DIVISION NAKASERO I",
			
		);

		return $seller_info;
	}

	public function basic_informationZERO($invNumber, $issuedDate, $cur, $operators, $kind=1, $inv_type=1){
		$bi = array(
			"invoiceNo"=> $invNumber,
			"antifakeCode"=> $invNumber,
			"deviceNo"=> $this->device_number,
			"issuedDate"=> $issuedDate,
			"operator"=> $operators,
			"currency"=> $cur,
			//"oriInvoiceId"=> "1",
			"invoiceType"=> $inv_type,
			"invoiceKind"=> $kind,
			"dataSource"=> "103",//webService api 
			"invoiceIndustryCode"=> "112",
			//"isBatch"=> "0",//Gsdflsdf 
		);
		return $bi;
	}

	public function basic_information($invNumber, $issuedDate, $cur, $operators, $kind=1, $inv_type=1, $invoiceIndustryCode=101, $dataSource=103){
		$bi = array(
			"invoiceNo"=> "",
			"antifakeCode"=> "",
			"deviceNo"=> $this->device_number,
			"issuedDate"=> $issuedDate,
			"operator"=> $operators,
			"currency"=> $cur,
			//"oriInvoiceId"=> "1",
			"invoiceType"=> $inv_type,
			"invoiceKind"=> $kind,
			"dataSource"=> $dataSource,//webService api 
			"invoiceIndustryCode"=> $invoiceIndustryCode
			//"isBatch"=> "0",//Gsdflsdf 
		);
		return $bi;
	}

	public function basic_information2($invNumber, $issuedDate, $cur, $operators, $kind=1, $inv_type=1, $oriInvoiceId){
		$bi = array(
			"invoiceNo"=> "",
			"antifakeCode"=> "",
			"deviceNo"=> $this->device_number,
			"issuedDate"=> $issuedDate,
			"operator"=> $operators,
			"currency"=> $cur,
			"oriInvoiceId"=> $oriInvoiceId,
			"invoiceType"=> $inv_type,
			"invoiceKind"=> $kind,
			"dataSource"=> "103",//webService api 
			//"invoiceIndustryCode"=> "102",
			//"isBatch"=> "0",//Gsdflsdf 
		);
		return $bi;
	}

	public function buyer_details($type, $tin, $name, $invoiceIndustryCode = 102){
		
		if(strlen($tin) < 10) $tin = "";
		if($type == "B2C"||$type==""){
			return array(
					"buyerTin"=> $tin,
					//"buyerNinBrn"=> "201905081705",
					//"buyerPassportNum"=> "201905081705",
					"buyerLegalName"=> $name,
					"buyerBusinessName"=> $name,
					//"buyerAddress"=> "beijin",
					"buyerEmail"=> "josemusiitwa@gmail.com",
					//"buyerMobilePhone"=> "15501234567",
					//"buyerLinePhone"=> "010-6689666",
					//"buyerPlaceOfBusi"=> "beijin",
					"buyerType"=> "1",//B2B ==> 1 AND B2C ==> 2
					//"buyerCitizenship"=> "UGANDAN",
					"buyerSector"=> "1",
					//"buyerReferenceNo"=> "00000000001",					
					//"invoiceIndustryCode"=>$invoiceIndustryCode
				);
		}else if($type == "B2F"){
			return array(
					"buyerTin"=> $tin,
					//"buyerNinBrn"=> "201905081705",
					//"buyerPassportNum"=> "201905081705",
					"buyerLegalName"=> $name,
					"buyerBusinessName"=> $name,
					//"buyerAddress"=> "beijin",
					"buyerEmail"=> "josemusiitwa@gmail.com",
					//"buyerMobilePhone"=> "15501234567",
					//"buyerLinePhone"=> "010-6689666",
					//"buyerPlaceOfBusi"=> "beijin",
					"buyerType"=> "2",//B2B ==> 1 AND B2C ==> 2
					//"buyerCitizenship"=> "UGANDAN",
					"buyerSector"=> "1",
					//"buyerReferenceNo"=> "00000000001"		
					//"invoiceIndustryCode"=>$invoiceIndustryCode
				);
		}else{
			return array(
					"buyerTin"=> $tin,
					//"buyerNinBrn"=> "201905081705",
					//"buyerPassportNum"=> "201905081705",
					"buyerLegalName"=> $name,
					"buyerBusinessName"=> $name,
					//"buyerAddress"=> "beijin",
					"buyerEmail"=> "josemusiitwa@gmail.com",
					//"buyerMobilePhone"=> "15501234567",
					//"buyerLinePhone"=> "010-6689666",
					//"buyerPlaceOfBusi"=> "beijin",
					"buyerType"=> "0",//B2B ==> 1 AND B2C ==> 2
					//"buyerCitizenship"=> "UGANDAN",
					"buyerSector"=> "1",
					//"buyerReferenceNo"=> "00000000001"		
					//"invoiceIndustryCode"=>$invoiceIndustryCode,
				);
		}

	}
	
	public function importServicesSeller($name,$email, $tel, $address, $date){
		$importServicesSeller = array(
			"importBusinessName"=>$name,
			"importEmailAddress"=>$email,
			"importContactNumber"=>$tel,
			"importAddress"=>$address,
			"importInvoiceDate"=>$date,
		);

		return $importServicesSeller;
	}

	public function item($item_code, $item_name, $qty, $amount, $item_id, $uom, $orderNumber, $tax=0){
		if($tax == 18){
			$taxx = 18/118;
			$taxRate = 0.18;
		}else if($tax == 'zero'){
			$taxx = 0;
			$taxRate = 0;
		}else{
			$taxx = '0';
			$taxRate = '-';
		}
			
		$amount = $amount ;//+ $amount*$tax/100;
		$amount = $this->nf($amount,5);
				
		$i = array(
			"item"=> $item_name,
			"itemCode"=> $item_code,
			"qty"=> $this->nf($qty,5),
			"unitOfMeasure"=> $uom,
			"unitPrice"=> $this->nf($amount,5),
			"total"=> $this->nf(($qty*$amount),2),
			"taxRate"=> $taxRate,
			"tax"=> $this->nf(($qty*$amount)*$taxx,2),
			//"discountTotal"=> "18.00",
			//"discountTaxRate"=> "0.18",
			"orderNumber"=> $orderNumber,
			"discountFlag"=> "2",
			"deemedFlag"=> "2",
			"exciseFlag"=> "2",
			//"categoryId"=> "1234",
			// "categoryName"=> "Test",
			"goodsCategoryId"=> $item_id,
			// "goodsCategoryName"=> "Test",
			// "exciseRate"=> "0.12",
			// "exciseRule"=> "1",
			// "exciseTax"=> "20.22",
			// "pack"=> "1",
			// "stick"=> "20",
			// "exciseUnit"=> "101",
			// "exciseCurrency"=> "UGX",
			// "exciseRateName"=> "123"
		);
		return $i;
	}

	public function receipt($item_code, $item_name, $amount, $item_id, $uom, $orderNumber){
		
		return array(
			"item"=> $item_name,
			"itemCode"=> $item_code,
			//"qty"=> $this->nf($qty,2),
			//"unitOfMeasure"=> $uom,
			//"unitPrice"=> $this->nf($amount,2),
			"total"=> $this->nf($amount,2),
			"taxRate"=> "0",
			"tax"=> 0,//$this->nf($qty*$amount*18/118,2),
			//"discountTotal"=> "18.00",
			//"discountTaxRate"=> "0.18",
			"orderNumber"=> $orderNumber,
			"discountFlag"=> "2",
			"deemedFlag"=> "2",
			"exciseFlag"=> "2",
			//"categoryId"=> "1234",
			// "categoryName"=> "Test",
			"goodsCategoryId"=> $item_id,
			// "goodsCategoryName"=> "Test",
			// "exciseRate"=> "0.12",
			// "exciseRule"=> "1",
			// "exciseTax"=> "20.22",
			// "pack"=> "1",
			// "stick"=> "20",
			// "exciseUnit"=> "101",
			// "exciseCurrency"=> "UGX",
			// "exciseRateName"=> "123"
		);
	}

	public function goodsUp(){
		return $ok = array(
			"operationType"=>$opertionType,
			"goodsName"=>$goodsName,
			"goodsCode"=>$goodsCode,
			"measureUnit"=>$measureUnit,
			"unitPrice"=>$unitPrice,
			"currency"=>$currency,
			"commodityCategoryId"=>$commodityCategoryId,
			"haveExciseTax"=>$haveExciseTax,
			"description"=>$description,
			"stockPrewarning"=>$stockPrewarning,
			"pieceMeasureUnit"=>$pieceMeasureUnit,
			"havePieceUnit"=>$havePieceUnit,
			"pieceUnitPrice"=>$pieceUnitPrice,
			"packageScaledValue"=>$packageScaledValue,
			"pieceScaledValue"=>$pieceScaledValue,
			"exciseDutyCode"=>$exciseDutyCode,
			"havePieceUnit"=>$havePieceUnit,
		);
	}
	
	public function goodsStockIn($date_in){
		
		$stock = array(
				"operationType"=>"101",
				"supplierTin"=>"",//$this->tin,
				"supplierName"=>"",//$this->businessName,
				"adjustType"=>"",//"104",
				"remarks"=>"Produced Tea",
				"stockInDate"=>$date_in,
				"stockInType"=>"103",
				"productionBatchNo"=>rand(1000,9999),
				"productionDate"=>$date_in,
				//"branchId"=>rand(1000,9999),
				//"invoiceNo"=>rand(1000,9999),
				//"isCheckBatchNo"=>"",
			);
		return $stock;

	}

	public function goodsStockOut($adjustType, $remarks){
		
		$stock = array(
				"operationType"=>"102",
				"supplierTin"=>"",//$this->tin,
				"supplierName"=>"",//$this->businessName,
				"adjustType"=>$adjustType,//"104",
				"remarks"=>$remarks,
				"stockInDate"=>$date_in,
				"stockInType"=>"",
				//"productionBatchNo"=>rand(1000,9999),
				"productionDate"=>"",
				//"branchId"=>rand(1000,9999),
				//"invoiceNo"=>rand(1000,9999),
				//"isCheckBatchNo"=>"",
			);
		return $stock;

	}

	public function good($goodname, $qty, $price){
	
		//50201710
		$goodname = strtolower($goodname);
		$x = array(
				"ankole"=>array(
					"400791828517546688","MRUL-AN","KGM","BLACK TEA ANKOLE","1","50201710",""
				),
				"kisaru"=>array(
					"400743625440462536","MRUL-KS","KGM","BLACK TEA KISARU","1","50201710"
				),
				"mwenge"=>array(
					"400743625440462554","MRUL-MW","KGM","BLACK TEA MWENGE","1","50201710"
				),
				"muzizi"=>array(
					"400743625440462580","MRUL-MU","KGM","BLACK TEA MUZIZI","1","50201710"
				),
				"kiko"=>array(
					"400791828517546643","MRUL-KI","KGM","BLACK TEA KIKO","1","50201710"
				),
				"bugambe"=>array(
					"400791828517546667","MRUL-BU","KGM","BLACK TEA BUGAMBE","1","50201710"
				),
				"license fee"=>array(
					"400634917540335246","MRUL-BTHL","115","LICENSE FEE","0", "80101513"
				),
				"all scrap"=>array(
					"400634917540335227","MRUL-SCRAP","103","ALL SCRAP","1","11191606"
				),
				"estate scrap"=>array(
					"400581451257340179","MRUL-SCRAP-ES","PC","Estate Scrap","1","11191613"
				),
				"poles"=>array(
					"400581451257340156","MRUL-POLES","PCE","POLES","1","10161516"
				),
				"pine logs"=>array(
					"400581451257340138","MRUL-PINE","PCE","PINE LOGS","1", "10161511"
				),
				"all vehicles"=>array(
					"400581451257340116","MRUL-USED VEHICLES","PCE","ALL VEHICLES","1","21101601"
				),
				"professional fees"=>array(
					"400476284912601557","MRUL PR FEES","115","PROFESSIONAL FEES","0", "80101508"
				),	
			);

		$stock=array(
			"commodityGoodsId"=>$x[$goodname][0],
			"goodsCode"=>$x[$goodname][1],
			"measureUnit"=>$x[$goodname][2],
			"quantity"=>$qty,
			"unitPrice"=>$price,
		);
		return $stock;
	}


	public function stockGoods(){
	
		//50201710
		$goodname = strtolower($goodname);
		$x = array(
				"ankole"=>array(
					"400791828517546688","MRUL-AN","KGM","BLACK TEA ANKOLE","1","50201710",""
				),
				"kisaru"=>array(
					"400743625440462536","MRUL-KS","KGM","BLACK TEA KISARU","1","50201710"
				),
				"mwenge"=>array(
					"400743625440462554","MRUL-MW","KGM","BLACK TEA MWENGE","1","50201710"
				),
				"muzizi"=>array(
					"400743625440462580","MRUL-MU","KGM","BLACK TEA MUZIZI","1","50201710"
				),
				"kiko"=>array(
					"400791828517546643","MRUL-KI","KGM","BLACK TEA KIKO","1","50201710"
				),
				"bugambe"=>array(
					"400791828517546667","MRUL-BU","KGM","BLACK TEA BUGAMBE","1","50201710"
				),
				"license fee"=>array(
					"400634917540335246","MRUL-BTHL","115","LICENSE FEE","0", "80101513"
				),
				"all scrap"=>array(
					"400634917540335227","MRUL-SCRAP","103","ALL SCRAP","1","11191606"
				),
				"estate scrap"=>array(
					"400581451257340179","MRUL-SCRAP-ES","PC","Estate Scrap","1","11191613"
				),
				"poles"=>array(
					"400581451257340156","MRUL-POLES","PCE","POLES","1","10161516"
				),
				"pine logs"=>array(
					"400581451257340138","MRUL-PINE","PCE","PINE LOGS","1", "10161511"
				),
				"all vehicles"=>array(
					"400581451257340116","MRUL-USED VEHICLES","PCE","ALL VEHICLES","1","21101601"
				),
				"professional fees"=>array(
					"400476284912601557","MRUL PR FEES","115","PROFESSIONAL FEES","0", "80101508"
				),	
			);

		return $x;
	}

	public function summary($items, $remarks, $code){
		$netAmount = $taxAmount = $grossAmount = 0;
		foreach($items as $item){
			$netAmount += $item['total'];
			$taxAmount += $item['tax'];
		}

		return array(
			"netAmount"=> $this->nf($netAmount-$taxAmount, 2),
			"taxAmount"=> $this->nf($taxAmount, 2),
			"grossAmount"=> $this->nf($netAmount, 2),
			"itemCount"=> count($items),
			"modeCode"=> "0",
			"remarks"=> $remarks,
			"qrCode"=> "Amount: ".$this->nf($netAmount, 2).", ".$code,
		);
	}
	
	public function taxDetails2($items, $tax=18, $tax_code,$tax_category, $tax_name, $excise_unit, $excise_currency){
		//SECTION - E
		$details = array();
		$ds = array();
		
		if($tax==18){
			$taxx = 18/118;
			$t = 0.18;
		}else{
			$taxx = 1;
			$t = 0;
		}
		foreach($items as $item){
			
			$netAmount = $item['unitPrice']*$item['qty'];
			$taxAmount = $this->nf($item['unitPrice']*$item['qty']*$taxx, 2);
			
				
			$dsgrossAmount += $this->nf($netAmount, 2);				
			$dstaxAmount += $this->nf($taxAmount, 2);				
			$dsnetAmount += $this->nf($netAmount-$taxAmount, 2);
			
		}
		//echo $dsgrossAmount.'@@'.$dstaxAmount.'@@'.$dsnetAmount.'<br/>';
		$details[] = array(	
			"taxCategoryCode"=>$tax_code,
			"taxCategory"=> $tax_category,
			"taxRateName"=> $tax_name,
			"taxRate"=>$t,
			"grossAmount"=> $this->nf($dsgrossAmount,2),//$this->nf($netAmount, 2),				
			"taxAmount"=> $this->nf($dstaxAmount,2),//$this->nf($taxAmount, 2),				
			"netAmount"=> $this->nf($dsnetAmount,2),//$this->nf($netAmount-$taxAmount, 2),
			"exciseUnit"=>$excise_unit,
			"exciseCurrency"=>$excise_currency,
		);
		
		return $details;
	}
	
	public function taxDetailsVAT18($items, $currency){
		//SECTION - E
		$details = array();
		$ds = array();
		foreach($items as $item){
			
			$netAmount = $item['unitPrice']*$item['qty'];
			$taxAmount = $this->nf($item['unitPrice']*$item['qty']*18/118, 2);
			
				
			$dsgrossAmount += $this->nf($netAmount, 2);				
			$dstaxAmount += $this->nf($taxAmount, 2);				
			$dsnetAmount += $this->nf($netAmount-$taxAmount, 2);
			
		}
		//echo $dsgrossAmount.'@@'.$dstaxAmount.'@@'.$dsnetAmount.'<br/>';
		$details[] = array(	
			"taxCategoryCode"=>"01",
			"taxCategory"=> "'Standard",
			"taxRateName"=> "Standard",
			"taxRate"=>"0.18",
			"grossAmount"=> $this->nf($dsgrossAmount,2),//$this->nf($netAmount, 2),				
			"taxAmount"=> $this->nf($dstaxAmount,2),//$this->nf($taxAmount, 2),				
			"netAmount"=> $this->nf($dsnetAmount,2),//$this->nf($netAmount-$taxAmount, 2),
			"exciseUnit"=>"101",
			"exciseCurrency"=>$currency,
		);
		
		return $details;
	}
	
	public function taxDetails($items,$currency){
		//SECTION - E
		$details = array();
		$ds = array();
		foreach($items as $item){
			
			$netAmount = $item['unitPrice']*$item['qty'];
			$taxAmount = 0;//$this->nf($item['unitPrice']*$item['qty']*18/118, 2);
			
				
			$dsgrossAmount += $this->nf($netAmount, 2);				
			$dstaxAmount += $this->nf($taxAmount, 2);				
			$dsnetAmount += $this->nf($netAmount-$taxAmount, 2);
			
		}
		//echo $dsgrossAmount.'@@'.$dstaxAmount.'@@'.$dsnetAmount.'<br/>';
		$details[] = array(	
			"taxCategoryCode"=>"03",
			"taxCategory"=> "'Exempt",
			"taxRateName"=> "Exempt",
			"taxRate"=>"-",
			"grossAmount"=> $this->nf($dsgrossAmount,2),//$this->nf($netAmount, 2),				
			"taxAmount"=> $this->nf($dstaxAmount,2),//$this->nf($taxAmount, 2),				
			"netAmount"=> $this->nf($dsnetAmount,2),//$this->nf($netAmount-$taxAmount, 2),
			//"exciseUnit"=>"101",
			"exciseCurrency"=>$currency,
		);
		
		return $details;
	}
	
	public function taxDetailsZERO($items,$currency){
		//SECTION - E
		$details = array();
		$ds = array();
		foreach($items as $item){
			
			$netAmount = $item['unitPrice']*$item['qty'];
			$taxAmount = 0;//$this->nf($item['unitPrice']*$item['qty']*18/118, 2);
			
				
			$dsgrossAmount += $this->nf($netAmount, 2);				
			$dstaxAmount += $this->nf($taxAmount, 2);				
			$dsnetAmount += $this->nf($netAmount-$taxAmount, 2);
			
		}
		//echo $dsgrossAmount.'@@'.$dstaxAmount.'@@'.$dsnetAmount.'<br/>';
		$details[] = array(	
			"taxCategoryCode"=>"02",
			"taxCategory"=> "Zero Rated",
			"taxRateName"=> "Zero Rated",
			"taxRate"=>"0",
			"grossAmount"=> $this->nf($dsgrossAmount,2),//$this->nf($netAmount, 2),				
			"taxAmount"=> $this->nf($dstaxAmount,2),//$this->nf($taxAmount, 2),				
			"netAmount"=> $this->nf($dsnetAmount,2),//$this->nf($netAmount-$taxAmount, 2),
			//"exciseUnit"=>"101",
			"exciseCurrency"=>$currency,
		);
		
		return $details;
	}
	public function taxDetailsZERORATED($items,$currency){
		//SECTION - E
		$details = array();
		$ds = array();
		foreach($items as $item){
			
			$netAmount = $item['unitPrice']*$item['qty'];
			$taxAmount = 0;//$this->nf($item['unitPrice']*$item['qty']*18/118, 2);
			
				
			$dsgrossAmount += $this->nf($netAmount, 2);				
			$dstaxAmount += $this->nf($taxAmount, 2);				
			$dsnetAmount += $this->nf($netAmount-$taxAmount, 2);
			
		}
		//echo $dsgrossAmount.'@@'.$dstaxAmount.'@@'.$dsnetAmount.'<br/>';
		$details[] = array(	
			"taxCategoryCode"=>"02",
			"taxCategory"=> "Zero rated",
			"taxRateName"=> "Zero rated",
			"taxRate"=>"-",
			"grossAmount"=> $this->nf($dsgrossAmount,2),//$this->nf($netAmount, 2),				
			"taxAmount"=> $this->nf($dstaxAmount,2),//$this->nf($taxAmount, 2),				
			"netAmount"=> $this->nf($dsnetAmount,2),//$this->nf($netAmount-$taxAmount, 2),
			//"exciseUnit"=>"101",
			"exciseCurrency"=>$currency,
		);
		
		return $details;
	}

	public function payWay($orderNumber, $items=array()){
		$total_gross = 0;
		foreach($items as $item){
			$netAmount = $item['unitPrice']*$item['qty'];
			$total_gross += $this->nf($netAmount, 2);	
		}
		
		return array(
			array(
				"paymentMode"=> "101",
				"paymentAmount"=> $this->nf($total_gross,2),
				"orderNumber"=> $orderNumber,
			)			
		);
	}

	public function extend(){
		return array(
				"reason"=> "reason",
				"reasonCode"=> "102"
		);
	}
	
	public function saveReference($id="",$reference="", $fdn="", $no, $vcode, $qrcode, $status, $inv_no){
		$PNG_TEMP_DIR = dirname(__FILE__).DIRECTORY_SEPARATOR.'temp'.DIRECTORY_SEPARATOR;
    
	    //html PNG location prefix
	    $PNG_WEB_DIR = 'images/'; 
	    
	    //ofcourse we need rights to create temp dir
	    if (!file_exists($PNG_TEMP_DIR))
	        mkdir($PNG_TEMP_DIR);
	 	
		$errorCorrectionLevel = 'H'; 
	    $matrixPointSize = 4;
		$matrixPointSize = min(max(5, 1), 10);
		
		if($qrcode != ""){
			$filename = '../images/EFRIS_CN'.md5($qrcode.'|'.$errorCorrectionLevel.'|'.$matrixPointSize).'.png';
	       QRcode::png($qrcode, $filename, $errorCorrectionLevel, $matrixPointSize, 2);    
	       
	      $qrLink = basename($filename);
		}
		//echo '.....'.$qrCode;
	    if (!empty($reference)) {      

	       $this->update("cn", [
		   "cn_reference"=>$reference,
		   "cn_no"=>$no,
		   "cn_fdn"=>$fdn,
		   "cn_vcode"=>$vcode,
		   "cn_qrcode"=>$qrLink,
		   "cn_status"=>$status,
		   "cn_credit_no"=>$inv_no,
		   ],["cn_id"=>$id]);
		   
	    } else {  
	        echo "Reference not found. Not uploaded...";
	    } 

	    return $qrLink;
	}
	public function generateQrCode($text, $fdn){
		$PNG_TEMP_DIR = dirname(__FILE__).DIRECTORY_SEPARATOR.'temp'.DIRECTORY_SEPARATOR;
    
	    //html PNG location prefix
	    $PNG_WEB_DIR = 'efris_qr_images/'; 
	    
	    //ofcourse we need rights to create temp dir
	    if (!file_exists($PNG_TEMP_DIR))
	        mkdir($PNG_TEMP_DIR);
	 	
		$errorCorrectionLevel = 'H'; 
	    $matrixPointSize = 4;
		$matrixPointSize = min(max(5, 1), 10);

	    if (isset($text)) {     
	        //it's very important!
	        // user data
	        $filename = '../efris_qr_images/'.$fdn.'.png';
	       QRcode::png($text, $filename, $errorCorrectionLevel, $matrixPointSize, 2);    
	       
	      	$qrLink = $fdn.'.png'; //basename($filename); 
		   
	    } else {  
	        $qrLink = "";
	    } 

	    return $qrLink;
	}
	
	public function generateQrCodeReceipt($text, $customer_id, $dateadded, $afc="", $inv=""){
		$PNG_TEMP_DIR = dirname(__FILE__).DIRECTORY_SEPARATOR.'temp'.DIRECTORY_SEPARATOR;
    
	    //html PNG location prefix
	    $PNG_WEB_DIR = 'efris_qr_images/'; 
	    
	    //ofcourse we need rights to create temp dir
	    if (!file_exists($PNG_TEMP_DIR))
	        mkdir($PNG_TEMP_DIR);
	 	
		$errorCorrectionLevel = 'H'; 
	    $matrixPointSize = 4;
		$matrixPointSize = min(max(5, 1), 10);

	    if (isset($text)) {     
	        //it's very important!
	        // user data
	        $filename = 'efris_qr_images/EFRIS'.md5($text.'|'.$errorCorrectionLevel.'|'.$matrixPointSize).'.png';
	       QRcode::png($text, $filename, $errorCorrectionLevel, $matrixPointSize, 2);    
	       
	      	$qrLink = basename($filename); 
		   
	    } else {  
	        $qrLink = "";
	    } 

	    return $qrLink;
	}
	
	public function generateQrCodeAndShow($text){
		$PNG_TEMP_DIR = dirname(__FILE__).DIRECTORY_SEPARATOR.'temp'.DIRECTORY_SEPARATOR;
    
	    //html PNG location prefix
	    $PNG_WEB_DIR = 'images/'; 
	    
	    //ofcourse we need rights to create temp dir
	    if (!file_exists($PNG_TEMP_DIR))
	        mkdir($PNG_TEMP_DIR);
	 	
		$errorCorrectionLevel = 'H'; 
	    $matrixPointSize = 4;
		$matrixPointSize = min(max(5, 1), 10);

	    if (isset($text)) {     
	        //it's very important!
	        // user data
	        $filename = 'images/EFRIS'.md5($text.'|'.$errorCorrectionLevel.'|'.$matrixPointSize).'.png';
	       QRcode::png($text, $filename, $errorCorrectionLevel, $matrixPointSize, 2);    
	       
	      	$qrLink = basename($filename);
		   
	    } else {  
	        echo "Not uploaded...";
	    } 

	    return $qrLink;
	}
}



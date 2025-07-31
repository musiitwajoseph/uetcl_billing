<?php
	include "Db.php";
	error_reporting(null);
	
	$account_code_credit = '180000';
	$account_code_debit = 'rec_bank';
	$account_code_vat = 'rec_vat_status';
	
	$from = $_GET['p1'];
	$to = $_GET['p2'];
	
	if(1){
		$label = "RECEIPTS FROM ".date('Y-m-d h:i:sa', $from).' to '.date('Y-m-d h:i:sa', $to);
		
	}
	
	$filename = 'test';
	
	
	//header info for browser
	
	header("Content-Type: text/csv");
	header("Content-Disposition: attachment; filename=".$label.".csv");
	header("Pragma: no-cache"); 
	header("Expires: 0");
	$file_ending = "csv";
	
	$file = fopen('php://output', 'w');
	
	
	$headers = array();
	$headers[] = 'LI';
	$headers[] = 'Account';
	$headers[] = 'Receipt_Narration';
	$headers[] = 'Receipt_No.';	
	$headers[] = 'Receipt_Date';
	$headers[] = 'Period';
	$headers[] = 'Base_Amount';
	$headers[] = 'Forex_Amount';
	$headers[] = 'Debit_Credit';
	$headers[] = 'Currency';
	$headers[] = 'Section';
	$headers[] = 'Staff';
	$headers[] = 'Project';
	
	//echo implode("\t", $headers)."\n";
	fputcsv($file, $headers);
	
	$sep = "\t";
	
	//$db = new Db();
	//$select = $db->select("SELECT * FROM ");
	
	
	$db = new Db();
	$sql = "SELECT * FROM receipts, receipt_amount WHERE rec_ref = ra_ref AND rec_date >= '$from' AND rec_date <= '$to' ORDER BY rec_receipt_no ASC";
	$select = $db->select($sql);
	$no = 1;
	$total_amount = 0;
	$rowx = array();
	$i = 1;
	foreach($select[0] as $row){
		extract($row);
		$cell = array();
		
		if($i==1){
			$li =  '3;6';
			$i++;
		}else{
			$li =  '6';
		}
		
		$i++;
		//credit
		$d = new Db();
		$dd = $d->select("SELECT account_number as rec_account2 FROM account2 WHERE account_id = '$rec_account2'");
		extract($dd[0][0]);
		$cell[] = $li;
		$cell[] = $rec_account2;
		$cell[] = $ra_desc;
		$cell[] = $rec_receipt_no;
		$cell[] = date('dmY', $rec_date);
		$cell[] = period($rec_date);
		$cell[] = ($ra_amount);
		$cell[] = '';
		$cell[] = 'C';
		$cell[] = $rec_curr;
		$cell[] = $rec_section;
		$cell[] = $rec_staff;
		$cell[] = $rec_project;
		//$rowx[] = implode($sep, $cell);
		fputcsv($file, $cell);
		$li = '6';
		
		if($rec_vat_status == 2){
			//debit
			$cell = array();
			
			$cell[] = $li;
			$cell[] = $$account_code_debit;
			$cell[] = $ra_desc;
			$cell[] = $rec_receipt_no;
			$cell[] = date('dmY', $rec_date);
			$cell[] = period($rec_date);
			$cell[] = ($ra_amount);
			$cell[] = '';
			$cell[] = 'D';
			$cell[] = $rec_curr;
			$cell[] = $rec_section;
			$cell[] = $rec_staff;
			$cell[] = $rec_project;
			//$rowx[] = implode($sep, $cell);
			fputcsv($file, $cell);
		}else{		
		
			//debit
			$cell = array();
			$cell[] = $li;
			$cell[] = $$account_code_debit;
			$cell[] = $ra_desc;
			$cell[] = $rec_receipt_no;
			$cell[] = date('dmY', $rec_date);
			$cell[] = period($rec_date);
			$cell[] = ($ra_amount*(1-18/100));
			$cell[] = '';
			$cell[] = 'D';
			$cell[] = $rec_curr;
			$cell[] = $rec_section;
			$cell[] = $rec_staff;
			$cell[] = $rec_project;	
			//$rowx[] = implode($sep, $cell);
			fputcsv($file, $cell);
			//vat
			//$cell[] = "";
			$cell = array();
			$cell[] = $li;
			$cell[] = $$account_code_vat;
			$cell[] = $ra_desc;
			$cell[] = $rec_receipt_no;
			$cell[] = date('dmY', $rec_date);
			$cell[] = period($rec_date);
			$cell[] = ($ra_amount*18/100);
			$cell[] = '';
			$cell[] = 'D';
			$cell[] = $rec_curr;
			$cell[] = $rec_section;
			$cell[] = $rec_staff;
			$cell[] = $rec_project;
			//$rowx[] = implode($sep, $cell);
			fputcsv($file, $cell);
		}
		
		
	}
	
	echo implode("\n", $rowx);
	
	
	
	
	function period($date){

		$mo = array(
			"JAN"=>"007",
			"FEB"=>"008",
			"MAR"=>"009",
			"APR"=>"010",
			"MAY"=>"011",
			"JUN"=>"012",
			"JUL"=>"001",
			"AUG"=>"002",
			"SEP"=>"003",
			"OCT"=>"004",
			"NOV"=>"005",
			"DEC"=>"006", 
		);
		$m = $mo[strtoupper(date('M', $date))].''.date('Y', $date);
		return $m;
	}
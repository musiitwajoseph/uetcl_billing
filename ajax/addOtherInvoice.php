<?php
error_reporting(null);
include "../classes/init.inc";
$t = new BeforeAndAfter();
    //ini_set('display_errors', 1);

			$date = $_POST['date'];
			$category = $_POST['category'];
			$customer = $_POST['customer'];
			$pay_template = $_POST['pay_template'];
			$currency = $_POST['currency'];
			$general_description = $_POST['general_description'];
			$vat = $_POST['vat'];
			$ref_no = $_POST['ref_no'];
			$exchange = $_POST['exchange'];
			//cho ">>>>".$exchange;
			//echo''.$general_description.'  '.$currency;
			$customer_id = $t->rgf("customers",$customer,"cust_id","cust_id");
			$time = time();
			$user = user_id();
			$errors = array();

			$totalItems = $_POST['totalItems']; 

			$rel = time().user_id();

			$xx = array();
			if(empty($errors)){
				$d = new Db();
			$insert = $d->insert("other_invoice",["oi_category"=>$category,"oi_customer"=>$customer,"oi_customer_id"=>$customer,"oi_pay_template"=>$pay_template,"oi_currency"=>$currency,"oi_general_description"=>$general_description,"oi_added_by"=>$user,"oi_date_added"=>$time,"oi_date"=>strtotime($date),"oi_rel"=>$rel, "oi_vat"=>$vat,"oi_ref_no"=>$ref_no, "oi_source"=>'OTHERINVOICES',"oi_exchange_rate"=>$exchange]);
			//$insert = $d->insert("other_invoice",["oi_category"=>$category,"oi_customer"=>$customer,"oi_customer_id"=>$customer,"oi_pay_template"=>$pay_template,"oi_currency"=>$currency,"oi_general_description"=>$general_description,"oi_added_by"=>$user,"oi_date_added"=>$time,"oi_date"=>strtotime($date),"oi_rel"=>$rel, "oi_vat"=>$vat,"oi_ref_no"=>$ref_no, "oi_exchange"=>$exchange,"oi_source"=>'OTHERINVOICES']);
				echo $d->error();

				for($i=0; $i<$totalItems; $i++){
					$d = $_POST['description'.$i];
					$q = $_POST['quantity'.$i];
					$a = str_replace(',','',$_POST['amount'.$i]);
					$db = new Db();
					$insert = $db->insert("invoice_item_description",["ii_description"=>$d, "ii_amount"=>$a,"ii_quantity"=>$q,"ii_date_added"=>time(), "ii_added_by"=>user_id(), "ii_rel"=>$rel]);
					//echo $db->error();
				}
				
				$xx['message'] = 'Successfully Saved';

				$sql = "SELECT TOP 1 oi_id FROM other_invoice WHERE oi_rel = '$rel' ORDER BY oi_id DESC";
				$select = $db->select($sql);
				extract($select[0][0]);

				$xx['id'] = $oi_id;
        }
        // else{
        	
        //   $xx['message'] = ('Not Saved, '.$db->error());
        // }
		else{
      	$xx['message']='Error';
        
      }
          
echo json_encode($xx);
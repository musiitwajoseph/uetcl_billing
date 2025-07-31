<?php
Class OtherInvoices extends BeforeAndAfter{
	public $page = "OTHER INVOICES";
	
	public function __construct(){
		$access = new AccessRights();
		//$access->pageAccess(user_id(), $this->page, 'V');
	}
	
	public function getLinks(){
		$v = new OtherInvoices();
		$pending = $v->pending();
		$pending2 = $v->pending2();
		$pending3 = $v->pending3();
		$pending4 = $v->pending4();
		if($pending) $pending = ' <span style="color:red;font-weight:bold;">('.$pending.')</span>'; else $pending = "";
		if($pending2) $pending2 = ' <span style="color:red;font-weight:bold;">('.$pending2.')</span>'; else $pending2 = "";
		if($pending3) $pending3 = ' <span style="color:red;font-weight:bold;">('.$pending3.')</span>'; else $pending3 = "";
		if($pending4) $pending4 = ' <span style="color:red;font-weight:bold;">('.$pending4.')</span>'; else $pending4 = "";

		$page = "OTHER INVOICES";
		$links = array(
			array(
				"link_name"=>"Add Invoice", 
				"link_address"=>"other-invoices/add-invoice",
				"link_icon"=>"fa-plus",
				"link_page"=>$page,
				"link_right"=>"A",
			),
			array(
				"link_name"=>"View Invoices".$pending, 
				"link_address"=>"other-invoices/all-invoices",
				"link_icon"=>"fa-eye",
				"link_page"=>$page,
				"link_right"=>"V",
			),
			array(
				"link_name"=>"View Credit Notes ".$pending2, 
				"link_address"=>"other-invoices/all-credit-notes",
				"link_icon"=>"fa-eye",
				"link_page"=>$page,
				"link_right"=>"V",
			),
			array(
				"link_name"=>"View Debit Notes ".$pending3, 
				"link_address"=>"other-invoices/all-debt-notes",
				"link_icon"=>"fa-eye",
				"link_page"=>$page,
				"link_right"=>"V",
			),
			array(
				"link_name"=>"Check CN ".$pending4, 
				"link_address"=>"other-invoices/check-credit-note-status",
				"link_icon"=>"fa-eye",
				"link_page"=>$page,
				"link_right"=>"V",
			),
			array(
				"link_name"=>"Check DN ".$pending4, 
				"link_address"=>"other-invoices/check-debt-note-status",
				"link_icon"=>"fa-eye",
				"link_page"=>$page,
				"link_right"=>"V",
			)
		);
		
		return $links;
	}

	public function checkDebtNoteStatusAction(){

		$access = new AccessRights();
	?>
		<div class="col-md-12">
			
			<?php 
			$db = new Db();
			$select = $db->select("SELECT * FROM dn_other_invoice ORDER BY oi_date_added DESC");
			
			if(!$select){
				$db->error();
			}else{
				echo '<table cellspacing="0" cellpadding="2" border="1" width="100%" id="table">';
				echo '<thead>';
				echo '<tr>';
				echo '<th width="30px">No.</th>';
				echo '<th>Date</th>';
				echo '<th>Category</th>';
				echo '<th>Customer</th>';
				echo '<th>Orginal Invoice</th>';
				echo '<th>Debit Note ID</th>';
				echo '<th>Status Code</th>';
				echo '<th>Status</th>';

				if($access->sectionAccess(user_id(), $this->page, 'E') || $access->sectionAccess(user_id(), $this->page, 'D')){
					echo '<th width="">Action</th>';
				}
				echo '</tr>';
				echo '</thead>';
				
				$i=1;
				echo '<tbody>';
				foreach($select[0] as $row){
					extract($row);
					if($oi_status == 101) $ois = "Approved";
					elseif($oi_status == 102) $ois = "Submitted. Pending Approval";
					echo '<tr>';
					echo '<td><center>'.($i++).'.</center></td>';
					echo '<td>'.FeedBack::date_s($oi_date).'</td>';
					echo '<td>'.$this->rgf("categories",$oi_category,"cat_id","cat_name").'</td>';
					echo '<td>'.$this->rgf("customers",$oi_customer,"cust_id","cust_name").'</td>';
					echo '<td>'.$oi_efris_cn_ori_invoice_no.'</td>';
					echo '<td>'.$oi_efris_cn_id.'</td>';
					echo '<td>'.$oi_status.'</td>';
					echo '<td>'.$ois.'</td>';
					
					if($access->sectionAccess(user_id(), $this->page, 'E') || $access->sectionAccess(user_id(), $this->page, 'D')){
					
						echo '<td>';
						if(1){	
							echo '<a style="margin:auto 5px;" class="btn btn-xs btn-primary" href="'.return_url().'other-invoices/check-status2/'.$oi_id.'">Check DN</a>';			
						}
						echo"&nbsp;<a class='btn btn-default btn-xs' href=".return_url().'other-invoices/view-credit-note/'.$oi_id.">View</a> ";
						echo '</td>';
					}
					echo '</tr>';
					
					
				}
				echo '</tbody>';
				
				echo '</table>';
				
			
				
			}
			
			?>
		</div>
		<?php
	}

	

	public function checkCreditNoteStatusAction(){

		$access = new AccessRights();
	?>
		<div class="col-md-12">
			
			<?php 
			$db = new Db();
			$select = $db->select("SELECT * FROM cn_other_invoice ORDER BY oi_date_added DESC");
			
			if(!$select){
				$db->error();
			}else{
				echo '<table cellspacing="0" cellpadding="2" border="1" width="100%" id="table">';
				echo '<thead>';
				echo '<tr>';
				echo '<th width="30px">No.</th>';
				echo '<th>Date</th>';
				echo '<th>Category</th>';
				echo '<th>Customer</th>';
				echo '<th>Orginal Invoice</th>';
				echo '<th>Credit Note ID</th>';
				echo '<th>Status Code</th>';
				echo '<th>Status</th>';

				if($access->sectionAccess(user_id(), $this->page, 'E') || $access->sectionAccess(user_id(), $this->page, 'D')){
					echo '<th width="">Action</th>';
				}
				echo '</tr>';
				echo '</thead>';
				
				$i=1;
				echo '<tbody>';
				foreach($select[0] as $row){
					extract($row);
					if($oi_status == 101) $ois = "Approved";
					elseif($oi_status == 102) $ois = "Submitted. Pending Approval";
					echo '<tr>';
					echo '<td><center>'.($i++).'.</center></td>';
					echo '<td>'.FeedBack::date_s($oi_date).'</td>';
					echo '<td>'.$this->rgf("categories",$oi_category,"cat_id","cat_name").'</td>';
					echo '<td>'.$this->rgf("customers",$oi_customer,"cust_id","cust_name").'</td>';
					echo '<td>'.$oi_efris_cn_ori_invoice_no.'</td>';
					echo '<td>'.$oi_efris_cn_id.'</td>';
					echo '<td>'.$oi_status.'</td>';
					echo '<td>'.$ois.'</td>';
					
					if($access->sectionAccess(user_id(), $this->page, 'E') || $access->sectionAccess(user_id(), $this->page, 'D')){
					
						echo '<td>';
						if(1){	
							echo '<a style="margin:auto 5px;" class="btn btn-xs btn-primary" href="'.return_url().'other-invoices/check-status/'.$oi_id.'">Check CN</a>';			
						}
						echo"&nbsp;<a class='btn btn-default btn-xs' href=".return_url().'other-invoices/view-credit-note/'.$oi_id.">View</a> ";
						echo '</td>';
					}
					echo '</tr>';
					
					
				}
				echo '</tbody>';
				
				echo '</table>';
				
			
				
			}
			
			?>
		</div>
		<?php
	}

	

	public function viewDebtNoteAction(){
		$id = portion(3);
		///////////////////////////////////////////////////////////////////////////
		$db = new Db();
    	$select = $db->select("SELECT  TOP 1 oi_id as previous FROM dn_other_invoice WHERE oi_id < '$id' ORDER BY oi_id DESC");
    	extract($select[0][0]);
    	$previous;

   
    	$db = new Db();
    	
    	$select = $db->select("SELECT TOP 1  oi_id as next FROM dn_other_invoice WHERE oi_id > '$id' ORDER BY oi_id ASC");
    	extract($select[0][0]);
    	$next;


    	if(!empty($previous))
    		echo '<a href="'.return_url().'other-invoices/view-debt-note/'.$previous.'" class="mr-2 mb-5"><i class="fa fa-arrow-left"></i> Previous</a>';
    	else
    		echo '<i class="fa fa-arrow-left"></i> Previous ';

    	echo '&nbsp;&nbsp;&nbsp;';

    	if(!empty($next))
    		echo '<a href="'.return_url().'other-invoices/view-debt-note/'.$next.'" class="mr-2 mb-5">Next <i class="fa fa-arrow-right"></i> </a>';
    	else
    		echo 'Next <i class="fa fa-arrow-right"></i>';

    	echo '<br/>';

		/////////////////////////////////////////////////////////////////////////////


		$id = portion(3);
        $db = new Db();
        
        $sql = "SELECT * FROM dn_other_invoice WHERE oi_id = '$id'";
        $select = $db->select($sql);

        if($db->num_rows()){
	        extract($select[0][0]);
	        //if($oi_fdn)
	        
	        if(!$oi_efris_cn_id){			
				echo '<a href="'.return_url().'other-invoices/edit-debt-note/'.$oi_id.'"><i class="fa fa-fw fa-pencil"></i> Edit</a>';
			}

        $this->start_print("DEBIT NOTE");
			
	        echo '<h5>Debit Note Details:</h5>';
	        echo '<table class="table">';
	         echo '<tr>';
	        echo '<td style="width:150px;">Date</td>';

	        //$r = 'TEST'.date('y-m/', $oi_date).str_pad($oi_id, 5, '0', STR_PAD_LEFT);
	        echo '<td>'.FeedBack::date_s($oi_date).' &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; </td>';
	        echo '</tr>';
	        
	        /*
	        echo '<tr>';
	        echo '<td style="width:150px;">Category</td>';
	        echo '<td>'.$this->rgf("categories",$oi_category,"cat_id","cat_name").'</td>';
	        echo '</tr>';*/
	        
	        echo '<tr>';
	        echo '<td style="width:150px;">Customer</td>';
	        echo '<td>'.$this->rgf("customers",$oi_customer,"cust_id","cust_name").'</td>';
	        echo '</tr>'; 

	         echo '<tr>';
	        echo '<td style="width:150px;">Reference No.</td>';
	        echo '<td>'.$oi_ref_no.'</td>';
	        echo '</tr>';       
	             
	       
	        

	        echo '</table>';
	        //echo '<br/>';
	        //echo $this->action('edit','other-invoices/edit-invoice/'.$oi_id, 'Edit');



	    }

	echo '<table style="width:100%;" cellpadding="0" cellspacing="0" style="font-size:12px;">';		
				echo '<tr valign="top">';
				echo '<td colspan="2">';
				echo '</td>';
				echo '<td rowspan="7" style="width:300px;">';
				if($oi_cn_qr_code)
				echo '<img style="width:150px;" src="'.return_url().'efris_qr_images/'.$oi_cn_qr_code.'" alt="'.$oi_cn_fdn.'"/>';
				echo '</td>';
				echo '</tr>';
				echo '<tr valign="top"><td style="width:250px;"><b>Debit Note number: </b></td><td>'.$oi_cn_fdn.'</td></tr>';
				echo '<tr valign="top"><td><b>Debit Note Verification Code: </b></td><td>'.$oi_cn_vc.'</td></tr>';
				echo '<tr valign="top"><td><b>Orginal Invoice FDN: </b></td><td>'.$oi_efris_cn_ori_invoice_no.'</td></tr>';
				echo '<tr valign="top"><td><b>Debit Note ID: </b></td><td>'.$oi_efris_cn_id.'</td></tr>';
				echo '<tr valign="top"><td><b>Debit Note Status Code: </b></td><td>'.$oi_status.'</td></tr>';
				echo '<tr valign="top"><td><b>Debit Note Status: </b></td><td>Approved</td></tr>';
				echo '</table>';

       
	echo '<br/>';
			echo '<div id="preDetails">';
			$db = new Db();
			$select = $db->select("SELECT * FROM dn_invoice_item_description WHERE ii_rel ='$oi_rel'");
			//print_r($select);
			
			if(!$select){
				$db->error();
			}else{
				echo '<table cellspacing="0" cellpadding="2" border="1" width="100%" id="table" style="font-size:11px;">';
				echo '<theade>';


				echo '<tr>';
				echo '<th style="border:1px solid #000; font-weight:bold;" width="20px;">No. </th>';
				echo '<th style="border:1px solid #000; font-weight:bold;">Item</th>';
				echo '<th style="border:1px solid #000; font-weight:bold;">Quantity</th>';
				echo '<th style="border:1px solid #000; font-weight:bold;">Unit Measure</th>';
				echo '<th style="border:1px solid #000; font-weight:bold;">Unit Price</th>';
				echo '<th style="border:1px solid #000; font-weight:bold;">Total</th>';
				echo '<th style="border:1px solid #000; font-weight:bold;">Tax Category</th>';
				echo '</tr>';
				$number = 0;
				$total = 0;
				
				foreach($select[0] as $row){
					extract($row);
					//$ii_quantity *= -1;
					$number++;
					echo '<tr>';
					echo '<td style="border:1px solid #000;"width="20px;">'.$number.' </td>';
					echo '<td style="border:1px solid #000;">'.$this->rgf("efris_goods",$ii_description,"eg_id","eg_name").' </td>';
					echo '<td style="border:1px solid #000; text-align:right;">'.number_format($ii_quantity,2).' </td>';
					echo '<td style="border:1px solid #000; text-align:left;">'.$this->uom($this->rgf("efris_goods",$ii_description,"eg_id","eg_uom")).' </td>';
					echo '<td style="border:1px solid #000; text-align:right;">'.number_format($ii_amount,2).' </td>';
					echo '<td style="border:1px solid #000; text-align:right;">'.number_format($ii_quantity*$ii_amount,2).' </td>';
					echo '<td style="border:1px solid #000; text-align:left;">'.$this->VATCAT($this->rgf("dn_other_invoice",$id, "oi_id", "oi_vat"), 3).' </td>';
					echo '</tr>';
					$total += $ii_quantity*$ii_amount;
				}

					
				
				echo '</tbody>';
				
				echo '</table>';

				$oi_vat = $this->rgf("dn_other_invoice", $id, "oi_id", "oi_vat");
				$tax_rate =($oi_vat == "RES" || $oi_vat == 'NRS' || $oi_vat == 'IMP' || $oi_vat == 'IMPWHT') ? 18/100 : 0;
				
				$c = (is_infinite($total/$tax_rate))? 0 : $total/$tax_rate;



				echo '<table cellspacing="0" cellpadding="2" width="100%" id="table" style="font-size:11px;">';
				echo '<tr>';
				echo '<td colspan="7" style="text-align:center;font-weight:bold;"><br/></td>';
				echo '</tr>';

				echo '<tr>';
				echo '<td colspan="2">Net Amount: </td>';
				echo '<td colspan="5">'.number_format($total/(1+$tax_rate),2).'</td>';
				echo '</tr>';

				echo '<tr>';
				echo '<td colspan="2">Tax Amount: </td>';
				echo '<td colspan="5">'.number_format($total/(1+$tax_rate)*$tax_rate,2).'</td>';
				echo '</tr>';

				echo '<tr>';
				echo '<td colspan="2">Gross Amount: </td>';
				echo '<td colspan="5">'.number_format($total,2).'<br/>'.FeedBack::withCents(($total), $currency).'</td>';
				echo '</tr>';

				echo '<tr>';
				echo '<td colspan="2">Currency: </td>';
				echo '<td colspan="5">'.$this->rgf("dictionary",$this->rgf("dn_other_invoice",$id, "oi_id", "oi_currency"), "d_code", "d_name").'</td>';
				echo '</tr>';

				echo '<tr>';
				echo '<td colspan="2">Remarks: </td>';
				echo '<td colspan="5">'.$oi_general_description.'</td>';
				echo '</tr>';

				echo '</table>';


				echo '<br/>';
				
				echo '<table cellspacing="0" cellpadding="2" border="1" width="100%" id="table" style="font-size:11px;">';
				echo '<theade>';
				echo '<tr>';
				echo '<th rowspan="2" width="30px">No.</th>';
				echo '<th rowspan="2">Item</th>';
				echo '<th colspan="3">Previous Invoice Details (P)</th>';
				echo '<th colspan="3">Debit Note Details (D)</th>';
				echo '<th rowspan="2">Current Invoice Amount (P + D)</th>';
				echo '</tr>';

				echo '<tr>';
				echo '<th>Qty</th>';
				echo '<th>Unit Price</th>';
				echo '<th style="text-align:right;">Amount</th>';

				echo '<th>Qty</th>';
				echo '<th>Unit Price</th>';
				echo '<th style="text-align:right;">Amount</th>';

				echo '</tr>';
				echo '</theade>';
				
				$i=1;
				$total = 0;
				echo '<tbody>';
				foreach($select[0] as $row){
					extract($row);
					//$ii_quantity *= -1;
					echo '<tr>';
					echo '<td><center>'.($i++).'.</center></td>';
					echo '<td>'.$this->rgf("efris_goods",$ii_description,"eg_id","eg_name").'</td>';
				
					$orel = $this->rgf2("other_invoice", ["oi_invoice_id"=>$oi_efris_id], "oi_rel");

					$pii_quantity = (int)$this->rgf2("invoice_item_description",["ii_rel"=>$orel, "ii_description"=>$ii_description], "ii_quantity");
					$pii_amount = $this->rgf2("invoice_item_description",["ii_rel"=>$orel, "ii_description"=>$ii_description], "ii_amount");

					echo '<td>'.$pii_quantity.'</td>';
					echo '<td>'.number_format($pii_amount,2).'</td>';
					echo '<td style="text-align:right;">'.number_format($pii_amount*$pii_quantity,2).'</td>';


					echo '<td>'.$ii_quantity.'</td>';
					echo '<td>'.number_format($ii_amount,2).'</td>';
					echo '<td style="text-align:right;">'.number_format($ii_amount*$ii_quantity,2).'</td>';

					echo '<td style="text-align:right;">'.number_format($pii_amount*$pii_quantity + $ii_amount*$ii_quantity,2).'</td>';

					echo '</tr>';
					$total += $ii_amount*$ii_quantity;
					$ptotal += $pii_amount*$pii_quantity;
				}
				echo '<tr>';
				echo '<td colspan="4"><b>Total Amount</b></td>';
				echo '<td style="text-align:right;">';
				echo '<b>'.number_format($ptotal,2).'</b>'; 
				echo'</td>';
				echo '<td colspan="2"></td>';
				echo '<td style="text-align:right;">';
				echo '<b>'.number_format($total,2).'</b>'; 
				echo'</td>';
				echo '<td style="text-align:right;">';
				echo '<b>'.number_format($ptotal + $total,2).'</b>'; 
				echo'</td>';
				echo '</tr>';		
				
				echo '</tbody>';
				
				echo '</table>';
				echo "<br>";
				echo '</div>';
				

				// if($oi_currency == "UGX") $currency = "Shillings"; else if($oi_currency == "USD") $currency = "Dollars"; else "..................."; 
				// echo '<p><b>Total (in words)</b>: '.FeedBack::withCents($total*$tax_rate/100+$total, $currency).'</p>';

				if(!$oi_cn_fdn){
					echo '<button type="submit" class="efrisInvoicePreview"><i class="fa fa-fw fa-eye"></i>Show / Hide Preview</button>';
					echo '<div id="efrisInvoiceDetails" style="display:none;"></div>';
				}
			?>
			<script type="text/javascript">
				$(document).ready(function(){
					$(".efrisInvoicePreview").click(function(){
						$("#efrisInvoiceDetails").toggle();
						$("#efrisInvoiceDetails").css({"padding":"10px","border":"1px solid black","background-color":"white", "margin-top":"20px"});
						$("#efrisInvoiceDetails").html($('#preDetails').html());
					});
				});
			</script>
			<?php

		if($oi_efris_cn_id){
	echo '</div>';
	 $this->end_print();
	}else{

		echo '<input type="hidden" id="buyer" value="'.$oi_customer.'"/>';
		echo '<input type="hidden" id="date" value="'.$oi_date.'"/>';
		echo '<input type="hidden" id="ref" value="'.$oi_ref_no.'"/>';
		echo '<input type="hidden" id="currency" value="'.$oi_currency.'"/>';
		echo '<input type="hidden" id="id" value="'.$oi_id.'"/>';
		echo '<input type="hidden" id="vat" value="'.$oi_vat.'"/>';
		echo '<input type="hidden" id="efris_id" value="'.$oi_efris_id.'"/>';
		$items = array();

		$v = 1;
		foreach($select[0] as $row){
			extract($row);
			if(!empty($ii_amount) && !empty($ii_quantity)){
				echo '<input type="hidden" id="item'.$v.'" value="'.$ii_description.'"/>';
				echo '<input type="hidden" id="unitPrice'.$v.'" value="'.$ii_amount.'"/>';
				echo '<input type="hidden" id="qty'.$v.'" value="'.$ii_quantity.'"/>';
				$v++;
			}
		}
		echo '<input type="hidden" id="items" value="'.($v-1).'"/>';

				echo'<button type="button" id="pushInvoiceToEFRIS" name="send" class="btn btn-primary"><i class="fa fa-fw fa-plane"></i> Push Invoice</button>';
				echo '<span id="pushInvoiceToEFRISStatus"></span>';

		?>
<script type="text/javascript">		
		var urlPath = $('#urlPath').val();
		$('#pushInvoiceToEFRIS').click(function(){
			$('#pushInvoiceToEFRISStatus').html('<img src="'+urlPath+'images/loading.gif" alt="loading..."/>');						
			$(this).attr("disabled", true);
			var items = $('#items').val();
			if($('#ref').val()){
				var form_data = new FormData();				
				form_data.append('items', $('#items').val());
				
				for(i=1; i<=items; i++){
					form_data.append('item'+i, $('#item'+i).val());
					form_data.append('qty'+i, $('#qty'+i).val());
					form_data.append('unitPrice'+i, $('#unitPrice'+i).val());
					//alert($('#item'+i).val());
				}
				
				form_data.append('buyer', $('#buyer').val());
				form_data.append('ref', $('#ref').val());
				form_data.append('id', $('#id').val());
				form_data.append('currency', $('#currency').val());
				form_data.append('vat', $('#vat').val());
				form_data.append('date', $('#date').val());
				form_data.append('efris_id', $('#efris_id').val());

				//return 0;
				
				$.ajax({
					url: urlPath+"ajax/pushDNEFRIS_OtherInvoices.php",
					type: "POST",
					data: form_data,
					contentType: false,
					cache: false,
					processData:false,
					success: function(data){
						$('#pushInvoiceToEFRISStatus').html(data);
						//alert(data);
						var	values = JSON.parse(data);
						$('#pushInvoiceToEFRIS').attr("disabled",false);
						if(values.status == "error"){
							$('#pushInvoiceToEFRISStatus').html(values.error);
						}else{
							$('#pushInvoiceToEFRISStatus').html('Pushed');							
							$('#pushInvoiceToEFRISStatus').append(values.response);
							location.reload();						
						}
					}
				});
			}else{
				alert("Enter Lot No.");
				$('#pushInvoiceToEFRISStatus').html("");
			}
		});
	</script>
		<?php
	}

        		//echo '<p>'.$this->rgf("payment_information",$oi_pay_template,"t_id","t_description").'</p>';

				
			}
	    

	}
	
	
	public function viewCreditNoteAction(){
		$id = portion(3);
		///////////////////////////////////////////////////////////////////////////
		$db = new Db();
    	$select = $db->select("SELECT  TOP 1 oi_id as previous FROM cn_other_invoice WHERE oi_id < '$id' ORDER BY oi_id DESC");
    	extract($select[0][0]);
    	$previous;

   
    	$db = new Db();
    	
    	$select = $db->select("SELECT TOP 1  oi_id as next FROM cn_other_invoice WHERE oi_id > '$id' ORDER BY oi_id ASC");
    	extract($select[0][0]);
    	$next;


    	if(!empty($previous))
    		echo '<a href="'.return_url().'other-invoices/view-credit-note/'.$previous.'" class="mr-2 mb-5"><i class="fa fa-arrow-left"></i> Previous</a>';
    	else
    		echo '<i class="fa fa-arrow-left"></i> Previous ';

    	echo '&nbsp;&nbsp;&nbsp;';

    	if(!empty($next))
    		echo '<a href="'.return_url().'other-invoices/view-credit-note/'.$next.'" class="mr-2 mb-5">Next <i class="fa fa-arrow-right"></i> </a>';
    	else
    		echo 'Next <i class="fa fa-arrow-right"></i>';

    	echo '<br/><br/>';

		/////////////////////////////////////////////////////////////////////////////


		$id = portion(3);
        $db = new Db();
        
        $sql = "SELECT * FROM cn_other_invoice WHERE oi_id = '$id'";
        $select = $db->select($sql);

        if($db->num_rows()){
	        extract($select[0][0]);
	        echo '<h5>CREDIT NOTE :</h5>';
	        //if($oi_fdn)
        $this->start_print("CREDIT NOTE ");
			echo '<div style="font-size:12px;">';
	        echo '<table class="table">';
	         echo '<tr>';
	        echo '<td style="width:150px;">Date</td>';

	        //$r = 'TEST'.date('y-m/', $oi_date).str_pad($oi_id, 5, '0', STR_PAD_LEFT);
	        echo '<td>'.FeedBack::date_s($oi_date).' &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; '.$r.'</td>';
	        echo '</tr>';
	        
	        /*
	        echo '<tr>';
	        echo '<td style="width:150px;">Category</td>';
	        echo '<td>'.$this->rgf("categories",$oi_category,"cat_id","cat_name").'</td>';
	        echo '</tr>';*/


	        echo '<tr>';
	        echo '<td style="width:150px;">TIN</td>';
	        echo '<td>'.$this->rgf("customers",$oi_customer,"cust_id","cust_tin").'</td>';
	        echo '</tr>';
	        
	        echo '<tr>';
	        echo '<td style="width:150px;">Customer</td>';
	        echo '<td>'.$this->rgf("customers",$oi_customer,"cust_id","cust_name").'</td>';
	        echo '</tr>';


	        echo '<tr>';
	        echo '<td style="width:150px;">Reference No.</td>';
	        echo '<td>'.$oi_ref_no.'</td>';
	        echo '</tr>';       
	        /*
	        echo '<tr>';
	        echo '<td style="width:150px;">Currency</td>';
	        echo '<td>'.$oi_currency.'</td>';
	        echo '</tr>';*/

	        // echo '<tr>';
	        // echo '<td style="width:150px;">General Description</td>';
	        // echo '<td>'.$oi_general_description.'</td>';
	        // echo '</tr>';
	        
	       
	        

	        echo '</table>';
	        //echo '<br/>';
	        //echo $this->action('edit','other-invoices/edit-invoice/'.$oi_id, 'Edit');



	    }
	 
	    	if($oi_efris_cn_id){
	    		if($oi_status == 101) $ois = "Approved";
					elseif($oi_status == 102) $ois = "Submitted. Pending Approval";
				echo '<br/>';
				echo '<table style="width:100%;" cellpadding="0" cellspacing="0">';		
				echo '<tr valign="top">';
				echo '<td colspan="2">';
				echo '<b>EFRIS Credit Note Status DETAILS</b>';
				echo '</td>';
				if($oi_cn_qr_code){
				echo '<td rowspan="7" style="width:300px;"><img style="width:150px;" src="'.return_url().'efris_qr_images/'.$oi_cn_qr_code.'" alt="'.$oi_cn_fdn.'"/></td>';
			     }
				echo '</tr>';
				echo '<tr valign="top"><td style="width:250px;"><b>Credit Note number: </b></td><td>'.$oi_cn_fdn.'</td></tr>';
				echo '<tr valign="top"><td><b>Credit Note Verification Code: </b></td><td>'.$oi_cn_vc.'</td></tr>';
				echo '<tr valign="top"><td><b>Orginal Invoice FDN: </b></td><td>'.$oi_efris_cn_ori_invoice_no.'</td></tr>';
				echo '<tr valign="top"><td><b>Credit Note ID: </b></td><td>'.$oi_efris_cn_id.'</td></tr>';
				echo '<tr valign="top"><td><b>Credit Note Status Code: </b></td><td>'.$oi_status.'</td></tr>';
				echo '<tr valign="top"><td><b>Credit Note Status: </b></td><td>'.$ois.'</td></tr>';

				echo '</table>';
				echo '<div style="clear:both;"></div>';
			}
	echo '<br/>';

			$db = new Db();
			$select = $db->select("SELECT * FROM cn_invoice_item_description WHERE ii_rel ='$oi_rel'");
			//print_r($select);
			
			if(!$select){
				$db->error();
			}else{
				echo '<div id="preDetails">';
				echo '<table cellspacing="0" cellpadding="2" border="1" width="100%" id="table">';
				echo '<theade>';


				echo '<tr>';
				echo '<th style="border:1px solid #000; font-weight:bold;" width="20px;">No. </th>';
				echo '<th style="border:1px solid #000; font-weight:bold;">Item</th>';
				echo '<th style="border:1px solid #000; font-weight:bold;">Quantity</th>';
				echo '<th style="border:1px solid #000; font-weight:bold;">Unit Measure</th>';
				echo '<th style="border:1px solid #000; font-weight:bold;">Unit Price</th>';
				echo '<th style="border:1px solid #000; font-weight:bold;">Total</th>';
				echo '<th style="border:1px solid #000; font-weight:bold;">Tax Category</th>';
				echo '</tr>';
				$number = 0;
				$total = 0;
				
				foreach($select[0] as $row){
					extract($row);
					$ii_quantity *= -1;
					$number++;
					echo '<tr>';
					echo '<td style="border:1px solid #000;"width="20px;">'.$number.' </td>';
					echo '<td style="border:1px solid #000;">'.$this->rgf("efris_goods",$ii_description,"eg_id","eg_name").' </td>';
					echo '<td style="border:1px solid #000; text-align:right;">'.number_format($ii_quantity,2).' </td>';
					echo '<td style="border:1px solid #000; text-align:left;">'.$this->uom($this->rgf("efris_goods",$ii_description,"eg_id","eg_uom")).' </td>';
					echo '<td style="border:1px solid #000; text-align:right;">'.number_format($ii_amount,2).' </td>';
					echo '<td style="border:1px solid #000; text-align:right;">'.number_format($ii_quantity*$ii_amount,2).' </td>';
					echo '<td style="border:1px solid #000; text-align:left;">'.$this->VATCAT($this->rgf("cn_other_invoice",$id, "oi_id", "oi_vat"), 3).' </td>';
					echo '</tr>';
					$total += $ii_quantity*$ii_amount;
				}

				// echo '<tr>';
				// echo '<th width="30px">No.</th>';
				// echo '<th>Item</th>';
				// echo '<th>Qty</th>';
				// echo '<th>Unit Price</th>';
				// echo '<th style="text-align:right;">Amount</th>';

				// echo '</tr>';
				// echo '</theade>';
				
				// $i=1;
				// $total = 0;
				// echo '<tbody>';
				// foreach($select[0] as $row){
				// 	extract($row);
				// 	$ii_quantity *= -1;
				// 	echo '<tr>';
				// 	echo '<td><center>'.($i++).'.</center></td>';
				// 	echo '<td>'.$this->rgf("efris_goods",$ii_description,"eg_id","eg_name").'</td>';
				// 	echo '<td>'.$ii_quantity.'</td>';
				// 	echo '<td>'.number_format($ii_amount,2).'</td>';
				// 	echo '<td style="text-align:right;">'.number_format($ii_amount*$ii_quantity,2).'</td>';
				// 	echo '</tr>';
				// 	$total += $ii_amount*$ii_quantity;
				// }
				// echo '<tr>';
				// echo '<td colspan="4"><b>Total Amount</b></td>';
				// echo '<td style="text-align:right;">';
				// echo '<b>'.number_format($total,2).'</b>'; 
				// echo'</td>';
				// echo '</tr>';		
				
				echo '</tbody>';
				
				echo '</table>';

				$oi_vat = $this->rgf("cn_other_invoice", $id, "oi_id", "oi_vat");
				$tax_rate =($oi_vat == "RES" || $oi_vat == 'NRS' || $oi_vat == 'IMP' || $oi_vat == 'IMPWHT') ? 18/100 : 0;
				
				$c = (is_infinite($total/$tax_rate))? 0 : $total/$tax_rate;


				echo '<table cellspacing="0" cellpadding="2" width="100%" id="table">';
				echo '<tr>';
				echo '<td colspan="7" style="text-align:center;font-weight:bold;"><br/></td>';
				echo '</tr>';

				echo '<tr>';
				echo '<td colspan="2">Net Amount: </td>';
				echo '<td colspan="5">'.number_format($total/(1+$tax_rate),2).'</td>';
				echo '</tr>';

				echo '<tr>';
				echo '<td colspan="2">Tax Amount: </td>';
				echo '<td colspan="5">'.number_format($total/(1+$tax_rate)*$tax_rate,2).'</td>';
				echo '</tr>';

				echo '<tr>';
				echo '<td colspan="2">Gross Amount: </td>';
				echo '<td colspan="5">'.number_format($total,2).'<br/>'.FeedBack::withCents(($total), $currency).'</td>';
				echo '</tr>';

				echo '<tr>';
				echo '<td colspan="2">Currency: </td>';
				echo '<td colspan="5">'.$this->rgf("dictionary",$this->rgf("cn_other_invoice",$id, "oi_id", "oi_currency"), "d_code", "d_name").'</td>';
				echo '</tr>';

				echo '<tr>';
				echo '<td colspan="2">Remarks: </td>';
				echo '<td colspan="5">'.$oi_general_description.'</td>';
				echo '</tr>';

				echo '</table>';


				echo '<br/>';

				
				echo '<table cellspacing="0" cellpadding="2" border="1" width="100%" id="table">';
				echo '<theade>';
				echo '<tr>';
				echo '<th rowspan="2" width="30px">No.</th>';
				echo '<th rowspan="2">Item</th>';
				echo '<th colspan="3">Previous Invoice Details (P)</th>';
				echo '<th colspan="3">Credit Note Details (C)</th>';
				echo '<th rowspan="2">Current Invoice Amount (P - C)</th>';
				echo '</tr>';

				echo '<tr>';
				echo '<th>Qty</th>';
				echo '<th>Unit Price</th>';
				echo '<th style="text-align:right;">Amount</th>';

				echo '<th>Qty</th>';
				echo '<th>Unit Price</th>';
				echo '<th style="text-align:right;">Amount</th>';

				echo '</tr>';
				echo '</theade>';
				
				$i=1;
				$total = 0;
				echo '<tbody>';
				foreach($select[0] as $row){
					extract($row);
					$ii_quantity *= -1;
					echo '<tr>';
					echo '<td><center>'.($i++).'.</center></td>';
					echo '<td>'.$this->rgf("efris_goods",$ii_description,"eg_id","eg_name").'</td>';
				
					$orel = $this->rgf("other_invoice", $oi_efris_id, "oi_invoice_id", "oi_rel");
					$pii_quantity = $this->rgf("invoice_item_description", $orel, "ii_rel", "ii_quantity");
					$pii_amount = $this->rgf("invoice_item_description", $orel, "ii_rel", "ii_amount");

					echo '<td>'.$pii_quantity.'</td>';
					echo '<td>'.number_format($pii_amount,2).'</td>';
					echo '<td style="text-align:right;">'.number_format($pii_amount*$pii_quantity,2).'</td>';


					echo '<td>'.$ii_quantity.'</td>';
					echo '<td>'.number_format($ii_amount,2).'</td>';
					echo '<td style="text-align:right;">'.number_format($ii_amount*$ii_quantity,2).'</td>';

					echo '<td style="text-align:right;">'.number_format($pii_amount*$pii_quantity + $ii_amount*$ii_quantity,2).'</td>';

					echo '</tr>';
					$total += $ii_amount*$ii_quantity;
					$ptotal += $pii_amount*$pii_quantity;
				}
				echo '<tr>';
				echo '<td colspan="4"><b>Total Amount</b></td>';
				echo '<td style="text-align:right;">';
				echo '<b>'.number_format($ptotal,2).'</b>'; 
				echo'</td>';
				echo '<td colspan="2"></td>';
				echo '<td style="text-align:right;">';
				echo '<b>'.number_format($total,2).'</b>'; 
				echo'</td>';
				echo '<td style="text-align:right;">';
				echo '<b>'.number_format($ptotal + $total,2).'</b>'; 
				echo'</td>';
				echo '</tr>';		
				
				echo '</tbody>';
				
				echo '</table>';
				echo "<br>";
				echo '</div>';


				
				
				if($oi_currency == "UGX") $currency = "Shillings"; else if($oi_currency == "USD") $currency = "Dollars"; else "..................."; 
				$totalp = abs($total); 
				//echo '<p><b>Total (in words)</b>: '.FeedBack::withCents($totalp*$tax_rate/100+$totalp, $currency).'</p>';

				
				// echo '<div style="width:900px; padding:20px; background-color:white; margin:10px;">';
				// echo '<table border="1" cellpadding="0" cellspacing="0" style="width:100%;">';

				// echo '<tr>';
				// echo '<td colspan="7" style="text-align:center;font-weight:bold;">e-INVOICE/TAX INVOICE</td>';
				// echo '</tr>';

				// echo '<tr>';
				// echo '<td colspan="7" style="text-align:center;font-weight:bold;">Section A: Seller\'s Details</td>';
				// echo '</tr>';

				// echo '<tr>';
				// echo '<td colspan="2">TIN:</td>';
				// echo '<td colspan="5"></td>';
				// echo '</tr>';

				// echo '<tr>';
				// echo '<td colspan="2" style="width:300px;">Legal Name:</td>';
				// echo '<td colspan="5"></td>';
				// echo '</tr>';

				// echo '<tr>';
				// echo '<td colspan="2">Trade Name:</td>';
				// echo '<td colspan="5"></td>';
				// echo '</tr>';

				// echo '<tr>';
				// echo '<td colspan="2">Address</td>';
				// echo '<td colspan="5"></td>';
				// echo '</tr>';

				// echo '<tr>';
				// echo '<td colspan="2">Seller\'s Reference Number:</td>';
				// echo '<td colspan="5"></td>';
				// echo '</tr>';

				// echo '<tr>';
				// echo '<td colspan="2">Served by:</td>';
				// echo '<td colspan="5"></td>';
				// echo '</tr>';

				// echo '<tr>';
				// echo '<td colspan="2" style="text-align:center;font-weight:bold;">Section B: URA Information</td>';
				// echo '</tr>';

				// echo '<tr>';
				// echo '<td colspan="2">Document Type:</td>';
				// echo '<td colspan="5"></td>';
				// echo '</tr>';

				// echo '<tr>';
				// echo '<td colspan="2">Issued Date:</td>';
				// echo '<td colspan="5"></td>';
				// echo '</tr>';

				// echo '<tr>';
				// echo '<td colspan="2">Time:</td>';
				// echo '<td colspan="5"></td>';
				// echo '</tr>';

				// echo '<tr>';
				// echo '<td colspan="2">Device Number:</td>';
				// echo '<td colspan="5"></td>';
				// echo '</tr>';

				// echo '<tr>';
				// echo '<td colspan="2">Fiscal Document Number:</td>';
				// echo '<td colspan="5"></td>';
				// echo '</tr>';

				// echo '<tr>';
				// echo '<td colspan="2">Verification Code:</td>';
				// echo '<td colspan="5"></td>';
				// echo '</tr>';

				// echo '<tr>';
				// echo '<td colspan="7" style="text-align:center;font-weight:bold;">Section C: Buyer\'s Details</td>';
				// echo '</tr>';

				// echo '<tr>';
				// echo '<td colspan="2">Name: </td>';
				// echo '<tdcolspan="5">'.'</td>';
				// echo '</tr>';

				// echo '<tr>';
				// echo '<td colspan="7" style="text-align:center;font-weight:bold;">Section D: Goods & Services Details</td>';
				// echo '</tr>';

				// echo '<tr>';
				// echo '<td width="20px;">No. </td>';
				// echo '<td>Item</td>';
				// echo '<td>Quantity</td>';
				// echo '<td>Unit Measure</td>';
				// echo '<td>Unit Price</td>';
				// echo '<td>Total</td>';
				// echo '<td>Tax Category</td>';
				// echo '</tr>';

				// echo '<tr>';
				// echo '<td>Verification Code:</td>';
				// echo '<td></td>';
				// echo '</tr>';


				// echo '</table>';
				// echo '</div>';

				// echo '<button type="submit" class="efrisInvoicePreview"><i class="fa fa-fw fa-eye"></i>Show / Hide Preview</button>';
				// echo '</div>';
			?>
			<script type="text/javascript">
				$(document).ready(function(){
					$(".efrisInvoicePreview").click(function(){
						$("#efrisInvoiceDetails").toggle();
						$("#efrisInvoiceDetails").css({"padding":"10px","border":"1px solid black","background-color":"white", "margin-top":"20px"});
						$("#efrisInvoiceDetails").html($('#preDetails').html());
					});
				});
			</script>
			<?php

	if($oi_efris_cn_id){
		// echo '<br/>';
		// echo '<table style="" cellpadding="0" border="1">';		
		// echo '<tr valign="top">';
		// echo '<td colspan="2">';
		// echo '<b>EFRIS Credit Note Status DETAILS</b><br/><hr/>';
		// echo '</td>';
		// echo '</tr>';
		// echo '<tr valign="top"><td><b>Orginal Invoice Id: </b></td><td>'.$oi_efris_cn_ori_invoice_no.'</td></tr>';
		// echo '<tr valign="top"><td><b>Credit Note ID: </b></td><td>'.$oi_efris_cn_id.'</td></tr>';
		// echo '<tr valign="top"><td><b>Credit Note Status: </b></td><td>'.$oi_status.'</td></tr>';
		// echo '</table>';
		// echo '<div style="clear:both;"></div>';
	}else{

		echo '<input type="hidden" id="buyer" value="'.$oi_customer.'"/>';
		echo '<input type="hidden" id="date" value="'.$oi_date.'"/>';
		echo '<input type="hidden" id="ref" value="'.$oi_ref_no.'"/>';
		echo '<input type="hidden" id="currency" value="'.$oi_currency.'"/>';
		echo '<input type="hidden" id="id" value="'.$oi_id.'"/>';
		echo '<input type="hidden" id="vat" value="'.$oi_vat.'"/>';
		echo '<input type="hidden" id="efris_id" value="'.$oi_efris_id.'"/>';
		$items = array();

		$v = 1;
		foreach($select[0] as $row){
			extract($row);
			if(!empty($ii_amount) && !empty($ii_quantity)){
				echo '<input type="hidden" id="item'.$v.'" value="'.$ii_description.'"/>';
				echo '<input type="hidden" id="unitPrice'.$v.'" value="'.$ii_amount.'"/>';
				echo '<input type="hidden" id="qty'.$v.'" value="'.(-1*$ii_quantity).'"/>';
				$v++;
			}
		}
		echo '<input type="hidden" id="items" value="'.($v-1).'"/>';

				echo'<button type="button" id="pushInvoiceToEFRIS" name="send" class="btn btn-primary"><i class="fa fa-fw fa-plane"></i> Push Invoice</button>';
				echo '<span id="pushInvoiceToEFRISStatus"></span>';


			echo ' &nbsp; &nbsp; <button type="submit" class="efrisInvoicePreview"><i class="fa fa-fw fa-eye"></i>Show / Hide Preview</button>';
			echo '<div id="efrisInvoiceDetails" style="display:none;">';
			echo 'sdfs lsfdf sld f';
			echo '</div>';	

		?>
<script type="text/javascript">		
		var urlPath = $('#urlPath').val();
		$('#pushInvoiceToEFRIS').click(function(){
			$('#pushInvoiceToEFRISStatus').html('<img src="'+urlPath+'images/loading.gif" alt="loading..."/>');						
			$(this).attr("disabled", true);
			var items = $('#items').val();
			if($('#ref').val()){
				var form_data = new FormData();				
				form_data.append('items', $('#items').val());
				
				for(i=1; i<=items; i++){
					form_data.append('item'+i, $('#item'+i).val());
					form_data.append('qty'+i, $('#qty'+i).val());
					form_data.append('unitPrice'+i, $('#unitPrice'+i).val());
					//alert($('#item'+i).val());
				}
				
				form_data.append('buyer', $('#buyer').val());
				form_data.append('ref', $('#ref').val());
				form_data.append('id', $('#id').val());
				form_data.append('currency', $('#currency').val());
				form_data.append('vat', $('#vat').val());
				form_data.append('date', $('#date').val());
				form_data.append('efris_id', $('#efris_id').val());

				//return 0;
				
				$.ajax({
					url: urlPath+"ajax/pushCNEFRIS_OtherInvoices.php",
					type: "POST",
					data: form_data,
					contentType: false,
					cache: false,
					processData:false,
					success: function(data){
						$('#pushInvoiceToEFRISStatus').html(data);
						//alert(data);
						var	values = JSON.parse(data);
						$('#pushInvoiceToEFRIS').attr("disabled",false);
						if(values.status == "error"){
							$('#pushInvoiceToEFRISStatus').html(values.error);
						}else{
							$('#pushInvoiceToEFRISStatus').html('Pushed');							
							$('#pushInvoiceToEFRISStatus').append(values.response);
							location.reload();						
						}
					}
				});
			}else{
				alert("Enter Lot No.");
				$('#pushInvoiceToEFRISStatus').html("");
			}
		});
	</script>
		<?php
	}

        		//echo '<p>'.$this->rgf("payment_information",$oi_pay_template,"t_id","t_description").'</p>';
echo '</div>';
	        $this->end_print();
				
			}
	    

	}
	
	public function viewInvoiceAction(){
		$id = portion(3);
		///////////////////////////////////////////////////////////////////////////
		$db = new Db();
    	$select = $db->select("SELECT  TOP 1 oi_id as previous FROM other_invoice WHERE oi_id < '$id' ORDER BY oi_id DESC");
    	extract($select[0][0]);
    	$previous;
		

   
    	$db = new Db();
    	
    	$select = $db->select("SELECT TOP 1  oi_id as next FROM other_invoice WHERE oi_id > '$id' ORDER BY oi_id ASC");
    	extract($select[0][0]);
    	$next;


    	if(!empty($previous))
    		echo '<a href="'.return_url().'other-invoices/view-invoice/'.$previous.'" class="mr-2 mb-5"><i class="fa fa-arrow-left"></i> Previous</a>';
    	else
    		echo '<i class="fa fa-arrow-left"></i> Previous ';

    	echo '&nbsp;&nbsp;&nbsp;';

    	if(!empty($next))
    		echo '<a href="'.return_url().'other-invoices/view-invoice/'.$next.'" class="mr-2 mb-5">Next <i class="fa fa-arrow-right"></i> </a>';
    	else
    		echo 'Next <i class="fa fa-arrow-right"></i>';

    	echo '<br/><br/>';

		/////////////////////////////////////////////////////////////////////////////


		$id = portion(3);
        $db = new Db();
        
        $sql = "SELECT * FROM other_invoice WHERE oi_id = '$id'";
        $select = $db->select($sql);

        if($db->num_rows()){
	        extract($select[0][0]);
			
	        if(!$oi_fdn){			
			echo '<a href="'.return_url().'other-invoices/edit-invoice/'.$oi_id.'"><i class="fa fa-fw fa-pencil"></i> Edit</a>';
			}
       
		$this->start_print("INVOICE");
		
			echo '<div style="font-size:12px;">';
	        echo '<h5>Invoice Details:</h5>';
	        echo '<table class="table">';
	         echo '<tr>';
	        echo '<td style="width:150px;">Date</td>';

	        $r = $oi_ref_no;//'TEST'.date('y-m/', $oi_date).str_pad($oi_id, 5, '0', STR_PAD_LEFT);
	         

	        echo '<td>'.FeedBack::date_s($oi_date).' &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; Invoice No.: '.$r.'</td>';
	        echo '</tr>';
	        
	        /*
	        echo '<tr>';
	        echo '<td style="width:150px;">Category</td>';
	        echo '<td>'.$this->rgf("categories",$oi_category,"cat_id","cat_name").'</td>';
	        echo '</tr>';*/
	        
	        echo '<tr>';
	        echo '<td style="width:150px;">TIN</td>';
	        echo '<td>'.$this->rgf("customers",$oi_customer,"cust_id","cust_tin").'</td>';
	        echo '</tr>';

	        echo '<tr>';
	        echo '<td style="width:150px;">Customer</td>';
	        echo '<td>'.$this->rgf("customers",$oi_customer,"cust_id","cust_name").' &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; CustomerType :'.$this->rgf("customers",$oi_customer,"cust_id","cust_type").'</td>';
	        echo '</tr>';
      
	        /*
	        echo '<tr>';
	        echo '<td style="width:150px;">Currency</td>';
	        echo '<td>'.$oi_currency.'</td>';
	        echo '</tr>';*/

	        echo '<tr>';
	        echo '<td style="width:150px;">General Description</td>';
	        echo '<td>'.$oi_general_description.'</td>';
	        echo '</tr>';
	        
	       
	        

	        echo '</table>';
	        //echo '<br/>';
	        //echo $this->action('edit','other-invoices/edit-invoice/'.$oi_id, 'Edit');



	   //}
	echo '<br/>';

			$db = new Db();
			$select = $db->select("SELECT * FROM invoice_item_description WHERE ii_rel ='$oi_rel'");
			//print_r($select);
			
			
			if(!$select){
				$db->error();
			}else{
				if(!$oi_fdn){
				echo '<table cellspacing="0" cellpadding="2" border="1" width="100%" id="table">';
				echo '<theade>';
				echo '<tr>';
				echo '<th width="30px">No.</th>';
				echo '<th>Item</th>';
				echo '<th>Qty</th>';
				echo '<th>Unit Price</th>';
				echo '<th style="text-align:right;">Amount</th>';

				echo '</tr>';
				echo '</theade>';
				}
				
				$i=1;
				$total = 0;
				echo '<tbody>';
				$itemsA = array();
				foreach($select[0] as $row){
					extract($row);
					$ii_amount = ($oi_vat == 'IMPWHT')? $ii_amount/0.85 : $ii_amount;
					//$ii_amount = ($oi_vat == 'IMPWHT')? $ii_amount * 1.18 : $ii_amount;
					if(!$oi_fdn){
					echo '<tr>';
					echo '<td><center>'.($i++).'.</center></td>';
					echo '<td>'.$this->rgf("efris_goods",$ii_description,"eg_id","eg_name").'</td>';
					echo '<td>'.$ii_quantity.'</td>';
					echo '<td>'.number_format($ii_amount,2).'</td>';
					echo '<td style="text-align:right;">'.number_format($ii_amount*$ii_quantity,2).'</td>';
					echo '</tr>';
					}

					$total += $ii_amount*$ii_quantity;
					
					$tax_rate =($oi_vat == "RES" || $oi_vat == 'NRS' || $oi_vat == 'IMP' || $oi_vat == 'IMPWHT') ? 18/100 : 0;
					
					if($oi_vat == 'IMPWHT'){
						$ii_amount *= 1.15;
						$itemsA[] = array(
							($i-1), 
							$this->rgf("efris_goods",$ii_description,"eg_id","eg_name"),
							$ii_quantity,
							($ii_amount+$ii_amount*tax_rate),
							(($ii_amount+$ii_amount*tax_rate)*$ii_quantity),
							$this->uom($this->rgf("efris_goods",$ii_description,"eg_id","eg_uom"))
						);
					}else{
						$itemsA[] = array(
							($i-1), 
							$this->rgf("efris_goods",$ii_description,"eg_id","eg_name"),
							$ii_quantity,
							($ii_amount+$ii_amount*tax_rate),
							(($ii_amount+$ii_amount*tax_rate)*$ii_quantity),
							$this->uom($this->rgf("efris_goods",$ii_description,"eg_id","eg_uom"))
						);
					}
				}
				if(!$oi_fdn){
				echo '<tr>';
					echo '<td colspan="4"><b>Total Amount</b></td>';
					echo '<td style="text-align:right;">';
					echo '<b>'.number_format($total,2).'</b>'; 
					echo'</td>';
					echo '</tr>';
				}


					if($oi_vat == "RES" || $oi_vat == 'NRS' || $oi_vat == 'IMP' || $oi_vat == 'IMPWHT') $oi_tax_rate = 18;

					$tax_rate =($oi_vat == "RES" || $oi_vat == 'NRS' || $oi_vat == 'IMP' || $oi_vat == 'IMPWHT') ? 18 : 0;
			
			/*echo '<tr>';
			echo '<td colspan="4" style="text-align:right;"><b>TAX RATE ('.$this->VATCAT($oi_vat,2).' '.$tax_rate.'%):</b></td>';
			echo '<td style="text-align:right;font-weight:bold;">'.number_format($total*$tax_rate/100, 2).'</td>';			
			echo '</tr>';*/
			if(!$oi_fdn){
			echo '<tr>';
			echo '<td colspan="4" style="text-align:right;"><b>Grand Total:</b></td>';
			echo '<td style="text-align:right;font-weight:bold;">'.number_format($total, 2).'</td>';			
			echo '</tr>';
				
				echo '</tbody>';
				
				echo '</table>';
				echo "<br>";

				if($oi_currency == "UGX") $currency = "Shillings"; else if($oi_currency == "USD") $currency = "Dollars"; else "..................."; 
				echo '<p><b>Total (in words)</b>: '.FeedBack::withCents($total, $this->rgf("dictionary", $this->rgf("other_invoice",$id, "oi_id", "oi_currency"), "d_code", "d_name")).'</p>';
			}
		}

				$buyer = $this->rgf("customers",$oi_customer,"cust_id","cust_name");
				$tin = $this->rgf("customers",$oi_customer,"cust_id","cust_tin");
				$qr = '<img style="text-align:center;width:150px;" src="'.return_url().'efris_qr_images/'.$oi_qr.'" alt="'.$oi_fdn.'"/>';
				$vc = $oi_vc;
				$fdn = $oi_fdn;
				$this->preview($buyer, $tin, $itemsA, $currency, $qr, $vc, $fdn, $oi_general_description, $this->full_name($oi_added_by), $oi_ref_no, $oi_id);
				//echo"@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@";

		if($oi_fdn){
		// echo '<br/>';
		// echo '<table style="" cellpadding="0">';		
		// echo '<tr valign="top">';
		// echo '<td rowspan="3">';
		// echo '<img style="width:200px;" src="'.return_url().'efris_qr_images/'.$oi_qr.'" alt="'.$oi_fdn.'"/>';
		// echo '</td>';
		// echo '<td colspan="2">';
		// echo '<b>EFRIS DETAILS</b><br/><hr/>';
		// echo '</td>';
		// echo '</tr>';
		// echo '<tr valign="top"><td><b>Fiscal Document Number: </b></td><td>'.$oi_fdn.'</td></tr>';
		// echo '<tr valign="top"><td><b>Verification Code: </b></td><td>'.$oi_vc.'</td></tr>';
		// echo '</table>';
		// echo '<div style="clear:both;"></div>';
	}else{

		echo '<input type="hidden" id="buyer" value="'.$oi_customer.'"/>';
		echo '<input type="hidden" id="date" value="'.$oi_date.'"/>';
		echo '<input type="hidden" id="ref" value="'.$r.'"/>';
		echo '<input type="hidden" id="currency" value="'.$oi_currency.'"/>';
		echo '<input type="hidden" id="id" value="'.$oi_id.'"/>';
		echo '<input type="hidden" id="vat" value="'.$oi_vat.'"/>';

		echo '<input type="hidden" id="a_currency" value="'.$this->rgf("dictionary",$oi_currency,"d_code","d_name").'"/>';
		echo '<input type="hidden" id="a_amount" value="'.number_format($total, 2).'"/>';
		echo '<input type="hidden" id="a_date" value="'.FeedBack::date_s($oi_date).'"/>';
		$items = array();

		$v = 1;
		foreach($select[0] as $row){
			extract($row);
			if(!empty($ii_amount) && !empty($ii_quantity)){
				echo '<input type="hidden" id="item'.$v.'" value="'.$ii_description.'"/>';
				echo '<input type="hidden" id="unitPrice'.$v.'" value="'.$ii_amount.'"/>';
				echo '<input type="hidden" id="qty'.$v.'" value="'.$ii_quantity.'"/>';
				$v++;
			}
		}
		echo '<input type="hidden" id="items" value="'.($v-1).'"/>';
		echo '<input type="hidden" id="remarks" value="'.$oi_general_description.'"/>';

				echo'<button type="button" id="pushInvoiceToEFRIS" name="send" class="btn btn-primary"><i class="fa fa-fw fa-plane"></i> Push Invoice</button>';
				echo '<span id="pushInvoiceToEFRISStatus"></span>';

		?>
<script type="text/javascript">		
		var urlPath = $('#urlPath').val();
		$('#pushInvoiceToEFRIS').click(function(){	
			var items = $('#items').val();

			var a_currency = $('#a_currency').val();
			var a_amount = $('#a_amount').val();
			var a_date = $('#a_date').val();

			if(!confirm('Please confirm the details below\nAmount: '+a_amount+'\nCurrency: '+a_currency+'\nInvoice Date: '+a_date+'')) return false;

			$(this).attr("disabled", true);
			$('#pushInvoiceToEFRISStatus').html('<img src="'+urlPath+'images/loading.gif" alt="loading..."/>');	

			if($('#ref').val()){
				var form_data = new FormData();				
				form_data.append('items', $('#items').val());
				
				for(i=1; i<=items; i++){
					form_data.append('item'+i, $('#item'+i).val());
					form_data.append('qty'+i, $('#qty'+i).val());
					form_data.append('unitPrice'+i, $('#unitPrice'+i).val());
					//alert($('#item'+i).val());
				}
				
				form_data.append('buyer', $('#buyer').val());
				form_data.append('ref', $('#ref').val());
				form_data.append('id', $('#id').val());
				form_data.append('currency', $('#currency').val());
				form_data.append('vat', $('#vat').val());
				form_data.append('date', $('#date').val());
				form_data.append('remarks', $('#remarks').val());

				//return 0;
				
				$.ajax({
					url: urlPath+"ajax/pushEFRIS_OtherInvoices.php",
					type: "POST",
					data: form_data,
					contentType: false,
					cache: false,
					processData:false,
					success: function(data){
						//alert(data);
						var	values = JSON.parse(data);
						$('#pushInvoiceToEFRIS').attr("disabled",false);
						if(values.status == "error"){
							$('#pushInvoiceToEFRISStatus').html(values.error);
						}else{
							$('#pushInvoiceToEFRISStatus').html('Pushed');							
							$('#pushInvoiceToEFRISStatus').append(values.response);
							location.reload();						
						}
					}
				});
			}else{
				alert("Enter Lot No.");
				$('#pushInvoiceToEFRISStatus').html("");
			}
		});
	</script>
		<?php
	}

        		//echo '<p>'.$this->rgf("payment_information",$oi_pay_template,"t_id","t_description").'</p>';
echo '</div>';
	       
				
			}
	    

	}
	
	public function cancelDebitNoteAction(){
		$id = portion(3);
		///////////////////////////////////////////////////////////////////////////
		$db = new Db();
    	$select = $db->select("SELECT  TOP 1 oi_id as previous FROM dn_other_invoice WHERE oi_id < '$id' ORDER BY oi_id DESC");
    	extract($select[0][0]);
    	$previous;

   
    	$db = new Db();
    	
    	$select = $db->select("SELECT TOP 1  oi_id as next FROM dn_other_invoice WHERE oi_id > '$id' ORDER BY oi_id ASC");
    	extract($select[0][0]);
    	$next;


		/////////////////////////////////////////////////////////////////////////////


		$id = portion(3);
        $db = new Db();
        
        $sql = "SELECT * FROM dn_other_invoice WHERE oi_id = '$id'";
        $select = $db->select($sql);

        if($db->num_rows()){
	        extract($select[0][0]);
	        if($oi_fdn)
        $this->start_print("Credit note INVOICE");
			echo '<div style="font-size:12px;">';
	        echo '<h5>Credit Details:</h5>';
	        echo '<table class="table">';
	         echo '<tr>';
	        echo '<td style="width:150px;">Date</td>';

	        $r = 'TEST'.date('y-m/', $oi_date).str_pad($oi_id, 5, '0', STR_PAD_LEFT);
	        echo '<td>'.FeedBack::date_s($oi_date).' &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; Invoice No.: '.$r.'</td>';
	        echo '</tr>';
	        
	        /*
	        echo '<tr>';
	        echo '<td style="width:150px;">Category</td>';
	        echo '<td>'.$this->rgf("categories",$oi_category,"cat_id","cat_name").'</td>';
	        echo '</tr>';*/
	        
	        echo '<tr>';
	        echo '<td style="width:150px;">Customer</td>';
	        echo '<td>'.$this->rgf("customers",$oi_customer,"cust_id","cust_name").'</td>';
	        echo '</tr>';       
	        /*
	        echo '<tr>';
	        echo '<td style="width:150px;">Currency</td>';
	        echo '<td>'.$oi_currency.'</td>';
	        echo '</tr>';*/

	        echo '<tr>';
	        echo '<td style="width:150px;">General Description</td>';
	        echo '<td>'.$oi_general_description.'</td>';
	        echo '</tr>';
	        
	       
	        

	        echo '</table>';
	        //echo '<br/>';
	        //echo $this->action('edit','other-invoices/edit-invoice/'.$oi_id, 'Edit');



	    }
	echo '<br/>';

			$db = new Db();
			$select = $db->select("SELECT * FROM dn_invoice_item_description WHERE ii_rel ='$oi_rel'");
			//print_r($select);
			
			if(!$select){
				$db->error();
			}else{
				echo '<table cellspacing="0" cellpadding="2" border="1" width="100%" id="table">';
				echo '<theade>';
				echo '<tr>';
				echo '<th width="30px">No.</th>';
				echo '<th>Item</th>';
				echo '<th>Qty</th>';
				echo '<th>Unit Price</th>';
				echo '<th style="text-align:right;">Amount</th>';

				echo '</tr>';
				echo '</theade>';
				
				$i=1;
				$total = 0;
				echo '<tbody>';
				foreach($select[0] as $row){
					extract($row);
					echo '<tr>';
					echo '<td><center>'.($i++).'.</center></td>';
					echo '<td>'.$this->rgf("efris_goods",$ii_description,"eg_id","eg_name").'</td>';
					echo '<td>'.$ii_quantity.'</td>';
					echo '<td>'.number_format($ii_amount,2).'</td>';
					echo '<td style="text-align:right;">'.number_format($ii_amount*$ii_quantity,2).'</td>';
					echo '</tr>';
					$total += $ii_amount*$ii_quantity;
				}
				echo '<tr>';
					echo '<td colspan="4"><b>Total Amount</b></td>';
					echo '<td style="text-align:right;">';
					echo '<b>'.number_format($total,2).'</b>'; 
					echo'</td>';
					echo '</tr>';

					if($oi_vat == "VATABLE(18%)") $oi_tax_rate = 18;

					$tax_rate =($oi_vat != "VATABLE(18%)") ? 0:18;
			
			echo '<tr>';
			echo '<td colspan="4" style="text-align:right;"><b>TAX RATE ('.$tax_rate.'%):</b></td>';
			echo '<td style="text-align:right;font-weight:bold;">'.number_format($total*$tax_rate/100, 2).'</td>';			
			echo '</tr>';
			
			echo '<tr>';
			echo '<td colspan="4" style="text-align:right;"><b>Grand Total:</b></td>';
			echo '<td style="text-align:right;font-weight:bold;">'.number_format($total*$tax_rate/100+$total, 2).'</td>';			
			echo '</tr>';
				
				echo '</tbody>';
				
				echo '</table>';
				echo "<br>";

				if($oi_currency == "UGX") $currency = "Shillings"; else if($oi_currency == "USD") $currency = "Dollars"; else "..................."; 
				echo '<p><b>Total (in words)</b>: '.FeedBack::withCents($total*$tax_rate/100+$total, $currency).'</p>';

				
			?>
			<script type="text/javascript">
				$(document).ready(function(){
					$(".efrisInvoicePreview").click(function(){
						$("#efrisInvoiceDetails").toggle();
					});
				});
			</script>
			<?php

		echo '<br/>';
		echo '<table style="" cellpadding="0" border=1>';
		echo '<tr valign="top"><td><b>Orginal Invoice Id: </b></td><td>'.$oi_efris_cn_ori_invoice_no.'</td></tr>';
		echo '<tr valign="top"><td><b>Debit Note ID: </b></td><td>'.$oi_efris_cn_id.'</td></tr>';
		echo '<tr valign="top"><td><b>Debit Note Status: </b></td><td>'.$oi_status.'</td></tr>';
		echo '</table>';

		if($oi_status==101010){
			echo 'Cancel:';	
		}else{

		echo '<input type="hidden" id="ori_fdn" value="'.$oi_efris_cn_ori_invoice_no.'"/>';
		echo '<input type="hidden" id="cn_id" value="'.$oi_efris_cn_id.'"/>';
		echo '<input type="hidden" id="reason_code" value="102"/>';
		echo '<input type="hidden" id="reason" value="test"/>';
		echo '<input type="hidden" id="appCatCode" value="104"/>';

		echo'<button type="button" id="pushInvoiceToEFRIS" name="send" class="btn btn-primary"><i class="fa fa-fw fa-plane"></i> Cancel Credit Note</button>';
		echo '<span id="pushInvoiceToEFRISStatus"></span>';

		?>
<script type="text/javascript">		
		var urlPath = $('#urlPath').val();
		$('#pushInvoiceToEFRIS').click(function(){
			$('#pushInvoiceToEFRISStatus').html('<img src="'+urlPath+'images/loading.gif" alt="loading..."/>');						
			$(this).attr("disabled", true);
			if(1){
				var form_data = new FormData();		
				
				form_data.append('ori_fdn', $('#ori_fdn').val());
				form_data.append('cn_id', $('#cn_id').val());
				form_data.append('id', $('#id').val());
				form_data.append('reason_code', $('#reason_code').val());
				form_data.append('reason', $('#reason').val());
				form_data.append('appCatCode', $('#appCatCode').val());

				$.ajax({
					url: urlPath+"ajax/pushCancelDNEFRIS_OtherInvoices.php",
					type: "POST",
					data: form_data,
					contentType: false,
					cache: false,
					processData:false,
					success: function(data){
						$('#pushInvoiceToEFRISStatus').html(data);
						//alert(data);
						var	values = JSON.parse(data);
						$('#pushInvoiceToEFRIS').attr("disabled",false);
						if(values.status == "error"){
							$('#pushInvoiceToEFRISStatus').html(values.error);
						}else{
							$('#pushInvoiceToEFRISStatus').html('Pushed');							
							$('#pushInvoiceToEFRISStatus').append(values.response);
							//location.reload();						
						}
					}
				});
			}else{
				alert("Enter Lot No.");
				$('#pushInvoiceToEFRISStatus').html("");
			}
		});
	</script>
		<?php
	}

        		//echo '<p>'.$this->rgf("payment_information",$oi_pay_template,"t_id","t_description").'</p>';
echo '</div>';
	        $this->end_print();
				
			}
	    

	}
	
	public function cancelCreditNoteAction(){
		$id = portion(3);
		///////////////////////////////////////////////////////////////////////////
		$db = new Db();
    	$select = $db->select("SELECT  TOP 1 oi_id as previous FROM cn_other_invoice WHERE oi_id < '$id' ORDER BY oi_id DESC");
    	extract($select[0][0]);
    	$previous;

   
    	$db = new Db();
    	
    	$select = $db->select("SELECT TOP 1  oi_id as next FROM cn_other_invoice WHERE oi_id > '$id' ORDER BY oi_id ASC");
    	extract($select[0][0]);
    	$next;


		/////////////////////////////////////////////////////////////////////////////


		$id = portion(3);
        $db = new Db();
        
        $sql = "SELECT * FROM cn_other_invoice WHERE oi_id = '$id'";
        $select = $db->select($sql);

        if($db->num_rows()){
	        extract($select[0][0]);
	        if($oi_fdn)
        $this->start_print("Credit note INVOICE");
			echo '<div style="font-size:12px;">';
	        echo '<h5>Credit Details:</h5>';
	        echo '<table class="table">';
	         echo '<tr>';
	        echo '<td style="width:150px;">Date</td>';

	        $r = 'TEST'.date('y-m/', $oi_date).str_pad($oi_id, 5, '0', STR_PAD_LEFT);
	        echo '<td>'.FeedBack::date_s($oi_date).' &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; Invoice No.: '.$r.'</td>';
	        echo '</tr>';
	        
	        /*
	        echo '<tr>';
	        echo '<td style="width:150px;">Category</td>';
	        echo '<td>'.$this->rgf("categories",$oi_category,"cat_id","cat_name").'</td>';
	        echo '</tr>';*/
	        
	        echo '<tr>';
	        echo '<td style="width:150px;">Customer</td>';
	        echo '<td>'.$this->rgf("customers",$oi_customer,"cust_id","cust_name").'</td>';
	        echo '</tr>';       
	        /*
	        echo '<tr>';
	        echo '<td style="width:150px;">Currency</td>';
	        echo '<td>'.$oi_currency.'</td>';
	        echo '</tr>';*/

	        echo '<tr>';
	        echo '<td style="width:150px;">General Description</td>';
	        echo '<td>'.$oi_general_description.'</td>';
	        echo '</tr>';
	        
	       
	        

	        echo '</table>';
	        //echo '<br/>';
	        //echo $this->action('edit','other-invoices/edit-invoice/'.$oi_id, 'Edit');



	    }
	echo '<br/>';

			$db = new Db();
			$select = $db->select("SELECT * FROM cn_invoice_item_description WHERE ii_rel ='$oi_rel'");
			//print_r($select);
			
			if(!$select){
				$db->error();
			}else{
				echo '<table cellspacing="0" cellpadding="2" border="1" width="100%" id="table">';
				echo '<theade>';
				echo '<tr>';
				echo '<th width="30px">No.</th>';
				echo '<th>Item</th>';
				echo '<th>Qty</th>';
				echo '<th>Unit Price</th>';
				echo '<th style="text-align:right;">Amount</th>';

				echo '</tr>';
				echo '</theade>';
				
				$i=1;
				$total = 0;
				echo '<tbody>';
				foreach($select[0] as $row){
					extract($row);
					echo '<tr>';
					echo '<td><center>'.($i++).'.</center></td>';
					echo '<td>'.$this->rgf("efris_goods",$ii_description,"eg_id","eg_name").'</td>';
					echo '<td>'.$ii_quantity.'</td>';
					echo '<td>'.number_format($ii_amount,2).'</td>';
					echo '<td style="text-align:right;">'.number_format($ii_amount*$ii_quantity,2).'</td>';
					echo '</tr>';
					$total += $ii_amount*$ii_quantity;
				}
				echo '<tr>';
					echo '<td colspan="4"><b>Total Amount</b></td>';
					echo '<td style="text-align:right;">';
					echo '<b>'.number_format($total,2).'</b>'; 
					echo'</td>';
					echo '</tr>';

					if($oi_vat == "VATABLE(18%)") $oi_tax_rate = 18;

					$tax_rate =($oi_vat != "VATABLE(18%)") ? 0:18;
			
			echo '<tr>';
			echo '<td colspan="4" style="text-align:right;"><b>TAX RATE ('.$tax_rate.'%):</b></td>';
			echo '<td style="text-align:right;font-weight:bold;">'.number_format($total*$tax_rate/100, 2).'</td>';			
			echo '</tr>';
			
			echo '<tr>';
			echo '<td colspan="4" style="text-align:right;"><b>Grand Total:</b></td>';
			echo '<td style="text-align:right;font-weight:bold;">'.number_format($total*$tax_rate/100+$total, 2).'</td>';			
			echo '</tr>';
				
				echo '</tbody>';
				
				echo '</table>';
				echo "<br>";

				if($oi_currency == "UGX") $currency = "Shillings"; else if($oi_currency == "USD") $currency = "Dollars"; else "..................."; 
				echo '<p><b>Total (in words)</b>: '.FeedBack::withCents($total*$tax_rate/100+$total, $currency).'</p>';

				
			?>
			<script type="text/javascript">
				$(document).ready(function(){
					$(".efrisInvoicePreview").click(function(){
						$("#efrisInvoiceDetails").toggle();
					});
				});
			</script>
			<?php

		echo '<br/>';
		echo '<table style="" cellpadding="0" border=1>';
		echo '<tr valign="top"><td><b>Orginal Invoice Id: </b></td><td>'.$oi_efris_cn_ori_invoice_no.'</td></tr>';
		echo '<tr valign="top"><td><b>Credit Note ID: </b></td><td>'.$oi_efris_cn_id.'</td></tr>';
		echo '<tr valign="top"><td><b>Credit Note Status: </b></td><td>'.$oi_status.'</td></tr>';
		echo '</table>';

		if($oi_status==101010){
			echo 'Cancel:';	
		}else{

		echo '<input type="hidden" id="ori_fdn" value="'.$oi_efris_cn_ori_invoice_no.'"/>';
		echo '<input type="hidden" id="cn_id" value="'.$oi_efris_cn_id.'"/>';
		echo '<input type="hidden" id="reason_code" value="101"/>';
		echo '<input type="hidden" id="reason" value=""/>';
		echo '<input type="hidden" id="appCatCode" value="104"/>';

		echo'<button type="button" id="pushInvoiceToEFRIS" name="send" class="btn btn-primary"><i class="fa fa-fw fa-plane"></i> Cancel Credit Note</button>';
		echo '<span id="pushInvoiceToEFRISStatus"></span>';

		?>
<script type="text/javascript">		
		var urlPath = $('#urlPath').val();
		$('#pushInvoiceToEFRIS').click(function(){
			$('#pushInvoiceToEFRISStatus').html('<img src="'+urlPath+'images/loading.gif" alt="loading..."/>');						
			$(this).attr("disabled", true);
			if(1){
				var form_data = new FormData();		
				
				form_data.append('ori_fdn', $('#ori_fdn').val());
				form_data.append('cn_id', $('#cn_id').val());
				form_data.append('id', $('#id').val());
				form_data.append('reason_code', $('#reason_code').val());
				form_data.append('reason', $('#reason').val());
				form_data.append('appCatCode', $('#appCatCode').val());

				$.ajax({
					url: urlPath+"ajax/pushCancelCNEFRIS_OtherInvoices.php",
					type: "POST",
					data: form_data,
					contentType: false,
					cache: false,
					processData:false,
					success: function(data){
						$('#pushInvoiceToEFRISStatus').html(data);
						//alert(data);
						var	values = JSON.parse(data);
						$('#pushInvoiceToEFRIS').attr("disabled",false);
						if(values.status == "error"){
							$('#pushInvoiceToEFRISStatus').html(values.error);
						}else{
							$('#pushInvoiceToEFRISStatus').html('Pushed');							
							$('#pushInvoiceToEFRISStatus').append(values.response);
							//location.reload();						
						}
					}
				});
			}else{
				alert("Enter Lot No.");
				$('#pushInvoiceToEFRISStatus').html("");
			}
		});
	</script>
		<?php
	}

        		//echo '<p>'.$this->rgf("payment_information",$oi_pay_template,"t_id","t_description").'</p>';
echo '</div>';
	        $this->end_print();
				
			}
	    

	}
	
	public function checkStatusAction(){
		$id = portion(3);
		///////////////////////////////////////////////////////////////////////////
		$db = new Db();
    	$select = $db->select("SELECT  TOP 1 oi_id as previous FROM cn_other_invoice WHERE oi_id < '$id' ORDER BY oi_id DESC");
    	extract($select[0][0]);
    	$previous;

   
    	$db = new Db();
    	
    	$select = $db->select("SELECT TOP 1  oi_id as next FROM cn_other_invoice WHERE oi_id > '$id' ORDER BY oi_id ASC");
    	extract($select[0][0]);
    	$next;


		/////////////////////////////////////////////////////////////////////////////


		$id = portion(3);
        $db = new Db();
        
        $sql = "SELECT * FROM cn_other_invoice WHERE oi_id = '$id'";
        $select = $db->select($sql);

        if($db->num_rows()){
	        extract($select[0][0]);

	        echo '<br/>';
		echo '<table style="" cellpadding="0" border=1>';
		echo '<tr valign="top"><td><b>Orginal Invoice Id: </b></td><td>'.$oi_efris_cn_ori_invoice_no.'</td></tr>';
		echo '<tr valign="top"><td><b>Credit Note ID: </b></td><td>'.$oi_efris_cn_id.'</td></tr>';
		echo '<tr valign="top"><td><b>Credit Note Status: </b></td><td>'.$oi_status.'</td></tr>';
		echo '</table>';
		echo '<br/>';
	
		if($oi_status==101010){
			echo 'Cancel:';	
		}else{

		echo '<input type="hidden" id="ori_fdn" value="'.$oi_efris_cn_ori_invoice_no.'"/>';
		echo '<input type="hidden" id="cn_id" value="'.$oi_efris_cn_id.'"/>';
		echo '<input type="hidden" id="reason_code" value="101"/>';
		echo '<input type="hidden" id="reason" value=""/>';
		echo '<input type="hidden" id="appCatCode" value="104"/>';

		echo'<button type="button" id="pushInvoiceToEFRIS" name="send" class="btn btn-primary"><i class="fa fa-fw fa-refresh"></i> Check Credit Note Status</button>';
		echo '<span id="pushInvoiceToEFRISStatus"></span>';

		?>
<script type="text/javascript">		
		var urlPath = $('#urlPath').val();
		$('#pushInvoiceToEFRIS').click(function(){
			$('#pushInvoiceToEFRISStatus').html('<img src="'+urlPath+'images/loading.gif" alt="loading..."/>');						
			$(this).attr("disabled", true);
			if(1){
				var form_data = new FormData();		
				
				form_data.append('ori_fdn', $('#ori_fdn').val());
				form_data.append('cn_id', $('#cn_id').val());
				form_data.append('id', $('#id').val());
				form_data.append('reason_code', $('#reason_code').val());
				form_data.append('reason', $('#reason').val());
				form_data.append('appCatCode', $('#appCatCode').val());

				$.ajax({
					url: urlPath+"ajax/check_cn.php",
					type: "POST",
					data: form_data,
					contentType: false,
					cache: false,
					processData:false,
					success: function(data){
						$('#pushInvoiceToEFRISStatus').html(data);
						//alert(data);
						// var	values = JSON.parse(data);
						$('#pushInvoiceToEFRIS').attr("disabled",false);
						// if(values.status == "error"){
						// 	$('#pushInvoiceToEFRISStatus').html(values.error);
						// }else{
						// 	$('#pushInvoiceToEFRISStatus').html('Pushed');							
						// 	$('#pushInvoiceToEFRISStatus').append(values.response);
						// 	//location.reload();						
						// }
					}
				});
			}else{
				alert("Enter Lot No.");
				$('#pushInvoiceToEFRISStatus').html("");
			}
		});
	</script>
		<?php
	}

        		//echo '<p>'.$this->rgf("payment_information",$oi_pay_template,"t_id","t_description").'</p>';
echo '</div>';
	        $this->end_print();
				
			}
	    

	}
	
	
	public function checkStatus2Action(){
		$id = portion(3);
		///////////////////////////////////////////////////////////////////////////
		$db = new Db();
    	$select = $db->select("SELECT  TOP 1 oi_id as previous FROM dn_other_invoice WHERE oi_id < '$id' ORDER BY oi_id DESC");
    	extract($select[0][0]);
    	$previous;

   
    	$db = new Db();
    	
    	$select = $db->select("SELECT TOP 1  oi_id as next FROM dn_other_invoice WHERE oi_id > '$id' ORDER BY oi_id ASC");
    	extract($select[0][0]);
    	$next;


		/////////////////////////////////////////////////////////////////////////////


		$id = portion(3);
        $db = new Db();
        
        $sql = "SELECT * FROM dn_other_invoice WHERE oi_id = '$id'";
        $select = $db->select($sql);

        if($db->num_rows()){
	        extract($select[0][0]);

	        echo '<br/>';
		echo '<table style="" cellpadding="0" border=1>';
		echo '<tr valign="top"><td><b>Orginal Invoice Id: </b></td><td>'.$oi_efris_cn_ori_invoice_no.'</td></tr>';
		echo '<tr valign="top"><td><b>Credit Note ID: </b></td><td>'.$oi_efris_cn_id.'</td></tr>';
		echo '<tr valign="top"><td><b>Credit Note Status: </b></td><td>'.$oi_status.'</td></tr>';
		echo '</table>';
		echo '<br/>';
	
		if($oi_status==101010){
			echo 'Cancel:';	
		}else{

		echo '<input type="hidden" id="ori_fdn" value="'.$oi_efris_cn_ori_invoice_no.'"/>';
		echo '<input type="hidden" id="cn_id" value="'.$oi_efris_cn_id.'"/>';
		echo '<input type="hidden" id="reason_code" value="101"/>';
		echo '<input type="hidden" id="reason" value=""/>';
		echo '<input type="hidden" id="appCatCode" value="104"/>';

		echo'<button type="button" id="pushInvoiceToEFRIS" name="send" class="btn btn-primary"><i class="fa fa-fw fa-refresh"></i> Check Credit Note Status</button>';
		echo '<span id="pushInvoiceToEFRISStatus"></span>';

		?>
<script type="text/javascript">		
		var urlPath = $('#urlPath').val();
		$('#pushInvoiceToEFRIS').click(function(){
			$('#pushInvoiceToEFRISStatus').html('<img src="'+urlPath+'images/loading.gif" alt="loading..."/>');						
			$(this).attr("disabled", true);
			if(1){
				var form_data = new FormData();		
				
				form_data.append('ori_fdn', $('#ori_fdn').val());
				form_data.append('cn_id', $('#cn_id').val());
				form_data.append('id', $('#id').val());
				form_data.append('reason_code', $('#reason_code').val());
				form_data.append('reason', $('#reason').val());
				form_data.append('appCatCode', $('#appCatCode').val());

				$.ajax({
					url: urlPath+"ajax/check_cn.php",
					type: "POST",
					data: form_data,
					contentType: false,
					cache: false,
					processData:false,
					success: function(data){
						$('#pushInvoiceToEFRISStatus').html(data);
						//alert(data);
						// var	values = JSON.parse(data);
						$('#pushInvoiceToEFRIS').attr("disabled",false);
						// if(values.status == "error"){
						// 	$('#pushInvoiceToEFRISStatus').html(values.error);
						// }else{
						// 	$('#pushInvoiceToEFRISStatus').html('Pushed');							
						// 	$('#pushInvoiceToEFRISStatus').append(values.response);
						// 	//location.reload();						
						// }
					}
				});
			}else{
				alert("Enter Lot No.");
				$('#pushInvoiceToEFRISStatus').html("");
			}
		});
	</script>
		<?php
	}

        		//echo '<p>'.$this->rgf("payment_information",$oi_pay_template,"t_id","t_description").'</p>';
echo '</div>';
	        $this->end_print();
				
			}
	    

	}
	
	public function deleteInvoiceAction(){
		$id = portion(3);
		$this->deletor("other_invoice", "oi_id",  $id, 'other-invoices/all-invoices');
	}
	
	public function addInvoiceAction(){
		if(isset($_POST['submit'])){
		
			$category = 1;
			$customer = $_POST['customer'];
			$pay_template = $_POST['pay_template'];
			$title = $_POST['title'];
			$description = $_POST['description'];
			$item = $_POST['item'];
			$qty = $_POST['qty'];
			$unit_price = $_POST['unit_price'];

			$time = time();
			$user = user_id();
			
			$errors = array();
			// if(empty($category)){
			// 	$errors[]="Select Category";
			// }
			
		

		if(empty($errors)){
			$db = new Db();
			$x = $db->insert("other_invoice",["oi_category"=>$category,"oi_customer"=>$customer,"oi_pay_template"=>$pay_template,"oi_title"=>$title,"oi_description"=>$description,"oi_added_by"=>$user,"oi_date_added"=>$time,"oi_item"=>$item,"oi_qty"=>$qty,"oi_unit_price"=>$unit_price]);
			
			if($x){
				FeedBack::success();
				FeedBack::refresh(3, return_url().'other-invoices/add-invoice');
			}else{
				FeedBack::error();
			}
			}else{
				FeedBack::errors($errors);
			}
		}
		?>
		<div class="col-md-12">
			<h3>&nbsp;</h3>
			<div class="col-lg-12">
			<div class="panel panel-default">
				<div class="panel-heading">
					
				</div>
				<div class="panel-body">
					<div class="row">
						<div class="col-lg-12">
							<form role="form" action="" method="post">
								<div class="form-group">
									<div id="must">All field with asterisk(*) are mandatory.</div>
									<div class="row">
										<div class="col-md-2">
										<label>Date<span class="must">*</span></label>
										<div class="form-line">
											<input class="form-control" style="height: 29px;" id="date" type="date" name="" value="<?php echo date('Y-m-d'); ?>">
										</div>
									</div>
									<div class="col-md-2" style="display:none;">
										<label>Category<span class="must">*</span></label>
										<div class="form-line">
											<select id="category" class="form-control">
												<?php
												echo '<option>-----select------</option>';	
												$db = new Db();
												$select = $db->select("SELECT * FROM categories");
												foreach ($select[0] as $row) {
													extract($row);
													if($cat_id == $category)
													echo '<option selected value="'.$cat_id.'">'.$cat_name.'</option>';	
													else
													echo '<option value="'.$cat_id.'">'.$cat_name.'</option>';		
												}

												?>
											</select>
											</div>
									</div>
									<div class="col-md-7">
										<label>Customers<span class="must">*</span></label>
										<?php
												echo'<select  class="form-control" name="customer" id="customer">';
												
											$select = $db->select("SELECT * FROM customers ORDER BY cust_name ASC");
											foreach($select[0] as $row){
												extract($row);
												if($customer == $cust_id)
													echo '<option selected value="'.$cust_id.'"><b>'.$cust_name.' - '.$cust_type.'</b></option>';
												else
													echo '<option value="'.$cust_id.'">'.$cust_name.' - '.$cust_type.'</option>';
											}
											
											
										echo'</select>';
										?>
									</div>
									<script type="text/javascript">
										$(document).ready(function(){
											$('#category').change(function(){
												var category = $(this).val();
												var form_data = new FormData(); 
												form_data.append('category', category); 
												//alert(category);
												$.ajax({
										        	url: '<?php echo return_url().'ajax/invoice_details_by_category.php'; ?>',
										            type: "POST",
										            data: form_data,
										            contentType: false,
										            cache: false,
										            processData:false,
										            success: function(data){
										            	//alert(data);
										            	$('#customer').html(data);	
										            }
										        });
											});
										});
									</script>
									<div class="col-md-3">
										<label>Payment Information<span class="must">*</span></label>
										<div class="form-line">
											<select class="form-control" id="pay_template" name="pay_template">
												<?php
												$sql = "SELECT * FROM payment_information";
												$db = new Db();
												$select = $db->query($sql);
												foreach($select as $row){
													extract($row);
													if($t_id == $template)
														echo '<option selected value="'.$t_id.'">'.$t_name.'</option>';
													else
														echo '<option value="'.$t_id.'">'.$t_name.'</option>';
												}
												
												?>
											</select>
										</div>
									</div>
									<div class="clearfix"></div>
									<div class="col-md-2">
										<label>Currency<span class="must">*</span></label>
										<div class="form-line">
											<select class="form-control" id="currency" name="currency">
												<?php
												$db = new Db();
												$sql = "SELECT * FROM dictionary WHERE d_category = '1' ORDER BY d_order ASC";
												$select = $db->select($sql);
												foreach($select[0] as $row){
													extract($row);
													echo '<option value="'.$d_code.'">'.$d_name.' ('.$d_code.')</option>';
												}
												
												?>
											</select>
										</div>
									</div>
									<div class="col-md-4">
										<label>VAT<span class="must">*</span></label>
										<div class="form-line">
											<select class="form-control" id="vat" name="currency">
												<?php
												

												foreach($this->VATCAT() as $row=>$x){
													extract($row);
													if($t_id == $template)
														echo '<option selected value="'.$row.'">'.$x[0].'</option>';
													else
														echo '<option value="'.$row.'">'.$x[0].'</option>';
												}
												
												?>
											</select>
										</div>
									</div>

									<div class="col-md-2">
										<label>Exchange Rate<span class="must">*</span></label>
										<div class="form-line">
											<input type="text" class="form-control" step="any" value="" id="exchange" name="exchange"/>
										</div>
									</div>
									
									<script type="text/javascript">
									$(document).ready(function(){
										var urlPath = $('#urlPath').val();
										
										$('#exchange').each(function(){
											var id = $(this).val();
											$(this).html('checking...');
											var form_data = new FormData();
											form_data.append('id', $('#date').val());
											form_data.append('vat', $('#vat').val());
											//alert(id);
											$.ajax({
												url: urlPath+"ajax/good_t126_dollar.php",
												type: "POST",
												data: form_data,
												contentType: false,
												cache: false,
												processData:false,
												success: function(data){
													var	values = JSON.parse(data);
													//$('#pushInvoiceToEFRIS').attr("disabled",false);
													if(values.status == "error"){
														$('#exchange').val(values.error);
													}else{
														$('#exchange').val('<div>'+values.response+'</div>');	
														$("#exchange").val(values.returned);				
													}
												}		
											});
										});

										$('#vat').change(function(){
											var id = $(this).val();
											$('#exchange').val('checking...');
											var form_data = new FormData();
											form_data.append('id', $('#date').val());
											form_data.append('vat', $('#vat').val());
											//alert(id);
											$.ajax({
												url: urlPath+"ajax/good_t126_dollar.php",
												type: "POST",
												data: form_data,
												contentType: false,
												cache: false,
												processData:false,
												success: function(data){
													var	values = JSON.parse(data);
													//$('#pushInvoiceToEFRIS').attr("disabled",false);
													if(values.status == "error"){
														$('#exchange').val(values.error);
													}else{
														$('#exchange').val('<div>'+values.response+'</div>');	
														$("#exchange").val(values.returned);				
													}
												}		
											});
										});

										$('#vat').change(function(){
											var id = $(this).val();
											$("#exchange").val('checking...');
											var form_data = new FormData();
											form_data.append('id', $('#date').val());
											form_data.append('vat', $('#vat').val());
											//alert(id);
											$.ajax({
												url: urlPath+"ajax/good_t126_dollar.php",
												type: "POST",
												data: form_data,
												contentType: false,
												cache: false,
												processData:false,
												success: function(data){
													var	values = JSON.parse(data);
													//$('#pushInvoiceToEFRIS').attr("disabled",false);
													if(values.status == "error"){
														$('#exchange').val(values.error);
													}else{
														$('#exchange').val('<div>'+values.response+'</div>');	
														$("#exchange").val(values.returned);				
													}
												}		
											});
										});
									});
									</script>
									<div class="col-md-4">
										<label>Invoice number / Reference No.<span class="must">*</span></label>
										<div class="form-line">
											<input type="text" id="ref_no" class="form-control" name="">
										</div>
									</div>
									<div class="col-md-12">
										<label>Remarks<span class="must">*</span></label>
										<div class="form-line">
											<textarea id="general_description" class="form-control"></textarea>
										</div>
									</div>
									
								</div>
								<div class="row">
									<div class="col-md-12">
										<label>Items<span class="must">*</span></label>
										<style type="text/css">
											.added-section:hover{
												background-color: red;
											}
										</style>
										<div id="worddescription">
											<div class="row">
												<div class="col-md-1" style="font-weight:bold;color:red;">
													#
												</div>
												<div class="col-md-6" style="font-weight:bold;color:red;">
													Goods
												</div>
												<div class="col-md-2" style="font-weight:bold;color:red;">
													Qty
												</div>
												<div class="col-md-2" style="font-weight:bold;color:red;">
													UnitPrice (VAT Inc)
												</div>
											</div>
											<div class="row section">
												<div class="added-section" id="1">
													<div class="col-md-1">
														1.
													</div>
													<div class="col-md-6">
														<!-- <textarea autofocus class="form-control" id="description1"></textarea> -->
														<?php
														$db = new Db();
															echo'<select class="form-control" id="description1">';
															echo'<option value="">-------select------</option>';
															$select = $db->select("SELECT * from efris_goods ORDER BY eg_name ASC");
															foreach ($select[0] as $row) {
																extract($row);
																if ($eg_id == $description)
																	echo'<option selected value="'.$eg_id.'">'.$eg_name.'('.$eg_code.')</option>';
																else
																echo'<option value="'.$eg_id.'">'.$eg_name.'('.$eg_code.')</option>';	
															}
															echo '</select>';
														?>
													</div>
													<div class="col-md-2">
														<input autocomplete="off" type="number" value="" id="quantity1" class="form-control"/>
													</div>
													<div class="col-md-2">
														<input autocomplete="off" type="text" min=0 value="" id="amount1" class="form-control"/>
													</div>
												</div>
												<div class="clearfix"></div>
											</div>
											<div class="col-md-12">
												<button type="button" id="plus-wd" class="btn btn-xs pull-right text-primary"><i class="fa fa-fw fa-plus"></i></button>
											</div>
										</div>
										<script type="text/javascript">
											$(function(){
												$(document).off('click','#plus-wd').on('click','#plus-wd',function(e){
													var random = Date.now();
													var html = '<div class="clearfix"></div>';
													html += '';
													html += '<div class="added-section" id="'+random+'">';
													html += '<div class="col-md-1 line-number"></div>';
													html += '<div class="col-md-6">';
													html += '<select class="form-control" id="description'+random+'" style=""><?php
														
														$db = new Db();
															echo'<option value="">-------select------</option>';
															$select = $db->select("SELECT * from efris_goods ORDER BY eg_name ASC");
															foreach ($select[0] as $row) {
																extract($row);
																if ($eg_id == $description)
																	echo'<option selected value="'.$eg_id.'">'.$eg_name.'('.$eg_code.')</option>';
																else
																echo'<option value="'.$eg_id.'">'.$eg_name.'('.$eg_code.')</option>';	
															}
															echo '</select>';
														
														?>';
													html += '</div>';
													html += '<div class="col-md-2">';
													html += '<input autocomplete="off" type="number" value="" id="quantity'+random+'" class="form-control"/>';
													html += '</div>';
													html += '<div class="col-md-2">';
													html += '<input autocomplete="off" type="text" value="" id="amount'+random+'" class="form-control"/>';
													html += '</div>';
													html += '<div class="col-md-1"><button type="button" class="text-danger" id="remove-wd"><i class="fa fa-fw fa-times"></i></button></div>';
													html += '</div>';
													html += '</div>';

													$('#worddescription .section').append(html);
													$('#description'+random).focus();

													var count = 2;
													$('.line-number').each(function(){
														$(this).html(count+'. ');
														count++;
													});

												});

												$(document).off('click','#remove-wd').on('click','#remove-wd',function(e){
													var parent = $(this).parent().parent();
													parent.remove();

													var count = 2;
													$('.line-number').each(function(){
														$(this).html(count+'. ');
														count++;
													});
												});


											});
										</script>
									</div>

								</div>	
									
								<div class="col-md-12">
									<br>
									<button id="addOtherInvoiceBtn" type="button" name="submit" class="btn btn-primary"><i class="fa fa-fw fa-save"></i> Save</button><span id="addOtherInvoiceStatus"></span>
								</div>	
								</div>

								</div>
								
								
							</form>
						</div>
					</div>
				</div>
			</div>
		</div>
		<script type="text/javascript">
			$(document).ready(function(){
	$(document).off('click','#addOtherInvoiceBtn').on('click', '#addOtherInvoiceBtn', function(e){

		var date = $('#date').val();
		//var category = $('#category').val();
		var customer  = $('#customer').val();
		var pay_template  = $('#pay_template').val();
		var currency  = $('#currency').val();
		var general_description  = $('#general_description').val();
		var vat  = $('#vat').val();
		var ref_no  = $('#ref_no').val();
		var exchange  = $('#exchange').val();

        var counter=1;
		var counterArray = new Array();
		$('#worddescription .section').find(".added-section").each(function(){
			var findID = $(this).attr('id');
			//var id = findID.split("_")[1];
			//id = (!id)?"":"_"+id;
			counterArray.push(findID);
			//alert(findID);
		});


		var errors = new Array();
		if(!customer){
			errors.push("Please Select a Customer");
		}
		if(ref_no==""){
			errors.push("Please Enter Reference Number.");
		}

		for(i=0; i<counterArray.length; i++){				
			var description1 = $('#description'+counterArray[i]).val();
			var quantity1 = $('#quantity'+counterArray[i]).val();				
			var amount1 = parseInt($('#amount'+counterArray[i]).val());

			if(!description1){
				errors.push("Line "+(i+1)+": Select Item");
			}
			if(!quantity1){
				errors.push("Line "+(i+1)+": Enter Quantity");
			}
			if(!amount1){
				errors.push("Line "+(i+1)+": Enter Amount");
			}
		}

		
		
		if(errors.length >= 1){
			alert("Please Review the following:\n "+errors.join("\n"));
		}else{
			//$('#addOtherInvoiceStatus').html('<img src="'+urlPath+'images/loading.gif" alt="loading..."/>');
			$(this).html("Add Invoice");
			$(this).attr("disabled", true);
			var form_data = new FormData();
			form_data.append('date', date);
			form_data.append('category', 1);
			form_data.append('customer', customer);
			form_data.append('pay_template', pay_template);
			form_data.append('currency', currency);
			form_data.append('general_description', general_description);
			form_data.append('vat', vat);
			form_data.append('ref_no', ref_no);
			form_data.append('exchange', exchange);

			
			 form_data.append('totalItems', counterArray.length);
			for(i=0; i<counterArray.length; i++){
				form_data.append('description'+i, $('#description'+counterArray[i]).val());
				form_data.append('quantity'+i, $('#quantity'+counterArray[i]).val());
				form_data.append('amount'+i, $('#amount'+counterArray[i]).val());
			}

			$.ajax({
			//url: urlPath2+"ajax/addOtherInvoice.php",
			url: '<?php echo return_url().'ajax/addOtherInvoice.php'; ?>',
			type: "POST",
			data: form_data,
			contentType: false,
			cache: false,
			processData:false,
			success: function(data){
				//alert(data);
				var	values = JSON.parse(data);
				$('#addOtherInvoiceStatus').html('Successfully Saved');
				location.replace($('#urlPath').val()+"other-invoices/view-invoice/"+values.id);
			}
		
		});	
		}

	});


});
		</script>
	<?php
	}
	public function editInvoiceAction(){
		$id=portion(3);
		$db=new Db();
		$select=$db->select("select * from other_invoice WHERE oi_id='$id'");
		if($db->num_rows()){
		extract($select[0][0]);
		//print_r($select);
		}
		if(isset($_POST['submit'])){
		
			$category = 1;//$_POST['category'];
			$customer = $_POST['customer'];
			$pay_template = $_POST['pay_template'];
			$title = $_POST['title'];
			$description = $_POST['description'];

			$time = time();
			$user = user_id();
			
			$errors = array();
			// if(empty($category)){
			// 	$errors[]="Select Category";
			// }
			
		

		if(empty($errors)){
			$db = new Db();
			$x = $db->update("other_invoice",["oi_category"=>$category,"oi_customer"=>$customer,"oi_pay_template"=>$pay_template,"oi_title"=>$title,"oi_description"=>$description,"oi_added_by"=>$user,"oi_date_added"=>$time],["oi_id"=>$id]);
			
			if($x){
				FeedBack::success();
				FeedBack::refresh(3, return_url().'other-invoices/edit-invoice/'.$id);
			}else{
				FeedBack::error();
			}
			}else{
				FeedBack::errors($errors);
			}
		}
		?>
		<div class="col-md-12">
			<h3>&nbsp;</h3>
			<div class="col-lg-12">
			<div class="panel panel-default">
				<div class="panel-heading">
					
				</div>
				<div class="panel-body">
					<div class="row">
						<div class="col-lg-12">
							<form role="form" action="" method="post">
								<div class="form-group">
									<div id="must">All field with asterisk(*) are mandatory.</div>
										<div class="row">
										<div class="col-md-2">
										<label>Date<span class="must">*</span></label>
										<div class="form-line">
											<input class="form-control" style="height: 29px;" id="date" type="date" name="" value="<?php echo date('Y-m-d',$oi_date); ?>">
										</div>
									</div>
									<div class="col-md-2" style="display:none;">
										<label>Category<span class="must">*</span></label>
										<div class="form-line">
											<select id="category" class="form-control">
												<?php
												echo '<option>-----select------</option>';	
												$db = new Db();
												$select = $db->select("SELECT * FROM categories");
												foreach ($select[0] as $row) {
													extract($row);
													if($cat_id == $oi_category)
													echo '<option selected value="'.$cat_id.'">'.$cat_name.'</option>';	
													else
													echo '<option value="'.$cat_id.'">'.$cat_name.'</option>';		
												}

												?>
											</select>
											</div>
									</div>
									<div class="col-md-7">
										<label>Customers<span class="must">*</span></label>
										<?php
												echo'<select  class="form-control" name="customer" id="customer">';
											$select = $db->select("SELECT * FROM customers ORDER BY cust_name ASC");
											foreach($select[0] as $row){
												extract($row);
												if($oi_customer == $cust_id)
													echo '<option selected value="'.$cust_id.'"><b>'.$cust_name.'</b></option>';
												else
													echo '<option value="'.$cust_id.'">'.$cust_name.'</option>';
											}
											
										echo'</select>';
										?>
									</div>
									<script type="text/javascript">
										$(document).ready(function(){
											$('#category').change(function(){
												var category = 1;//$(this).val();
												var form_data = new FormData(); 
												form_data.append('category', category); 

												$.ajax({
										        	url: '<?php echo return_url().'ajax/invoice_details_by_category.php'; ?>',
										            type: "POST",
										            data: form_data,
										            contentType: false,
										            cache: false,
										            processData:false,
										            success: function(data){
										            	//alert(data);
										            	$('#customer').html(data);	
										            }
										        });
											});
										});
									</script>
									<div class="col-md-3">
										<label>Payment Template<span class="must">*</span></label>
										<div class="form-line">
											<select class="form-control" id="pay_template" name="pay_template">
												<?php
												$sql = "SELECT * FROM payment_information";
												$db = new Db();
												$select = $db->query($sql);
												foreach($select as $row){
													extract($row);
													if($t_id == $oi_template)
														echo '<option selected value="'.$t_id.'">'.$t_name.'</option>';
													else
														echo '<option value="'.$t_id.'">'.$t_name.'</option>';
												}
												
												?>
											</select>
										</div>
									</div>
									
									<div class="clearfix"></div>
									<div class="col-md-2">
										<label>Currency<span class="must">*</span></label>
										<div class="form-line">
											<select class="form-control" id="currency" name="currency">
												<?php
												$db = new Db();
												$sql = "SELECT * FROM dictionary WHERE d_category = '1' ORDER BY d_order ASC";
												$select = $db->select($sql);
												foreach($select[0] as $row){
													extract($row);
													if($oi_currency == $d_code)
													echo '<option selected value="'.$d_code.'">'.$d_name.' ('.$d_code.')</option>';
												else
													echo '<option value="'.$d_code.'">'.$d_name.' ('.$d_code.')</option>';
												}
												
												?>
											</select>
										</div>
									</div>
									<div class="col-md-4">
										<label>VAT<span class="must">*</span></label>
										<div class="form-line">
											<select class="form-control" id="vat" name="currency">
												<?php
												

												foreach($this->VATCAT() as $row=>$x){
													extract($row);
													if($row == $oi_vat)
														echo '<option selected value="'.$row.'">'.$x[0].'</option>';
													else
														echo '<option value="'.$row.'">'.$x[0].'</option>';
												}
												
												?>
											</select>
										</div>
									</div>

									<div class="col-md-2">
										<label>Exchange Rate<span class="must">*</span></label>
										<div class="form-line">
											<input type="text" class="form-control" step="any" value="" id="exchange" name="exchange"/>
										</div>
									</div>
									<script type="text/javascript">
									$(document).ready(function(){
										var urlPath = $('#urlPath').val();
										
										$('#date').change(function(){
											var date = $(this).val();
											$(this).html('checking...');
											var form_data = new FormData();
											form_data.append('id', $('#date').val());
											//alert(id);
											$.ajax({
												url: urlPath+"ajax/good_t126_dollar.php",
												type: "POST",
												data: form_data,
												contentType: false,
												cache: false,
												processData:false,
												success: function(data){
													var	values = JSON.parse(data);
													//$('#pushInvoiceToEFRIS').attr("disabled",false);
													if(values.status == "error"){
														$('#exchange').val(values.error);
													}else{
														$('#exchange').val('<div>'+values.response+'</div>');	
														$("#exchange").val(values.returned);				
													}
												}		
											});
										});
										$('#exchange').each(function(){
											var id = $(this).val();
											$(this).html('checking...');
											var form_data = new FormData();
											form_data.append('id', $('#date').val());
											//alert(id);
											$.ajax({
												url: urlPath+"ajax/good_t126_dollar.php",
												type: "POST",
												data: form_data,
												contentType: false,
												cache: false,
												processData:false,
												success: function(data){
													var	values = JSON.parse(data);
													//$('#pushInvoiceToEFRIS').attr("disabled",false);
													if(values.status == "error"){
														$('#exchange').val(values.error);
													}else{
														$('#exchange').val('<div>'+values.response+'</div>');	
														$("#exchange").val(values.returned);				
													}
												}		
											});
										});
									});
									</script>
									
									<div class="col-md-4">
										<label>Reference No.<span class="must">*</span></label>
										<div class="form-line">
											<input type="text" value="<?php echo $oi_ref_no;?>" id="ref_no" class="form-control" name="">
										</div>
									</div>
									<div class="col-md-12">
										<label>Remarks<span class="must">*</span></label>
										<div class="form-line">
											<textarea id="general_description" class="form-control"><?php echo $oi_general_description;?></textarea>
										</div>
									</div>
									
								</div>
								<div class="row">
							<?php 
								$desc1 = "";
								$amount1 = 0;
								$sql = "SELECT * FROM invoice_item_description WHERE ii_rel = '$oi_rel' ORDER BY ii_id ASC";
								$select = $db->select($sql);
								if($db->num_rows())
								extract($select[0][0]);
							?>
							<div class="col-md-12">
								<label>Item<span class="must">*</span></label>
								<style type="text/css">
									.added-section:hover{
										background-color: red;
									}
								</style>
								<div id="worddescription">
									<div class="row">
										<div class="col-md-1" style="font-weight:bold;color:red;">
											#
										</div>
										<div class="col-md-6" style="font-weight:bold;color:red;">
											Details
										</div>
										<div class="col-md-2" style="font-weight:bold;color:red;">
											Qty
										</div>
										<div class="col-md-2" style="font-weight:bold;color:red;">
											UnitPrice <i>(VAT Inc)</i>
										</div>
									</div>
									<div class="row section">
										<div class="added-section" id="1">
											<div class="col-md-1">
												1.
											</div>
											<div class="col-md-6">
												<?php
													$db = new Db();
														echo'<select class="form-control" id="description1">';
														echo'<option value="">-------select------</option>';
														$select = $db->select("SELECT * from efris_goods ORDER BY eg_name ASC");
														foreach ($select[0] as $row) {
															extract($row);
															if ($eg_id == $ii_description)
																echo'<option selected value="'.$eg_id.'">'.$eg_name.'('.$eg_code.')</option>';
															else
															echo'<option value="'.$eg_id.'">'.$eg_name.'('.$eg_code.')</option>';	
														}
														echo '</select>';
													?>
											</div>
											<div class="col-md-2">
												<input autocomplete="off" type="number" value="<?php echo $ii_quantity;?>" id="quantity1" class="form-control"/>
											</div>
											<div class="col-md-2">
												<input autocomplete="off" type="text" min=0 value="<?php echo $ii_amount;?>" id="amount1" class="form-control"/>
											</div>
										</div>
										<?php 
											$db = new Db();
											$sql = "SELECT * FROM invoice_item_description WHERE ii_rel = '$oi_rel' AND ii_id != '$ii_id' ORDER BY ii_id ASC";
											$select = $db->select($sql);
											//print_r($select);
											$x=1;
											foreach($select[0] as $row){
												extract($row);
												$x++;
												echo '<div class="clearfix"></div>';
												echo '<div class="added-section" id="'.$x.'">';
												echo '<div class="col-md-1 line-number" >'.$x.'.</div>';
												echo '<div class="col-md-6">';
												// <textarea autofocus class="form-control" id="description'.$x.'">'.$ii_description.'</textarea>

													$db = new Db();
														$db = new Db();
															echo'<select class="form-control" id="description'.$x.'">';
															echo'<option value="">-------select------</option>';
															$select = $db->select("SELECT * from efris_goods ORDER BY eg_name ASC");
															foreach ($select[0] as $row) {
																extract($row);
																if ($eg_id == $ii_description)
																	echo'<option selected value="'.$eg_id.'">'.$eg_name.'('.$eg_code.')</option>';
																else
																echo'<option value="'.$eg_id.'">'.$eg_name.'('.$eg_code.')</option>';	
															}
															echo '</select>';
														
													
												echo'</div>';
												echo '<div class="col-md-2"><input autocomplete="off" type="number" value="'.$ii_quantity.'" id="quantity'.$x.'" class="form-control"/></div>';
												echo '<div class="col-md-2"><input autocomplete="off" type="text" value="'.number_format($ii_amount).'" id="amount'.$x.'" class="form-control"/></div>';
												echo '<div class="col-md-2"><button type="button" class="text-danger" id="remove-wd"><i class="fa fa-fw fa-times"></i></button></div>';
												echo '</div>';

							
											}
										?>
									</div>
									<div class="col-md-12">
										<button type="button" id="plus-wd" class="btn btn-xs pull-right text-primary"><i class="fa fa-fw fa-plus"></i></button>
									</div>
								</div>
								<script type="text/javascript">
									$(function(){
										$(document).off('click','#plus-wd').on('click','#plus-wd',function(e){
											var random = Date.now();
											var html = '<div class="clearfix"></div>';
											html += '<div class="added-section" id="'+random+'">';
											html += '<div class="col-md-1 line-number"></div>';
											html += '<div class="col-md-6">';
											html += '<?php
													
													echo '<select class="form-control" id="description'; ?>'+random+'<?php echo '" style="">';
														
													$db = new Db();
														
														echo'<option value="">-------select------</option>';
														$select = $db->select("SELECT * from efris_goods ORDER BY eg_name ASC");
														foreach ($select[0] as $row) {
															extract($row);
															if ($eg_id == $ii_description)
																echo'<option selected value="'.$eg_id.'">'.$eg_name.'('.$eg_code.')</option>';
															else
															echo'<option value="'.$eg_id.'">'.$eg_name.'('.$eg_code.')</option>';	
														}
														echo '</select>';
														
													?>';
											html += '</div>';
											html += '<div class="col-md-2">';
											html += '<input autocomplete="off" type="number" value="" id="quantity'+random+'" class="form-control"/>';
											html += '</div>';
											html += '<div class="col-md-2">';
											html += '<input autocomplete="off" type="text" value="" id="amount'+random+'" class="form-control"/>';
											html += '</div>';
											html += '<div class="col-md-1"><button type="button" class="text-danger" id="remove-wd"><i class="fa fa-fw fa-times"></i></button></div>';
											html += '</div>';
											html += '</div>';

											$('#worddescription .section').append(html);
											$('#description'+random).focus();

											var count = 2;
											$('.line-number').each(function(){
												$(this).html(count+'. ');
												count++;
											});

										});

										$(document).off('click','#remove-wd').on('click','#remove-wd',function(e){
											var parent = $(this).parent().parent();
											parent.remove();

											var count = 2;
											$('.line-number').each(function(){
												$(this).html(count+'. ');
												count++;
											});
										});


									});
								</script>
							</div>

						</div>
										<br>
										<input type="hidden" value="<?php echo$oi_id?>" id="id" name="">
										<button id="editOtherInvoiceBtn" type="button" name="submit" class="btn btn-primary"><i class="fa fa-fw fa-save"></i> Save</button><span id="editOtherInvoiceStatus"></span>
									</div>

								</div>
								
								
							</form>
						</div>
					</div>
				</div>
			</div>
		</div>
			<script type="text/javascript">
			$(document).ready(function(){
	$(document).off('click','#editOtherInvoiceBtn').on('click', '#editOtherInvoiceBtn', function(e){

		var date = $('#date').val();
		var category = 1;//$('#category').val();
		var customer  = $('#customer').val();
		var pay_template  = $('#pay_template').val();
		var currency  = $('#currency').val();
		var vat  = $('#vat').val();
		var general_description  = $('#general_description').val();
		var ref_no  = $('#ref_no').val();
		var exchange  = $('#exchange').val();
		var id  = $('#id').val();

        var counter=1;
		var counterArray = new Array();
		$('#worddescription .section').find(".added-section").each(function(){
			var findID = $(this).attr('id');
			//var id = findID.split("_")[1];
			//id = (!id)?"":"_"+id;
			counterArray.push(findID);
			//alert(findID);
		});


		var errors = new Array();
		if(!customer){
			errors.push("Please Select Customer");
		}
		if(ref_no==""){
			errors.push("Please Enter Reference Number");
		}

		for(i=0; i<counterArray.length; i++){				
			var description1 = $('#description'+counterArray[i]).val();
			var quantity1 = $('#quantity'+counterArray[i]).val();				
			var amount1 = parseInt($('#amount'+counterArray[i]).val());

			if(!description1){
				errors.push("Line "+(i+1)+": Select Item");
			}
			if(!quantity1){
				errors.push("Line "+(i+1)+": Enter quantity");
			}
			if(!amount1){
				errors.push("Line "+(i+1)+": Enter Amount");
			}

			//alert(description1);
		}

//return 0;
		
		
		if(errors.length >= 1){
			alert("Please Review the following:\n "+errors.join("\n"));
		}else{
			//$('#addOtherInvoiceStatus').html('<img src="'+urlPath+'images/loading.gif" alt="loading..."/>');
			$(this).html("Add Invoice");
			$(this).attr("disabled", true);
			var form_data = new FormData();
			form_data.append('date', date);
			form_data.append('category', 1);
			form_data.append('customer', customer);
			form_data.append('pay_template', pay_template);
			form_data.append('currency', currency);
			form_data.append('vat', vat);
			form_data.append('ref_no', ref_no);
			form_data.append('general_description', general_description);
			form_data.append('exchange', exchange);

			
			 form_data.append('totalItems', counterArray.length);
			for(i=0; i<counterArray.length; i++){
				form_data.append('description'+i, $('#description'+counterArray[i]).val());
				form_data.append('quantity'+i, $('#quantity'+counterArray[i]).val());
				form_data.append('amount'+i, $('#amount'+counterArray[i]).val());
				form_data.append('id', id);
			}

			$.ajax({
			//url: urlPath2+"ajax/addOtherInvoice.php",
			url: '<?php echo return_url().'ajax/editOtherInvoice.php'; ?>',
			type: "POST",
			data: form_data,
			contentType: false,
			cache: false,
			processData:false,
			success: function(data){
				var	values = JSON.parse(data);
				$('#addOtherInvoiceStatus').html('Successfully Saved');
				location.replace($('#urlPath').val()+"other-invoices/view-invoice/"+values.id);
			}
		
		});	
		}

	});


});
		</script>
	<?php
	}

	public function pending(){
		$db = new Db();
		$sql = "SELECT * FROM other_invoice WHERE oi_fdn IS NULL";
		$select = $db->select($sql);
		return $db->num_rows();
	}

	public function pending2(){
		$db = new Db();
		$sql = "SELECT * FROM cn_other_invoice WHERE oi_efris_cn_id IS NULL";
		$select = $db->select($sql);
		return $db->num_rows();
	}

	public function pending3(){
		$db = new Db();
		$sql = "SELECT * FROM dn_other_invoice WHERE oi_efris_cn_id IS NULL";
		$select = $db->select($sql);
		return $db->num_rows();
	}

	public function pending4(){
		$db = new Db();
		$sql = "SELECT * FROM cn_other_invoice WHERE oi_cn_qr_code IS NULL AND oi_status IS NOT NULL";
		$select = $db->select($sql);
		return $db->num_rows();
	}
	
	public function allInvoicesAction(){
	$access = new AccessRights();
	?>
		<div class="col-md-12">
			
			<?php 
			$db = new Db();
			$select = $db->select("SELECT * FROM other_invoice ORDER BY oi_date DESC");
			
			if(!$select){
				$db->error();
			}else{
				echo '<table cellspacing="0" id="table" cellpadding="2"  style="font-size:8px!important;" border="1" width="100%">';
				echo '<thead>';
				echo '<tr>';
				echo '<th width="30px">No.</th>';
				echo '<th>Date</th>';
				//echo '<th>Category</th>';
				echo '<th>Customer</th>';
				echo '<th>VAT Type</th>';
				echo '<th>Ref No.</th>';
				echo '<th>FDN</th>';
				//echo '<th>Title</th>';

				if($access->sectionAccess(user_id(), $this->page, 'E') || $access->sectionAccess(user_id(), $this->page, 'D')){
					echo '<th width="">Action</th>';
				}
				echo '</tr>';
				echo '</thead>';
				
				$i=1;
				echo '<tbody>';
				foreach($select[0] as $row){
					extract($row);
					echo '<tr>';
					echo '<td><center>'.($i++).'.</center></td>';
					echo '<td>'.FeedBack::date_s($oi_date).'</td>';
					//echo '<td>'.$this->rgf("categories",$oi_category,"cat_id","cat_name").'</td>';
					echo '<td>';
					if(!empty($this->rgf("customers",$oi_customer,"cust_id","cust_name"))){
						echo $this->rgf("customers",$oi_customer,"cust_id","cust_name");
					}elseif(!empty($this->rgf("of_customer",$oi_customer,"ofc_id","ofc_customer_name"))){
					    echo $this->rgf("of_customer",$oi_customer,"ofc_id","ofc_customer_name");
					    //
					}else{
						echo $this->rgf("customer",$oi_customer,"customer_id","customer_full_name");
					}
					echo'</td>';
					echo '<td>'.$this->VATCAT($oi_vat, 2).'</td>';
					echo '<td>'.$oi_ref_no.'</td>';
					echo '<td>'.$oi_fdn.'</td>';
					// echo '<td>'.$oi_title.'</td>';
					
					if($access->sectionAccess(user_id(), $this->page, 'E') || $access->sectionAccess(user_id(), $this->page, 'D')){
					
						echo '<td>';
						if(!$oi_fdn){
							if($access->sectionAccess(user_id(), $this->page, 'E'))
							echo $this->action('edit','other-invoices/edit-invoice/'.$oi_id, 'Edit');
							if($access->sectionAccess(user_id(), $this->page, 'D'))
							//echo $this->action('delete','other-invoices/delete-invoice/'.$oi_id, 'Delete');
						echo '<a class="btn btn-xs btn-danger" onclick="return confirm(\'Do you want to Delete this Invoice?\');" href="'.return_url().'/other-invoices/delete-invoice/'.$oi_id.'"><i class="fa fa-fw fa-trash"></i> Delete</a>';
						}else{	
							$k = $this->rgf("cn_other_invoice", $oi_id, "oi_idid", "oi_id");
							$k2 = $this->rgf("dn_other_invoice", $oi_invoice_id, "oi_efris_id", "oi_id");
							if($k){
								echo '<a style="margin:auto 5px;" class="btn btn-xs btn-secondary" href="'.return_url().'other-invoices/view-credit-note/'.$k.'">View CN</a>';
							}else if($k2){
								echo '<a style="margin:auto 5px;" class="btn btn-xs btn-secondary" href="'.return_url().'other-invoices/view-debt-note/'.$k2.'">View DN</a>';
							}else{
								echo '<a style="margin:auto 5px;" class="btn btn-xs btn-info" href="'.return_url().'other-invoices/credit-note/'.$oi_id.'">Credit Note</a>';
								echo '<a style="margin:auto 5px;" class="btn btn-xs btn-primary" href="'.return_url().'other-invoices/debt-note/'.$oi_id.'">Debt Note</a>';
							}

										
						}
						echo"&nbsp;<a class='btn btn-default btn-xs' href=".return_url().'other-invoices/view-invoice/'.$oi_id.">View</a> ";
						echo '</td>';
					}
					echo '</tr>';
					
					
				}
				echo '</tbody>';
				
				echo '</table>';
				
			
				
			}
			
			?>
		</div>
		<?php
	}

	
	public function allCreditNotesAction(){
	$access = new AccessRights();
	?>
		<div class="col-md-12">
			
			<?php 
			$db = new Db();
			$select = $db->select("SELECT * FROM cn_other_invoice ORDER BY oi_date_added DESC");
			
			if(!$select){
				$db->error();
			}else{
				echo '<table cellspacing="0" cellpadding="2" border="1" width="100%" id="table">';
				echo '<thead>';
				echo '<tr>';
				echo '<th width="30px">No.</th>';
				echo '<th>Date</th>';
				//echo '<th>Category</th>';
				echo '<th>Customer</th>';
				echo '<th>CN No.</th>';
				echo '<th>Credit Note ID</th>';
				echo '<th>Status</th>';

				if($access->sectionAccess(user_id(), $this->page, 'E') || $access->sectionAccess(user_id(), $this->page, 'D')){
					echo '<th width="">Action</th>';
				}
				echo '</tr>';
				echo '</thead>';
				
				$i=1;
				echo '<tbody>';
				foreach($select[0] as $row){
					extract($row);
					echo '<tr>';
					echo '<td><center>'.($i++).'.</center></td>';
					echo '<td>'.FeedBack::date_s($oi_date).'</td>';
					//echo '<td>'.$this->rgf("categories",$oi_category,"cat_id","cat_name").'</td>';
					echo '<td>'.$this->rgf("customers",$oi_customer,"cust_id","cust_name").'</td>';
					echo '<td>'.$oi_cn_fdn.'</td>';
					echo '<td>'.$oi_efris_cn_id.'</td>';
					echo '<td>'.$oi_status.'</td>';
					
					if($access->sectionAccess(user_id(), $this->page, 'E') || $access->sectionAccess(user_id(), $this->page, 'D')){
					
						echo '<td>';
						if(!$oi_efris_cn_id){
							if($access->sectionAccess(user_id(), $this->page, 'E'))
							echo $this->action('edit','other-invoices/edit-credit-note/'.$oi_id, 'Edit');
							if($access->sectionAccess(user_id(), $this->page, 'D'))
							//echo $this->action('delete','other-invoices/delete-credit-note/'.$oi_id, 'Delete');
							echo '<a class="btn btn-xs btn-danger" onclick="return confirm(\'Do you want to Delete this Credit Note?\');" href="'.return_url().'/other-invoices/delete-credit-note/'.$oi_id.'"><i class="fa fa-fw fa-trash"></i> Delete</a>';
						}else{	
							if(!$oi_cn_fdn)
							echo '<a style="margin:auto 5px;" class="btn btn-xs btn-primary" href="'.return_url().'other-invoices/cancel-credit-note/'.$oi_id.'">Cancel CN</a>';			
						}
						echo"&nbsp;<a class='btn btn-default btn-xs' href=".return_url().'other-invoices/view-credit-note/'.$oi_id.">View</a> ";
						echo '</td>';
					}
					echo '</tr>';
					
					
				}
				echo '</tbody>';
				
				echo '</table>';
				
			
				
			}
			
			?>
		</div>
		<?php
	}
public function deleteCreditNoteAction(){
		$id = portion(3);
		$this->deletor("cn_other_invoice", "oi_id",  $id, 'other-invoices/all-credit-notes');
	}
	
	public function allDebtNotesAction(){
	$access = new AccessRights();
	?>
		<div class="col-md-12">
			
			<?php 
			$db = new Db();
			$select = $db->select("SELECT * FROM dn_other_invoice ORDER BY oi_date_added DESC");
			
			if(!$select){
				$db->error();
			}else{
				echo '<table cellspacing="0" cellpadding="2" border="1" width="100%" id="table">';
				echo '<thead>';
				echo '<tr>';
				echo '<th width="30px">No.</th>';
				echo '<th>Date</th>';
				//echo '<th>Category</th>';
				echo '<th>Customer</th>';
				echo '<th>DN No.</th>';
				echo '<th>Credit Note ID</th>';
				echo '<th>Status</th>';

				if($access->sectionAccess(user_id(), $this->page, 'E') || $access->sectionAccess(user_id(), $this->page, 'D')){
					echo '<th width="">Action</th>';
				}
				echo '</tr>';
				echo '</thead>';
				
				$i=1;
				echo '<tbody>';
				foreach($select[0] as $row){
					extract($row);
					echo '<tr>';
					echo '<td><center>'.($i++).'.</center></td>';
					echo '<td>'.FeedBack::date_s($oi_date).'</td>';
					//echo '<td>'.$this->rgf("categories",$oi_category,"cat_id","cat_name").'</td>';
					echo '<td>'.$this->rgf("customers",$oi_customer,"cust_id","cust_name").'</td>';
					echo '<td>'.$oi_cn_fdn.'</td>';
					echo '<td>'.$oi_efris_cn_id.'</td>';
					echo '<td>'.$oi_status.'</td>';
					
					if($access->sectionAccess(user_id(), $this->page, 'E') || $access->sectionAccess(user_id(), $this->page, 'D')){
					
						echo '<td>';
						if(!$oi_efris_cn_id){
							if($access->sectionAccess(user_id(), $this->page, 'E'))
							echo $this->action('edit','other-invoices/edit-debt-note/'.$oi_id, 'Edit');
							if($access->sectionAccess(user_id(), $this->page, 'D'))
							//echo $this->action('delete','other-invoices/delete-debt-note/'.$oi_id, 'Delete');
							echo '<a class="btn btn-xs btn-danger" onclick="return confirm(\'Do you want to Delete this Debt Note?\');" href="'.return_url().'/other-invoices/delete-debt-note/'.$oi_id.'"><i class="fa fa-fw fa-trash"></i> Delete</a>';
						}else{	
							//echo '<a style="margin:auto 5px;" class="btn btn-xs btn-primary" href="'.return_url().'other-invoices/cancel-debit-note/'.$oi_id.'">Cancel DN</a>';			
						}
						echo"&nbsp;<a class='btn btn-default btn-xs' href=".return_url().'other-invoices/view-debt-note/'.$oi_id.">View</a> ";
						echo '</td>';
					}
					echo '</tr>';
					
					
				}
				echo '</tbody>';
				
				echo '</table>';
				
			
				
			}
			
			?>
		</div>
		<?php
	}
	public function deleteDebitNoteAction(){
		$id = portion(3);
		$this->deletor("dn_other_invoice", "oi_id",  $id, 'other-invoices/all-debit-notes');
	}
	public function creditNoteAction(){
		$id=portion(3);
		$db=new Db();

		$isCN = TRUE;
		$select=$db->select("select * from cn_other_invoice WHERE oi_idid = '$id'");
		if(!$db->num_rows()){
			$isCN = FALSE;
			$select=$db->select("select * from other_invoice WHERE oi_id='$id'");
		}
		if($db->num_rows()){
		extract($select[0][0]);
		//print_r($select);
		}
		if(isset($_POST['submit'])){
		
			$category = 1;//$_POST['category'];
			$customer = $_POST['customer'];
			$pay_template = $_POST['pay_template'];
			$title = $_POST['title'];
			$description = $_POST['description'];

			$time = time();
			$user = user_id();
			
			$errors = array();
			// if(empty($category)){
			// 	$errors[]="Select Category";
			// }
			
		

		if(empty($errors)){
			$db = new Db();
			$x = $db->update("other_invoice",["oi_category"=>$category,"oi_customer"=>$customer,"oi_pay_template"=>$pay_template,"oi_title"=>$title,"oi_description"=>$description,"oi_added_by"=>$user,"oi_date_added"=>$time],["oi_id"=>$id]);
			
			if($x){
				FeedBack::success();
				FeedBack::refresh(3, return_url().'other-invoices/edit-invoice/'.$id);
			}else{
				FeedBack::error();
			}
			}else{
				FeedBack::errors($errors);
			}
		}
		?>
		<div class="col-md-12">
			<h3>&nbsp;</h3>
			<div class="col-lg-12">
			<div class="panel panel-default">
				<div class="panel-heading">
					
				</div>
				<div class="panel-body">
					<div class="row">
						<div class="col-lg-12">
							<form role="form" action="" method="post">
								<div class="form-group">
									<div id="must">All field with asterisk(*) are mandatory.</div>
										<div class="row">
										<div class="col-md-2">
										<label>Date<span class="must">*</span></label>
										<div class="form-line">
											<input class="form-control" style="height: 29px;" id="date" type="date" name="" value="<?php echo date('Y-m-d',$oi_date); ?>">
										</div>
									</div>
									<div class="col-md-2" style="display:none;">
										<label>Category<span class="must">*</span></label>
										<div class="form-line">
											<select id="category" class="form-control">
												<?php
												echo '<option>-----select------</option>';	
												$db = new Db();
												$select = $db->select("SELECT * FROM categories");
												foreach ($select[0] as $row) {
													extract($row);
													if($cat_id == $oi_category)
													echo '<option selected value="'.$cat_id.'">'.$cat_name.'</option>';	
													else
													echo '<option value="'.$cat_id.'">'.$cat_name.'</option>';		
												}

												?>
											</select>
											</div>
									</div>
									<div class="col-md-7">
										<label>Customers<span class="must">*</span></label>
										<?php
												echo'<select  class="form-control" name="customer" id="customer">';
											$select = $db->select("SELECT * FROM customers ORDER BY cust_name ASC");
											foreach($select[0] as $row){
												extract($row);
												if($oi_customer == $cust_id)
													echo '<option selected value="'.$cust_id.'"><b>'.$cust_name.'</b></option>';
												else
													echo '<option value="'.$cust_id.'">'.$cust_name.'</option>';
											}
											
										echo'</select>';
										?>
									</div>
									<script type="text/javascript">
										$(document).ready(function(){
											$('#category').change(function(){
												var category = $(this).val();
												var form_data = new FormData(); 
												form_data.append('category', category); 

												$.ajax({
										        	url: '<?php echo return_url().'ajax/invoice_details_by_category.php'; ?>',
										            type: "POST",
										            data: form_data,
										            contentType: false,
										            cache: false,
										            processData:false,
										            success: function(data){
										            	//alert(data);
										            	$('#customer').html(data);	
										            }
										        });
											});
										});
									</script>
									<div class="col-md-3">
										<label>Payment Template<span class="must">*</span></label>
										<div class="form-line">
											<select class="form-control" id="pay_template" name="pay_template">
												<?php
												$sql = "SELECT * FROM payment_information";
												$db = new Db();
												$select = $db->query($sql);
												foreach($select as $row){
													extract($row);
													if($t_id == $oi_template)
														echo '<option selected value="'.$t_id.'">'.$t_name.'</option>';
													else
														echo '<option value="'.$t_id.'">'.$t_name.'</option>';
												}
												
												?>
											</select>
										</div>
									</div>

									<div class="clearfix"></div>
									<div class="col-md-2">
										<label>Currency<span class="must">*</span></label>
										<div class="form-line">
											<select class="form-control" id="currency" name="currency">
												<?php
												$db = new Db();
												$sql = "SELECT * FROM dictionary WHERE d_category = '1' ORDER BY d_name ASC";
												$select = $db->select($sql);
												foreach($select[0] as $row){
													extract($row);
													if($oi_currency == $d_code)
													echo '<option selected value="'.$d_code.'">'.$d_name.' ('.$d_code.')</option>';
												else
													echo '<option value="'.$d_code.'">'.$d_name.' ('.$d_code.')</option>';
												}
												
												?>
											</select>
										</div>
									</div>
									<div class="col-md-4">
										<label>VAT<span class="must">*</span></label>
										<div class="form-line">
											<select class="form-control" id="vat" name="currency">
												<?php
												foreach($this->VATCAT() as $row=>$x){
													extract($row);
													if($row == $oi_vat)
														echo '<option selected value="'.$row.'">'.$x[0].'</option>';
													else
														echo '<option value="'.$row.'">'.$x[0].'</option>';
												}
												
												?>
											</select>
										</div>
									</div>
									<div class="col-md-5">
										<label>Reference No.<span class="must">*</span></label>
										<div class="form-line">
											<input type="text" value="<?php echo $oi_ref_no;?>" id="ref_no" class="form-control" name="">
										</div>
									</div>
									<div class="col-md-12">
										<label>Remarks<span class="must">*</span></label>
										<div class="form-line">
											<textarea id="general_description" class="form-control"><?php echo $oi_general_description;?></textarea>
										</div>
									</div>
									
								</div>
								<div class="row">
							<?php 
								$desc1 = "";
								$amount1 = 0;

								if($isCN)
									$sql = "SELECT * FROM cn_invoice_item_description WHERE ii_rel = '$oi_rel' ORDER BY ii_id ASC";
								else
									$sql = "SELECT * FROM invoice_item_description WHERE ii_rel = '$oi_rel' ORDER BY ii_id ASC";

								$select = $db->select($sql);
								if($db->num_rows())
								extract($select[0][0]);
							?>
							<div class="col-md-12">
								<label>Item<span class="must">*</span></label>
								<style type="text/css">
									.added-section:hover{
										background-color: red;
									}
								</style>
								<div id="worddescription">
									<div class="row">
										<div class="col-md-1" style="font-weight:bold;color:red;">
											#
										</div>
										<div class="col-md-5" style="font-weight:bold;color:red;">
											Details
										</div>
										<div class="col-md-2" style="font-weight:bold;color:red;">
											Qty
										</div>
										<div class="col-md-2" style="font-weight:bold;color:red;">
											UnitPrice <i>(VAT Inc)</i>
										</div>
									</div>
									<div class="row section">
										<div class="added-section" id="1">
											<div class="col-md-1">
												1.
											</div>
											<div class="col-md-5">
												<?php
													$db = new Db();
														echo'<select class="form-control" id="description1">';
														echo'<option value="">-------select------</option>';
														$select = $db->select("SELECT * from efris_goods ORDER BY eg_name ASC");
														foreach ($select[0] as $row) {
															extract($row);
															if ($eg_id == $ii_description)
																echo'<option selected value="'.$eg_id.'">'.$eg_name.'('.$eg_code.')</option>';
															else
															echo'<option value="'.$eg_id.'">'.$eg_name.'('.$eg_code.')</option>';	
														}
														echo '</select>';
													?>
											</div>
											<div class="col-md-2">
												<input autocomplete="off" type="number" value="<?php echo $ii_quantity;?>" id="quantity1" class="form-control"/>
											</div>
											<div class="col-md-2">
												<input autocomplete="off" type="text" min=0 value="<?php echo $ii_amount;?>" id="amount1" class="form-control"/>
											</div>
										</div>
										<?php 
											$db = new Db();
											//$sql = "SELECT * FROM invoice_item_description WHERE ii_rel = '$oi_rel' AND ii_id != '$ii_id' ORDER BY ii_id ASC";

											if($isCN)
												$sql = "SELECT * FROM cn_invoice_item_description WHERE ii_rel = '$oi_rel' AND ii_id != '$ii_id' ORDER BY ii_id ASC";
											else
												$sql = "SELECT * FROM invoice_item_description WHERE ii_rel = '$oi_rel' AND ii_id != '$ii_id' ORDER BY ii_id ASC";

											$select = $db->select($sql);
											//print_r($select);
											$x=1;
											foreach($select[0] as $row){
												extract($row);
												$x++;
												echo '<div class="clearfix"></div>';
												echo '<div class="added-section" id="'.$x.'">';
												echo '<div class="col-md-1 line-number" >'.$x.'.</div>';
												echo '<div class="col-md-5">';
												// <textarea autofocus class="form-control" id="description'.$x.'">'.$ii_description.'</textarea>

													$db = new Db();
														$db = new Db();
															echo'<select class="form-control" id="description'.$x.'">';
															echo'<option value="">-------select------</option>';
															$select = $db->select("SELECT * from efris_goods ORDER BY eg_name ASC");
															foreach ($select[0] as $row) {
																extract($row);
																if ($eg_id == $ii_description)
																	echo'<option selected value="'.$eg_id.'">'.$eg_name.'('.$eg_code.')</option>';
																else
																echo'<option value="'.$eg_id.'">'.$eg_name.'('.$eg_code.')</option>';	
															}
															echo '</select>';
														
													
												echo'</div>';
												echo '<div class="col-md-2"><input autocomplete="off" type="number" value="'.$ii_quantity.'" id="quantity'.$x.'" class="form-control"/></div>';
												echo '<div class="col-md-2"><input autocomplete="off" type="text" value="'.number_format($ii_amount,2).'" id="amount'.$x.'" class="form-control"/></div>';
												echo '<div class="col-md-2"><button type="button" class="text-danger" id="remove-wd"><i class="fa fa-fw fa-times"></i></button></div>';
												echo '</div>';

							
											}
										?>
									</div>
									<div class="col-md-12">
										<button type="button" id="plus-wd" class="btn btn-xs pull-right text-primary"><i class="fa fa-fw fa-plus"></i></button>
									</div>
								</div>
								<script type="text/javascript">
									$(function(){
										$(document).off('click','#plus-wd').on('click','#plus-wd',function(e){
											var random = Date.now();
											var html = '<div class="clearfix"></div>';
											html += '<div class="added-section" id="'+random+'">';
											html += '<div class="col-md-1 line-number"></div>';
											html += '<div class="col-md-5">';
											html += '<?php
													
													echo '<select class="form-control" id="description'; ?>'+random+'<?php echo '" style="">';
														
													$db = new Db();
														
														echo'<option value="">-------select------</option>';
														$select = $db->select("SELECT * from efris_goods ORDER BY eg_name ASC");
														foreach ($select[0] as $row) {
															extract($row);
															if ($eg_id == $ii_description)
																echo'<option selected value="'.$eg_id.'">'.$eg_name.'('.$eg_code.')</option>';
															else
															echo'<option value="'.$eg_id.'">'.$eg_name.'('.$eg_code.')</option>';	
														}
														echo '</select>';
														
													?>';
											html += '</div>';
											html += '<div class="col-md-2">';
											html += '<input autocomplete="off" type="number" value="" id="quantity'+random+'" class="form-control"/>';
											html += '</div>';
											html += '<div class="col-md-2">';
											html += '<input autocomplete="off" type="text" value="" id="amount'+random+'" class="form-control"/>';
											html += '</div>';
											html += '<div class="col-md-1"><button type="button" class="text-danger" id="remove-wd"><i class="fa fa-fw fa-times"></i></button></div>';
											html += '</div>';
											html += '</div>';

											$('#worddescription .section').append(html);
											$('#description'+random).focus();

											var count = 2;
											$('.line-number').each(function(){
												$(this).html(count+'. ');
												count++;
											});

										});

										$(document).off('click','#remove-wd').on('click','#remove-wd',function(e){
											var parent = $(this).parent().parent();
											parent.remove();

											var count = 2;
											$('.line-number').each(function(){
												$(this).html(count+'. ');
												count++;
											});
										});


									});
								</script>
							</div>

						</div>
										<br>
										<input type="hidden" value="<?php echo $oi_id?>" id="oi_idid" name="">
										<input type="hidden" value="<?php echo $oi_invoice_id?>" id="efris_id" name="">
										<button id="editOtherInvoiceBtn" type="button" name="submit" class="btn btn-primary"><i class="fa fa-fw fa-save"></i> Save</button><span id="editOtherInvoiceStatus"></span>
									</div>

								</div>
								
								
							</form>
						</div>
					</div>
				</div>
			</div>
		</div>
			<script type="text/javascript">
			$(document).ready(function(){
	$(document).off('click','#editOtherInvoiceBtn').on('click', '#editOtherInvoiceBtn', function(e){

		var date = $('#date').val();
		var category = 1;//$('#category').val();
		var customer  = $('#customer').val();
		var pay_template  = $('#pay_template').val();
		var currency  = $('#currency').val();
		var general_description  = $('#general_description').val();
		var oi_idid  = $('#oi_idid').val();
		var efris_id  = $('#efris_id').val();
		var id  = $('#id').val();
		var vat  = $('#vat').val();
		var ref_no  = $('#ref_no').val();

        var counter=1;
		var counterArray = new Array();
		$('#worddescription .section').find(".added-section").each(function(){
			var findID = $(this).attr('id');
			//var id = findID.split("_")[1];
			//id = (!id)?"":"_"+id;
			counterArray.push(findID);
			//alert(findID);
		});


		var errors = new Array();
		// if(general_description==""){
		// 	errors.push("Select Customer Name");
		// }

		for(i=0; i<counterArray.length; i++){				
			var description1 = $('#description'+counterArray[i]).val();
			var quantity1 = $('#quantity'+counterArray[i]).val();				
			var amount1 = parseInt($('#amount'+counterArray[i]).val());

			if(!description1){
				errors.push("Line "+(i+1)+": Select Item");
			}
			if(!quantity1){
				errors.push("Line "+(i+1)+": Enter quantity");
			}
			if(!amount1){
				errors.push("Line "+(i+1)+": Enter Amount");
			}

			//alert(description1);
		}

//return 0;
		
		
		if(errors.length >= 1){
			alert("Please Review the following:\n "+errors.join("\n"));
		}else{
			//$('#addOtherInvoiceStatus').html('<img src="'+urlPath+'images/loading.gif" alt="loading..."/>');
			$(this).html("Add Invoice");
			$(this).attr("disabled", true);
			var form_data = new FormData();
			form_data.append('date', date);
			form_data.append('category', category);
			form_data.append('customer', customer);
			form_data.append('pay_template', pay_template);
			form_data.append('currency', currency);
			form_data.append('general_description', general_description);
			form_data.append('efris_id', efris_id);
			form_data.append('oi_idid', oi_idid);
			form_data.append('vat', vat);
			form_data.append('ref_no', ref_no);

			
			 form_data.append('totalItems', counterArray.length);
			for(i=0; i<counterArray.length; i++){
				form_data.append('description'+i, $('#description'+counterArray[i]).val());
				form_data.append('quantity'+i, $('#quantity'+counterArray[i]).val());
				form_data.append('amount'+i, $('#amount'+counterArray[i]).val());
				form_data.append('id', id);
			}

			$.ajax({
			//url: urlPath2+"ajax/addOtherInvoice.php",
			url: '<?php echo return_url().'ajax/cnOtherInvoice.php'; ?>',
			type: "POST",
			data: form_data,
			contentType: false,
			cache: false,
			processData:false,
			success: function(data){
				//alert(data);
				location.reload();
				$('#editOtherInvoiceStatus').html('Successfully Saved');
			}
		
		});	
		}

	});


});
		</script>
	<?php
	}

	public function debtNoteAction(){
		$id=portion(3);
		$db=new Db();

		$isCN = TRUE;
		$select=$db->select("select * from dn_other_invoice WHERE oi_idid = '$id'");
		if(!$db->num_rows()){
			$isCN = FALSE;
			$select=$db->select("select * from other_invoice WHERE oi_id='$id'");
		}
		if($db->num_rows()){
		extract($select[0][0]);
		//print_r($select);
		}
		if(isset($_POST['submit'])){
		
			$category = $_POST['category'];
			$customer = $_POST['customer'];
			$pay_template = $_POST['pay_template'];
			$title = $_POST['title'];
			$description = $_POST['description'];

			$time = time();
			$user = user_id();
			
			$errors = array();
			// if(empty($category)){
			// 	$errors[]="Select Category";
			// }
			
		

		if(empty($errors)){
			$db = new Db();
			$x = $db->update("other_invoice",["oi_category"=>$category,"oi_customer"=>$customer,"oi_pay_template"=>$pay_template,"oi_title"=>$title,"oi_description"=>$description,"oi_added_by"=>$user,"oi_date_added"=>$time],["oi_id"=>$id]);
			
			if($x){
				FeedBack::success();
				FeedBack::refresh(3, return_url().'other-invoices/edit-invoice/'.$id);
			}else{
				FeedBack::error();
			}
			}else{
				FeedBack::errors($errors);
			}
		}
		?>
		<div class="col-md-12">
			<h3>&nbsp;</h3>
			<div class="col-lg-12">
			<div class="panel panel-default">
				<div class="panel-heading">
					
				</div>
				<div class="panel-body">
					<div class="row">
						<div class="col-lg-12">
							<form role="form" action="" method="post">
								<div class="form-group">
									<div id="must">All field with asterisk(*) are mandatory.</div>
										<div class="row">
										<div class="col-md-2">
										<label>Date<span class="must">*</span></label>
										<div class="form-line">
											<input class="form-control" style="height: 29px;" id="date" type="date" name="" value="<?php echo date('Y-m-d',$oi_date); ?>">
										</div>
									</div>
									<div class="col-md-2" style="display: none;">
										<label>Category<span class="must">*</span></label>
										<div class="form-line">
											<select id="category" class="form-control">
												<?php
												echo '<option>-----select------</option>';	
												$db = new Db();
												$select = $db->select("SELECT * FROM categories");
												foreach ($select[0] as $row) {
													extract($row);
													if($cat_id == $oi_category)
													echo '<option selected value="'.$cat_id.'">'.$cat_name.'</option>';	
													else
													echo '<option value="'.$cat_id.'">'.$cat_name.'</option>';		
												}

												?>
											</select>
											</div>
									</div>
									<div class="col-md-7">
										<label>Customers<span class="must">*</span></label>
										<?php
												echo'<select  class="form-control" name="customer" id="customer">';
											$select = $db->select("SELECT * FROM customers ORDER BY cust_name ASC");
											foreach($select[0] as $row){
												extract($row);
												if($oi_customer == $cust_id)
													echo '<option selected value="'.$cust_id.'"><b>'.$cust_name.'</b></option>';
												else
													echo '<option value="'.$cust_id.'">'.$cust_name.'</option>';
											}
											
										echo'</select>';
										?>
									</div>
									<script type="text/javascript">
										$(document).ready(function(){
											$('#category').change(function(){
												var category = $(this).val();
												var form_data = new FormData(); 
												form_data.append('category', category); 

												$.ajax({
										        	url: '<?php echo return_url().'ajax/invoice_details_by_category.php'; ?>',
										            type: "POST",
										            data: form_data,
										            contentType: false,
										            cache: false,
										            processData:false,
										            success: function(data){
										            	//alert(data);
										            	$('#customer').html(data);	
										            }
										        });
											});
										});
									</script>
									<div class="col-md-3">
										<label>Payment Template<span class="must">*</span></label>
										<div class="form-line">
											<select class="form-control" id="pay_template" name="pay_template">
												<?php
												$sql = "SELECT * FROM payment_information";
												$db = new Db();
												$select = $db->query($sql);
												foreach($select as $row){
													extract($row);
													if($t_id == $oi_template)
														echo '<option selected value="'.$t_id.'">'.$t_name.'</option>';
													else
														echo '<option value="'.$t_id.'">'.$t_name.'</option>';
												}
												
												?>
											</select>
										</div>
									</div>

									<div class="clearfix"></div>
									<div class="col-md-2">
										<label>Currency<span class="must">*</span></label>
										<div class="form-line">
											<select class="form-control" id="currency" name="currency">
												<?php
												$db = new Db();
												$sql = "SELECT * FROM dictionary WHERE d_category = '1' ORDER BY d_name ASC";
												$select = $db->select($sql);
												foreach($select[0] as $row){
													extract($row);
													if($oi_currency == $d_code)
													echo '<option selected value="'.$d_code.'">'.$d_name.' ('.$d_code.')</option>';
												else
													echo '<option value="'.$d_code.'">'.$d_name.' ('.$d_code.')</option>';
												}
												
												?>
											</select>
										</div>
									</div>
									<div class="col-md-5">
										<label>VAT<span class="must">*</span></label>
										<div class="form-line">
											<select class="form-control" id="vat" name="currency">
												<?php
												

												foreach($this->VATCAT() as $row=>$x){
													extract($row);
													if($t_id == $template)
														echo '<option selected value="'.$row.'">'.$x[0].'</option>';
													else
														echo '<option value="'.$row.'">'.$x[0].'</option>';
												}
												
												?>
											</select>
										</div>
									</div>
									<div class="col-md-4">
										<label>Reference No.<span class="must">*</span></label>
										<div class="form-line">
											<input type="text" value="<?php echo $oi_ref_no;?>" id="ref_no" class="form-control" name="">
										</div>
									</div>
									<div class="col-md-12">
										<label>Remarks<span class="must">*</span></label>
										<div class="form-line">
											<textarea id="general_description" class="form-control"><?php echo $oi_general_description;?></textarea>
										</div>
									</div>
									
								</div>
								<div class="row">
							<?php 
								$desc1 = "";
								$amount1 = 0;

								if($isCN)
									$sql = "SELECT * FROM dn_invoice_item_description WHERE ii_rel = '$oi_rel' ORDER BY ii_id ASC";
								else
									$sql = "SELECT * FROM invoice_item_description WHERE ii_rel = '$oi_rel' ORDER BY ii_id ASC";

								$select = $db->select($sql);
								if($db->num_rows())
								extract($select[0][0]);
							?>
							<div class="col-md-12">
								<label>Item<span class="must">*</span></label>
								<style type="text/css">
									.added-section:hover{
										background-color: red;
									}
								</style>
								<div id="worddescription">
									<div class="row">
										<div class="col-md-1" style="font-weight:bold;color:red;">
											#
										</div>
										<div class="col-md-5" style="font-weight:bold;color:red;">
											Details
										</div>
										<div class="col-md-2" style="font-weight:bold;color:red;">
											Qty
										</div>
										<div class="col-md-2" style="font-weight:bold;color:red;">
											UnitPrice <i>(VAT Inc)</i>
										</div>
									</div>
									<div class="row section">
										<div class="added-section" id="1">
											<div class="col-md-1">
												1.
											</div>
											<div class="col-md-5">
												<?php
													$db = new Db();
														echo'<select class="form-control" id="description1">';
														echo'<option value="">-------select------</option>';
														$select = $db->select("SELECT * from efris_goods ORDER BY eg_name ASC");
														foreach ($select[0] as $row) {
															extract($row);
															if ($eg_id == $ii_description)
																echo'<option selected value="'.$eg_id.'">'.$eg_name.'('.$eg_code.')</option>';
															else
															echo'<option value="'.$eg_id.'">'.$eg_name.'('.$eg_code.')</option>';	
														}
														echo '</select>';
													?>
											</div>
											<div class="col-md-2">
												<input autocomplete="off" type="number" value="<?php echo $ii_quantity;?>" id="quantity1" class="form-control"/>
											</div>
											<div class="col-md-2">
												<input autocomplete="off" type="text" min=0 value="<?php echo $ii_amount;?>" id="amount1" class="form-control"/>
											</div>
										</div>
										<?php 
											$db = new Db();
											//$sql = "SELECT * FROM invoice_item_description WHERE ii_rel = '$oi_rel' AND ii_id != '$ii_id' ORDER BY ii_id ASC";

											if($isCN)
												$sql = "SELECT * FROM dn_invoice_item_description WHERE ii_rel = '$oi_rel' AND ii_id != '$ii_id' ORDER BY ii_id ASC";
											else
												$sql = "SELECT * FROM invoice_item_description WHERE ii_rel = '$oi_rel' AND ii_id != '$ii_id' ORDER BY ii_id ASC";

											$select = $db->select($sql);
											//print_r($select);
											$x=1;
											foreach($select[0] as $row){
												extract($row);
												$x++;
												echo '<div class="clearfix"></div>';
												echo '<div class="added-section" id="'.$x.'">';
												echo '<div class="col-md-1 line-number" >'.$x.'.</div>';
												echo '<div class="col-md-5">';
												// <textarea autofocus class="form-control" id="description'.$x.'">'.$ii_description.'</textarea>

													$db = new Db();
														$db = new Db();
															echo'<select class="form-control" id="description'.$x.'">';
															echo'<option value="">-------select------</option>';
															$select = $db->select("SELECT * from efris_goods ORDER BY eg_name ASC");
															foreach ($select[0] as $row) {
																extract($row);
																if ($eg_id == $ii_description)
																	echo'<option selected value="'.$eg_id.'">'.$eg_name.'('.$eg_code.')</option>';
																else
																echo'<option value="'.$eg_id.'">'.$eg_name.'('.$eg_code.')</option>';	
															}
															echo '</select>';
														
													
												echo'</div>';
												echo '<div class="col-md-2"><input autocomplete="off" type="number" value="'.$ii_quantity.'" id="quantity'.$x.'" class="form-control"/></div>';
												echo '<div class="col-md-2"><input autocomplete="off" type="text" value="'.number_format($ii_amount,2).'" id="amount'.$x.'" class="form-control"/></div>';
												echo '<div class="col-md-2"><button type="button" class="text-danger" id="remove-wd"><i class="fa fa-fw fa-times"></i></button></div>';
												echo '</div>';

							
											}
										?>
									</div>
									<div class="col-md-12">
										<button type="button" id="plus-wd" class="btn btn-xs pull-right text-primary"><i class="fa fa-fw fa-plus"></i></button>
									</div>
								</div>
								<script type="text/javascript">
									$(function(){
										$(document).off('click','#plus-wd').on('click','#plus-wd',function(e){
											var random = Date.now();
											var html = '<div class="clearfix"></div>';
											html += '<div class="added-section" id="'+random+'">';
											html += '<div class="col-md-1 line-number"></div>';
											html += '<div class="col-md-5">';
											html += '<?php
													
													echo '<select class="form-control" id="description'; ?>'+random+'<?php echo '" style="">';
														
													$db = new Db();
														
														echo'<option value="">-------select------</option>';
														$select = $db->select("SELECT * from efris_goods ORDER BY eg_name ASC");
														foreach ($select[0] as $row) {
															extract($row);
															if ($eg_id == $ii_description)
																echo'<option selected value="'.$eg_id.'">'.$eg_name.'('.$eg_code.')</option>';
															else
															echo'<option value="'.$eg_id.'">'.$eg_name.'('.$eg_code.')</option>';	
														}
														echo '</select>';
														
													?>';
											html += '</div>';
											html += '<div class="col-md-2">';
											html += '<input autocomplete="off" type="number" value="" id="quantity'+random+'" class="form-control"/>';
											html += '</div>';
											html += '<div class="col-md-2">';
											html += '<input autocomplete="off" type="text" value="" id="amount'+random+'" class="form-control"/>';
											html += '</div>';
											html += '<div class="col-md-1"><button type="button" class="text-danger" id="remove-wd"><i class="fa fa-fw fa-times"></i></button></div>';
											html += '</div>';
											html += '</div>';

											$('#worddescription .section').append(html);
											$('#description'+random).focus();

											var count = 2;
											$('.line-number').each(function(){
												$(this).html(count+'. ');
												count++;
											});

										});

										$(document).off('click','#remove-wd').on('click','#remove-wd',function(e){
											var parent = $(this).parent().parent();
											parent.remove();

											var count = 2;
											$('.line-number').each(function(){
												$(this).html(count+'. ');
												count++;
											});
										});


									});
								</script>
							</div>

						</div>
										<br>
										<input type="hidden" value="<?php echo $oi_id?>" id="oi_idid" name="">
										<input type="hidden" value="<?php echo $oi_invoice_id?>" id="efris_id" name="">
										<button id="editOtherInvoiceBtn" type="button" name="submit" class="btn btn-primary"><i class="fa fa-fw fa-save"></i> Save</button><span id="editOtherInvoiceStatus"></span>
									</div>

								</div>
								
								
							</form>
						</div>
					</div>
				</div>
			</div>
		</div>
			<script type="text/javascript">
			$(document).ready(function(){
	$(document).off('click','#editOtherInvoiceBtn').on('click', '#editOtherInvoiceBtn', function(e){

		var date = $('#date').val();
		var category = $('#category').val();
		var customer  = $('#customer').val();
		var pay_template  = $('#pay_template').val();
		var currency  = $('#currency').val();
		var general_description  = $('#general_description').val();
		var oi_idid  = $('#oi_idid').val();
		var efris_id  = $('#efris_id').val();
		var id  = $('#id').val();
		var vat  = $('#vat').val();
		var ref_no  = $('#ref_no').val();

        var counter=1;
		var counterArray = new Array();
		$('#worddescription .section').find(".added-section").each(function(){
			var findID = $(this).attr('id');
			//var id = findID.split("_")[1];
			//id = (!id)?"":"_"+id;
			counterArray.push(findID);
			//alert(findID);
		});


		var errors = new Array();
		// if(general_description==""){
		// 	errors.push("Select Customer Name");
		// }

		for(i=0; i<counterArray.length; i++){				
			var description1 = $('#description'+counterArray[i]).val();
			var quantity1 = $('#quantity'+counterArray[i]).val();				
			var amount1 = parseInt($('#amount'+counterArray[i]).val());

			if(!description1){
				errors.push("Line "+(i+1)+": Select Item");
			}
			if(!quantity1){
				errors.push("Line "+(i+1)+": Enter quantity");
			}
			if(!amount1){
				errors.push("Line "+(i+1)+": Enter Amount");
			}

			//alert(description1);
		}

//return 0;
		
		
		if(errors.length >= 1){
			alert("Please Review the following:\n "+errors.join("\n"));
		}else{
			//$('#addOtherInvoiceStatus').html('<img src="'+urlPath+'images/loading.gif" alt="loading..."/>');
			$(this).html("Add Invoice");
			$(this).attr("disabled", true);
			var form_data = new FormData();
			form_data.append('date', date);
			form_data.append('category', category);
			form_data.append('customer', customer);
			form_data.append('pay_template', pay_template);
			form_data.append('currency', currency);
			form_data.append('general_description', general_description);
			form_data.append('efris_id', efris_id);
			form_data.append('oi_idid', oi_idid);
			form_data.append('vat', vat);
			form_data.append('ref_no', ref_no);

			
			 form_data.append('totalItems', counterArray.length);
			for(i=0; i<counterArray.length; i++){
				form_data.append('description'+i, $('#description'+counterArray[i]).val());
				form_data.append('quantity'+i, $('#quantity'+counterArray[i]).val());
				form_data.append('amount'+i, $('#amount'+counterArray[i]).val());
				form_data.append('id', id);
			}

			$.ajax({
			//url: urlPath2+"ajax/addOtherInvoice.php",
			url: '<?php echo return_url().'ajax/dnOtherInvoice.php'; ?>',
			type: "POST",
			data: form_data,
			contentType: false,
			cache: false,
			processData:false,
			success: function(data){
				//alert(data);
				location.replace('<?php echo return_url(); ?>other-invoices/all-debt-notes');
				$('#editOtherInvoiceStatus').html('Successfully Saved');
			}
		
		});	
		}

	});


});
		</script>
	<?php
	}
	public function editCreditNoteAction(){
		$id=portion(3);
		$db=new Db();
		$select=$db->select("select * from cn_other_invoice WHERE oi_id='$id'");
		if($db->num_rows()){
		extract($select[0][0]);
		//print_r($select);
		}
		if(isset($_POST['submit'])){
		
			$category = $_POST['category'];
			$customer = $_POST['customer'];
			$pay_template = $_POST['pay_template'];
			$title = $_POST['title'];
			$description = $_POST['description'];

			$time = time();
			$user = user_id();
			
			$errors = array();
			// if(empty($category)){
			// 	$errors[]="Select Category";
			// }
			
		

		if(empty($errors)){
			$db = new Db();
			$x = $db->update("other_invoice",["oi_category"=>$category,"oi_customer"=>$customer,"oi_pay_template"=>$pay_template,"oi_title"=>$title,"oi_description"=>$description,"oi_added_by"=>$user,"oi_date_added"=>$time],["oi_id"=>$id]);
			
			if($x){
				FeedBack::success();
				FeedBack::refresh(3, return_url().'other-invoices/edit-invoice/'.$id);
			}else{
				FeedBack::error();
			}
			}else{
				FeedBack::errors($errors);
			}
		}
		?>
		<div class="col-md-12">
			<h3>&nbsp;</h3>
			<div class="col-lg-12">
			<div class="panel panel-default">
				<div class="panel-heading">
					
				</div>
				<div class="panel-body">
					<div class="row">
						<div class="col-lg-12">
							<form role="form" action="" method="post">
								<div class="form-group">
									<div id="must">All field with asterisk(*) are mandatory.</div>
										<div class="row">
										<div class="col-md-2">
										<label>Date<span class="must">*</span></label>
										<div class="form-line">
											<input class="form-control" style="height: 29px;" id="date" type="date" name="" value="<?php echo date('Y-m-d',$oi_date); ?>">
										</div>
									</div>
									<div class="col-md-2" style="display:none;">
										<label>Category<span class="must">*</span></label>
										<div class="form-line">
											<select id="category" class="form-control">
												<?php
												echo '<option>-----select------</option>';	
												$db = new Db();
												$select = $db->select("SELECT * FROM categories");
												foreach ($select[0] as $row) {
													extract($row);
													if($cat_id == $oi_category)
													echo '<option selected value="'.$cat_id.'">'.$cat_name.'</option>';	
													else
													echo '<option value="'.$cat_id.'">'.$cat_name.'</option>';		
												}

												?>
											</select>
											</div>
									</div>
									<div class="col-md-7">
										<label>Customers<span class="must">*</span></label>
										<?php
												echo'<select  class="form-control" name="customer" id="customer">';
											$select = $db->select("SELECT * FROM customers ORDER BY cust_name ASC");
											foreach($select[0] as $row){
												extract($row);
												if($oi_customer == $cust_id)
													echo '<option selected value="'.$cust_id.'"><b>'.$cust_name.'</b></option>';
												else
													echo '<option value="'.$cust_id.'">'.$cust_name.'</option>';
											}
											
										echo'</select>';
										?>
									</div>
									<script type="text/javascript">
										$(document).ready(function(){
											$('#category').change(function(){
												var category = $(this).val();
												var form_data = new FormData(); 
												form_data.append('category', category); 

												$.ajax({
										        	url: '<?php echo return_url().'ajax/invoice_details_by_category.php'; ?>',
										            type: "POST",
										            data: form_data,
										            contentType: false,
										            cache: false,
										            processData:false,
										            success: function(data){
										            	//alert(data);
										            	$('#customer').html(data);	
										            }
										        });
											});
										});
									</script>
									<div class="col-md-3">
										<label>Payment Template<span class="must">*</span></label>
										<div class="form-line">
											<select class="form-control" id="pay_template" name="pay_template">
												<?php
												$sql = "SELECT * FROM payment_information";
												$db = new Db();
												$select = $db->query($sql);
												foreach($select as $row){
													extract($row);
													if($t_id == $oi_template)
														echo '<option selected value="'.$t_id.'">'.$t_name.'</option>';
													else
														echo '<option value="'.$t_id.'">'.$t_name.'</option>';
												}
												
												?>
											</select>
										</div>
									</div>
									
									<div class="clearfix"></div>
									<div class="col-md-2">
										<label>Currency<span class="must">*</span></label>
										<div class="form-line">
											<select class="form-control" id="currency" name="currency">
												<?php
												$db = new Db();
												$sql = "SELECT * FROM dictionary WHERE d_category = '1' ORDER BY d_name ASC";
												$select = $db->select($sql);
												foreach($select[0] as $row){
													extract($row);
													if($oi_currency == $d_code)
													echo '<option selected value="'.$d_code.'">'.$d_name.' ('.$d_code.')</option>';
												else
													echo '<option value="'.$d_code.'">'.$d_name.' ('.$d_code.')</option>';
												}
												
												?>
											</select>
										</div>
									</div>
									<div class="col-md-4">
										<label>VAT<span class="must">*</span></label>
										<div class="form-line">
											<select class="form-control" id="vat" name="currency">
												<?php
												

												foreach($this->VATCAT() as $row=>$x){
													extract($row);
													if($t_id == $template)
														echo '<option selected value="'.$row.'">'.$x[0].'</option>';
													else
														echo '<option value="'.$row.'">'.$x[0].'</option>';
												}
												
												?>
											</select>
										</div>
									</div>
									<div class="col-md-5">
										<label>Reference No.<span class="must">*</span></label>
										<div class="form-line">
											<input type="text" value="<?php echo $oi_ref_no;?>" id="ref_no" class="form-control" name="">
										</div>
									</div>
									
									<div class="col-md-12">
										<label>Remarks<span class="must">*</span></label>
										<div class="form-line">
											<textarea id="general_description" class="form-control"><?php echo $oi_general_description;?></textarea>
										</div>
									</div>
									
								</div>
								<div class="row">
							<?php 
								$desc1 = "";
								$amount1 = 0;
								$sql = "SELECT * FROM cn_invoice_item_description WHERE ii_rel = '$oi_rel' ORDER BY ii_id ASC";
								$select = $db->select($sql);
								if($db->num_rows())
								extract($select[0][0]);
							?>
							<div class="col-md-12">
								<label>Item<span class="must">*</span></label>
								<style type="text/css">
									.added-section:hover{
										background-color: red;
									}
								</style>
								<div id="worddescription">
									<div class="row">
										<div class="col-md-1" style="font-weight:bold;color:red;">
											#
										</div>
										<div class="col-md-6" style="font-weight:bold;color:red;">
											Details
										</div>
										<div class="col-md-2" style="font-weight:bold;color:red;">
											Qty
										</div>
										<div class="col-md-2" style="font-weight:bold;color:red;">
											UnitPrice <i>(VAT Inc)</i>
										</div>
									</div>
									<div class="row section">
										<div class="added-section" id="1">
											<div class="col-md-1">
												1.
											</div>
											<div class="col-md-6">
												<?php
													$db = new Db();
														echo'<select class="form-control" id="description1">';
														echo'<option value="">-------select------</option>';
														$select = $db->select("SELECT * from efris_goods ORDER BY eg_name ASC");
														foreach ($select[0] as $row) {
															extract($row);
															if ($eg_id == $ii_description)
																echo'<option selected value="'.$eg_id.'">'.$eg_name.'('.$eg_code.')</option>';
															else
															echo'<option value="'.$eg_id.'">'.$eg_name.'('.$eg_code.')</option>';	
														}
														echo '</select>';
													?>
											</div>
											<div class="col-md-2">
												<input autocomplete="off" type="number" value="<?php echo $ii_quantity;?>" id="quantity1" class="form-control"/>
											</div>
											<div class="col-md-2">
												<input autocomplete="off" type="text" min=0 value="<?php echo $ii_amount;?>" id="amount1" class="form-control"/>
											</div>
										</div>
										<?php 
											$db = new Db();
											$sql = "SELECT * FROM cn_invoice_item_description WHERE ii_rel = '$oi_rel' AND ii_id != '$ii_id' ORDER BY ii_id ASC";
											$select = $db->select($sql);
											//print_r($select);
											$x=1;
											foreach($select[0] as $row){
												extract($row);
												$x++;
												echo '<div class="clearfix"></div>';
												echo '<div class="added-section" id="'.$x.'">';
												echo '<div class="col-md-1 line-number" >'.$x.'.</div>';
												echo '<div class="col-md-6">';
												// <textarea autofocus class="form-control" id="description'.$x.'">'.$ii_description.'</textarea>

													$db = new Db();
														$db = new Db();
															echo'<select class="form-control" id="description'.$x.'">';
															echo'<option value="">-------select------</option>';
															$select = $db->select("SELECT * from efris_goods ORDER BY eg_name ASC");
															foreach ($select[0] as $row) {
																extract($row);
																if ($eg_id == $ii_description)
																	echo'<option selected value="'.$eg_id.'">'.$eg_name.'('.$eg_code.')</option>';
																else
																echo'<option value="'.$eg_id.'">'.$eg_name.'('.$eg_code.')</option>';	
															}
															echo '</select>';
														
													
												echo'</div>';
												echo '<div class="col-md-2"><input autocomplete="off" type="number" value="'.$ii_quantity.'" id="quantity'.$x.'" class="form-control"/></div>';
												echo '<div class="col-md-2"><input autocomplete="off" type="text" value="'.number_format($ii_amount).'" id="amount'.$x.'" class="form-control"/></div>';
												echo '<div class="col-md-2"><button type="button" class="text-danger" id="remove-wd"><i class="fa fa-fw fa-times"></i></button></div>';
												echo '</div>';

							
											}
										?>
									</div>
									<div class="col-md-12">
										<button type="button" id="plus-wd" class="btn btn-xs pull-right text-primary"><i class="fa fa-fw fa-plus"></i></button>
									</div>
								</div>
								<script type="text/javascript">
									$(function(){
										$(document).off('click','#plus-wd').on('click','#plus-wd',function(e){
											var random = Date.now();
											var html = '<div class="clearfix"></div>';
											html += '<div class="added-section" id="'+random+'">';
											html += '<div class="col-md-1 line-number"></div>';
											html += '<div class="col-md-6">';
											html += '<?php
													
													echo '<select class="form-control" id="description'; ?>'+random+'<?php echo '" style="">';
														
													$db = new Db();
														
														echo'<option value="">-------select------</option>';
														$select = $db->select("SELECT * from efris_goods ORDER BY eg_name ASC");
														foreach ($select[0] as $row) {
															extract($row);
															if ($eg_id == $ii_description)
																echo'<option selected value="'.$eg_id.'">'.$eg_name.'('.$eg_code.')</option>';
															else
															echo'<option value="'.$eg_id.'">'.$eg_name.'('.$eg_code.')</option>';	
														}
														echo '</select>';
														
													?>';
											html += '</div>';
											html += '<div class="col-md-2">';
											html += '<input autocomplete="off" type="number" value="" id="quantity'+random+'" class="form-control"/>';
											html += '</div>';
											html += '<div class="col-md-2">';
											html += '<input autocomplete="off" type="text" value="" id="amount'+random+'" class="form-control"/>';
											html += '</div>';
											html += '<div class="col-md-1"><button type="button" class="text-danger" id="remove-wd"><i class="fa fa-fw fa-times"></i></button></div>';
											html += '</div>';
											html += '</div>';

											$('#worddescription .section').append(html);
											$('#description'+random).focus();

											var count = 2;
											$('.line-number').each(function(){
												$(this).html(count+'. ');
												count++;
											});

										});

										$(document).off('click','#remove-wd').on('click','#remove-wd',function(e){
											var parent = $(this).parent().parent();
											parent.remove();

											var count = 2;
											$('.line-number').each(function(){
												$(this).html(count+'. ');
												count++;
											});
										});


									});
								</script>
							</div>

						</div>
										<br>
										<input type="hidden" value="<?php echo$oi_id?>" id="id" name="">
										<button id="editCnOtherInvoiceBtn" type="button" name="submit" class="btn btn-primary"><i class="fa fa-fw fa-save"></i> Save</button><span id="editOtherInvoiceStatus"></span>
									</div>

								</div>
								
								
							</form>
						</div>
					</div>
				</div>
			</div>
		</div>
			<script type="text/javascript">
			$(document).ready(function(){
	$(document).off('click','#editCnOtherInvoiceBtn').on('click', '#editCnOtherInvoiceBtn', function(e){

		var date = $('#date').val();
		var category = $('#category').val();
		var customer  = $('#customer').val();
		var pay_template  = $('#pay_template').val();
		var currency  = $('#currency').val();
		var vat  = $('#vat').val();
		var general_description  = $('#general_description').val();
		var id  = $('#id').val();
		var ref_no  = $('#ref_no').val();

        var counter=1;
		var counterArray = new Array();
		$('#worddescription .section').find(".added-section").each(function(){
			var findID = $(this).attr('id');
			//var id = findID.split("_")[1];
			//id = (!id)?"":"_"+id;
			counterArray.push(findID);
			//alert(findID);
		});


		var errors = new Array();
		// if(general_description==""){
		// 	errors.push("Select Customer Name");
		// }

		for(i=0; i<counterArray.length; i++){				
			var description1 = $('#description'+counterArray[i]).val();
			var quantity1 = $('#quantity'+counterArray[i]).val();				
			var amount1 = parseInt($('#amount'+counterArray[i]).val());

			if(!description1){
				errors.push("Line "+(i+1)+": Select Item");
			}
			if(!quantity1){
				errors.push("Line "+(i+1)+": Enter quantity");
			}
			if(!amount1){
				errors.push("Line "+(i+1)+": Enter Amount");
			}

			//alert(description1);
		}

//return 0;
		
		
		if(errors.length >= 1){
			alert("Please Review the following:\n "+errors.join("\n"));
		}else{
			//$('#addOtherInvoiceStatus').html('<img src="'+urlPath+'images/loading.gif" alt="loading..."/>');
			$(this).html("Editing Credit Note");
			$(this).attr("disabled", true);
			var form_data = new FormData();
			form_data.append('date', date);
			form_data.append('category', category);
			form_data.append('customer', customer);
			form_data.append('pay_template', pay_template);
			form_data.append('currency', currency);
			form_data.append('vat', vat);
			form_data.append('ref_no', ref_no);
			form_data.append('general_description', general_description);

			
			 form_data.append('totalItems', counterArray.length);
			for(i=0; i<counterArray.length; i++){
				form_data.append('description'+i, $('#description'+counterArray[i]).val());
				form_data.append('quantity'+i, $('#quantity'+counterArray[i]).val());
				form_data.append('amount'+i, $('#amount'+counterArray[i]).val());
				form_data.append('id', id);
			}

			$.ajax({
			//url: urlPath2+"ajax/addOtherInvoice.php",
			url: '<?php echo return_url().'ajax/editCnOtherInvoice.php'; ?>',
			type: "POST",
			data: form_data,
			contentType: false,
			cache: false,
			processData:false,
			success: function(data){
				//alert(data);
				location.reload();
				$('#editOtherInvoiceStatus').html('Successfully Saved');
			}
		
		});	
		}

	});


});
		</script>
	<?php
	}
public function editDebtNoteAction(){
		$id=portion(3);
		$db=new Db();
		$select=$db->select("select * from dn_other_invoice WHERE oi_id='$id'");
		if($db->num_rows()){
		extract($select[0][0]);
		//print_r($select);
		}
		if(isset($_POST['submit'])){
		
			$category = $_POST['category'];
			$customer = $_POST['customer'];
			$pay_template = $_POST['pay_template'];
			$title = $_POST['title'];
			$description = $_POST['description'];

			$time = time();
			$user = user_id();
			
			$errors = array();
			// if(empty($category)){
			// 	$errors[]="Select Category";
			// }
			
		

		if(empty($errors)){
			$db = new Db();
			$x = $db->update("dn_other_invoice",["oi_category"=>$category,"oi_customer"=>$customer,"oi_pay_template"=>$pay_template,"oi_title"=>$title,"oi_description"=>$description,"oi_added_by"=>$user,"oi_date_added"=>$time],["oi_id"=>$id]);
			
			if($x){
				FeedBack::success();
				FeedBack::refresh(3, return_url().'other-invoices/edit-invoice/'.$id);
			}else{
				FeedBack::error();
			}
			}else{
				FeedBack::errors($errors);
			}
		}
		?>
		<div class="col-md-12">
			<h3>&nbsp;</h3>
			<div class="col-lg-12">
			<div class="panel panel-default">
				<div class="panel-heading">
					
				</div>
				<div class="panel-body">
					<div class="row">
						<div class="col-lg-12">
							<form role="form" action="" method="post">
								<div class="form-group">
									<div id="must">All field with asterisk(*) are mandatory.</div>
										<div class="row">
										<div class="col-md-2">
										<label>Date<span class="must">*</span></label>
										<div class="form-line">
											<input class="form-control" style="height: 29px;" id="date" type="date" name="" value="<?php echo date('Y-m-d',$oi_date); ?>">
										</div>
									</div>
									<div class="col-md-2">
										<label>Category<span class="must">*</span></label>
										<div class="form-line">
											<select id="category" class="form-control">
												<?php
												echo '<option>-----select------</option>';	
												$db = new Db();
												$select = $db->select("SELECT * FROM categories");
												foreach ($select[0] as $row) {
													extract($row);
													if($cat_id == $oi_category)
													echo '<option selected value="'.$cat_id.'">'.$cat_name.'</option>';	
													else
													echo '<option value="'.$cat_id.'">'.$cat_name.'</option>';		
												}

												?>
											</select>
											</div>
									</div>
									<div class="col-md-3">
										<label>Customers<span class="must">*</span></label>
										<?php
												echo'<select  class="form-control" name="customer" id="customer">';
											$select = $db->select("SELECT * FROM customers where cust_category = '$oi_category' ORDER BY cust_name ASC");
											foreach($select[0] as $row){
												extract($row);
												if($oi_customer == $cust_id)
													echo '<option selected value="'.$cust_id.'"><b>'.$cust_name.'</b></option>';
												else
													echo '<option value="'.$cust_id.'">'.$cust_name.'</option>';
											}
											
										echo'</select>';
										?>
									</div>
									<script type="text/javascript">
										$(document).ready(function(){
											$('#category').change(function(){
												var category = $(this).val();
												var form_data = new FormData(); 
												form_data.append('category', category); 

												$.ajax({
										        	url: '<?php echo return_url().'ajax/invoice_details_by_category.php'; ?>',
										            type: "POST",
										            data: form_data,
										            contentType: false,
										            cache: false,
										            processData:false,
										            success: function(data){
										            	//alert(data);
										            	$('#customer').html(data);	
										            }
										        });
											});
										});
									</script>
									<div class="col-md-3">
										<label>Payment Template<span class="must">*</span></label>
										<div class="form-line">
											<select class="form-control" id="pay_template" name="pay_template">
												<?php
												$sql = "SELECT * FROM payment_information";
												$db = new Db();
												$select = $db->query($sql);
												foreach($select as $row){
													extract($row);
													if($t_id == $oi_template)
														echo '<option selected value="'.$t_id.'">'.$t_name.'</option>';
													else
														echo '<option value="'.$t_id.'">'.$t_name.'</option>';
												}
												
												?>
											</select>
										</div>
									</div>
									
									<div class="clearfix"></div>
									<div class="col-md-2">
										<label>Currency<span class="must">*</span></label>
										<div class="form-line">
											<select class="form-control" id="currency" name="currency">
												<?php
												$db = new Db();
												$sql = "SELECT * FROM dictionary WHERE d_category = '1' ORDER BY d_name ASC";
												$select = $db->select($sql);
												foreach($select[0] as $row){
													extract($row);
													if($oi_currency == $d_code)
													echo '<option selected value="'.$d_code.'">'.$d_name.' ('.$d_code.')</option>';
												else
													echo '<option value="'.$d_code.'">'.$d_name.' ('.$d_code.')</option>';
												}
												
												?>
											</select>
										</div>
									</div>
									<div class="col-md-2">
										<label>VAT<span class="must">*</span></label>
										<div class="form-line">
											<select class="form-control" id="vat" name="currency">
												<?php
												

												foreach($this->VATCAT() as $row=>$x){
													extract($row);
													if($t_id == $template)
														echo '<option selected value="'.$row.'">'.$x[0].'</option>';
													else
														echo '<option value="'.$row.'">'.$x[0].'</option>';
												}
												
												?>
											</select>
										</div>
									</div>
									<div class="col-md-2">
										<label>Reference No.<span class="must">*</span></label>
										<div class="form-line">
											<input type="text" value="<?php echo $oi_ref_no;?>" id="ref_no" class="form-control" name="">
										</div>
									</div>
									<div class="col-md-12">
										<label>Remarks<span class="must">*</span></label>
										<div class="form-line">
											<textarea id="general_description" class="form-control"><?php echo $oi_general_description;?></textarea>
										</div>
									</div>
									
								</div>
								<div class="row">
							<?php 
								$desc1 = "";
								$amount1 = 0;
								$sql = "SELECT * FROM dn_invoice_item_description WHERE ii_rel = '$oi_rel' ORDER BY ii_id ASC";
								$select = $db->select($sql);
								if($db->num_rows())
								extract($select[0][0]);
							?>
							<div class="col-md-12">
								<label>Item<span class="must">*</span></label>
								<style type="text/css">
									.added-section:hover{
										background-color: red;
									}
								</style>
								<div id="worddescription">
									<div class="row">
										<div class="col-md-1" style="font-weight:bold;color:red;">
											#
										</div>
										<div class="col-md-6" style="font-weight:bold;color:red;">
											Details
										</div>
										<div class="col-md-2" style="font-weight:bold;color:red;">
											Qty
										</div>
										<div class="col-md-1" style="font-weight:bold;color:red;">
											Unit Price <i>(VAT Inclusive)</i>
										</div>
									</div>
									<div class="row section">
										<div class="added-section" id="1">
											<div class="col-md-1">
												1.
											</div>
											<div class="col-md-6">
												<?php
													$db = new Db();
														echo'<select class="form-control" id="description1">';
														echo'<option value="">-------select------</option>';
														$select = $db->select("SELECT * from efris_goods ORDER BY eg_name ASC");
														foreach ($select[0] as $row) {
															extract($row);
															if ($eg_id == $ii_description)
																echo'<option selected value="'.$eg_id.'">'.$eg_name.'('.$eg_code.')</option>';
															else
															echo'<option value="'.$eg_id.'">'.$eg_name.'('.$eg_code.')</option>';	
														}
														echo '</select>';
													?>
											</div>
											<div class="col-md-2">
												<input autocomplete="off" type="number" value="<?php echo $ii_quantity;?>" id="quantity1" class="form-control"/>
											</div>
											<div class="col-md-2">
												<input autocomplete="off" type="text" min=0 value="<?php echo $ii_amount;?>" id="amount1" class="form-control"/>
											</div>
										</div>
										<?php 
											$db = new Db();
											$sql = "SELECT * FROM dn_invoice_item_description WHERE ii_rel = '$oi_rel' AND ii_id != '$ii_id' ORDER BY ii_id ASC";
											$select = $db->select($sql);
											//print_r($select);
											$x=1;
											foreach($select[0] as $row){
												extract($row);
												$x++;
												echo '<div class="clearfix"></div>';
												echo '<div class="added-section" id="'.$x.'">';
												echo '<div class="col-md-1 line-number" >'.$x.'.</div>';
												echo '<div class="col-md-6">';
												// <textarea autofocus class="form-control" id="description'.$x.'">'.$ii_description.'</textarea>

													$db = new Db();
														$db = new Db();
															echo'<select class="form-control" id="description'.$x.'">';
															echo'<option value="">-------select------</option>';
															$select = $db->select("SELECT * from efris_goods ORDER BY eg_name ASC");
															foreach ($select[0] as $row) {
																extract($row);
																if ($eg_id == $ii_description)
																	echo'<option selected value="'.$eg_id.'">'.$eg_name.'('.$eg_code.')</option>';
																else
																echo'<option value="'.$eg_id.'">'.$eg_name.'('.$eg_code.')</option>';	
															}
															echo '</select>';
														
													
												echo'</div>';
												echo '<div class="col-md-2"><input autocomplete="off" type="number" value="'.$ii_quantity.'" id="quantity'.$x.'" class="form-control"/></div>';
												echo '<div class="col-md-2"><input autocomplete="off" type="text" value="'.number_format($ii_amount).'" id="amount'.$x.'" class="form-control"/></div>';
												echo '<div class="col-md-2"><button type="button" class="text-danger" id="remove-wd"><i class="fa fa-fw fa-times"></i></button></div>';
												echo '</div>';

							
											}
										?>
									</div>
									<div class="col-md-12">
										<button type="button" id="plus-wd" class="btn btn-xs pull-right text-primary"><i class="fa fa-fw fa-plus"></i></button>
									</div>
								</div>
								<script type="text/javascript">
									$(function(){
										$(document).off('click','#plus-wd').on('click','#plus-wd',function(e){
											var random = Date.now();
											var html = '<div class="clearfix"></div>';
											html += '<div class="added-section" id="'+random+'">';
											html += '<div class="col-md-1 line-number"></div>';
											html += '<div class="col-md-6">';
											html += '<?php
													
													echo '<select class="form-control" id="description'; ?>'+random+'<?php echo '" style="">';
														
													$db = new Db();
														
														echo'<option value="">-------select------</option>';
														$select = $db->select("SELECT * from efris_goods ORDER BY eg_name ASC");
														foreach ($select[0] as $row) {
															extract($row);
															if ($eg_id == $ii_description)
																echo'<option selected value="'.$eg_id.'">'.$eg_name.'('.$eg_code.')</option>';
															else
															echo'<option value="'.$eg_id.'">'.$eg_name.'('.$eg_code.')</option>';	
														}
														echo '</select>';
														
													?>';
											html += '</div>';
											html += '<div class="col-md-2">';
											html += '<input autocomplete="off" type="number" value="" id="quantity'+random+'" class="form-control"/>';
											html += '</div>';
											html += '<div class="col-md-2">';
											html += '<input autocomplete="off" type="text" value="" id="amount'+random+'" class="form-control"/>';
											html += '</div>';
											html += '<div class="col-md-1"><button type="button" class="text-danger" id="remove-wd"><i class="fa fa-fw fa-times"></i></button></div>';
											html += '</div>';
											html += '</div>';

											$('#worddescription .section').append(html);
											$('#description'+random).focus();

											var count = 2;
											$('.line-number').each(function(){
												$(this).html(count+'. ');
												count++;
											});

										});

										$(document).off('click','#remove-wd').on('click','#remove-wd',function(e){
											var parent = $(this).parent().parent();
											parent.remove();

											var count = 2;
											$('.line-number').each(function(){
												$(this).html(count+'. ');
												count++;
											});
										});


									});
								</script>
							</div>

						</div>
										<br>
										<input type="hidden" value="<?php echo$oi_id?>" id="id" name="">
										<button id="editDnOtherInvoiceBtn" type="button" name="submit" class="btn btn-primary"><i class="fa fa-fw fa-save"></i> Save</button><span id="editOtherInvoiceStatus"></span>
									</div>

								</div>
								
								
							</form>
						</div>
					</div>
				</div>
			</div>
		</div>
			<script type="text/javascript">
			$(document).ready(function(){
	$(document).off('click','#editDnOtherInvoiceBtn').on('click', '#editDnOtherInvoiceBtn', function(e){

		var date = $('#date').val();
		var category = $('#category').val();
		var customer  = $('#customer').val();
		var pay_template  = $('#pay_template').val();
		var currency  = $('#currency').val();
		var vat  = $('#vat').val();
		var general_description  = $('#general_description').val();
		var id  = $('#id').val();
		var ref_no  = $('#ref_no').val();

        var counter=1;
		var counterArray = new Array();
		$('#worddescription .section').find(".added-section").each(function(){
			var findID = $(this).attr('id');
			//var id = findID.split("_")[1];
			//id = (!id)?"":"_"+id;
			counterArray.push(findID);
			//alert(findID);
		});


		var errors = new Array();
		// if(general_description==""){
		// 	errors.push("Select Customer Name");
		// }

		for(i=0; i<counterArray.length; i++){				
			var description1 = $('#description'+counterArray[i]).val();
			var quantity1 = $('#quantity'+counterArray[i]).val();				
			var amount1 = parseInt($('#amount'+counterArray[i]).val());

			if(!description1){
				errors.push("Line "+(i+1)+": Select Item");
			}
			if(!quantity1){
				errors.push("Line "+(i+1)+": Enter quantity");
			}
			if(!amount1){
				errors.push("Line "+(i+1)+": Enter Amount");
			}

			//alert(description1);
		}

//return 0;
		
		
		if(errors.length >= 1){
			alert("Please Review the following:\n "+errors.join("\n"));
		}else{
			//$('#addOtherInvoiceStatus').html('<img src="'+urlPath+'images/loading.gif" alt="loading..."/>');
			$(this).html("Editing Debt Note");
			$(this).attr("disabled", true);
			var form_data = new FormData();
			form_data.append('date', date);
			form_data.append('category', category);
			form_data.append('customer', customer);
			form_data.append('pay_template', pay_template);
			form_data.append('currency', currency);
			form_data.append('vat', vat);
			form_data.append('general_description', general_description);
			form_data.append('ref_no', ref_no);

			
			 form_data.append('totalItems', counterArray.length);
			for(i=0; i<counterArray.length; i++){
				form_data.append('description'+i, $('#description'+counterArray[i]).val());
				form_data.append('quantity'+i, $('#quantity'+counterArray[i]).val());
				form_data.append('amount'+i, $('#amount'+counterArray[i]).val());
				form_data.append('id', id);
			}

			$.ajax({
			//url: urlPath2+"ajax/addOtherInvoice.php",
			url: '<?php echo return_url().'ajax/editDnOtherInvoice.php'; ?>',
			type: "POST",
			data: form_data,
			contentType: false,
			cache: false,
			processData:false,
			success: function(data){
				//alert(data);
				location.reload();
				$('#editOtherInvoiceStatus').html('Successfully Saved');
			}
		
		});	
		}

	});


});
		</script>
	<?php
	}
}
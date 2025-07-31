<?php
Class Receipts extends BeforeAndAfter{
	public $page = "RECEIPTS";
	
	
	public $vat_list = array(
		'220006'=>'VAT Input Local',
		'220007'=>'VAT Input - imports',
		'220008'=>'VAT Output Energy Sales',
		'220009'=>'VAT Output Other Sales',
		'220010'=>'VAT Return',
		'220011'=>'Unclaimed VAT',
		'220012'=>'VAT Output Imported Power',
		'220013'=>'VAT - (URA) Deferred Claims',
		'220014'=>'VAT Withheld',
	);

	public $payment_type = array(
		'Cash'=>'Cash',
		'Cheque'=>'Cheque',
		'EFT'=>'EFT',
	);
		
	public $bank = array(
		'170001'=>'SCB, Opeartions A/C, USD',
		'170002'=>'SCB, Opeartions A/C, UGX',
		'170004'=>'SCB, Fixed Deposit A/C, USD',
		'170005'=>'SCB, Fixed Deposit A/C, UGX',
		'170006'=>'SCB Tariff Stabilization Funds',
		'170007'=>'ABSA Industrial Parks',
		'170021'=>'Citi Bank, Operations A/C, USD',
		'170022'=>'Citi Bank, Operations A/C, UGX',
		'170023'=>'Citi Bank, Fixed Deeposits, USD',
		'170024'=>'Citi Bank, Fixed Deposits UGX',
		'170025'=>'Liquidity Financing',
		'170026'=>'Bank of Uganda - Project funds',
		'170027'=>'Bank of Uganda Projects Accounts (US $)',
		'170028'=>'SCB, Fixed Deposits A/C , EUR',
		'170029'=>'SCB, Deposits A/C , EUR',
		'170030'=>'Stanbic, Fixed Deposit A/C , UGX',
		'170031'=>'Stanbic, Operations A/C , EUR',
		'170032'=>'Stanbic, Operations A/C , UGX',
		'170033'=>'Stanbic, Operations A/C , USD',
		'170034'=>'Barclays Bank (U) Ltd, UGX',
		'170035'=>'Barclays Bank Uganda Ltd, USD',
		'170036'=>'Barclays Bank Uganda Ltd, EUR',
		'170037'=>'Stanbic Bank, Projects Operational Account (UGX)',
		'170038'=>'Barclays Bank, Fixed Deposit Account (UGX)',
		'170039'=>'Citi Bank, Operations A/C, Euro',
	);
	
	public function __construct(){
		$access = new AccessRights();
		$access->pageAccess(user_id(), $this->page, 'V');
	}
	
	public function getLinks(){
		$page = "RECEIPTS";
		$links = array(
			array(
				"link_name"=>"Add Receipt", 
				"link_address"=>"receipts/add-receipt",
				"link_icon"=>"fa-plus",
				"link_page"=>$page,
				"link_right"=>"A",
			),
			array(
				"link_name"=>"View Receipts", 
				"link_address"=>"receipts/view-receipts",
				"link_icon"=>"fa-eye",
				"link_page"=>$page,
				"link_right"=>"V",
			),
			array(
				"link_name"=>"Cancelled Receipts", 
				"link_address"=>"receipts/view-cancelled-receipts",
				"link_icon"=>"fa-times",
				"link_page"=>$page,
				"link_right"=>"V",
			),
			array(
				"link_name"=>"Receipt Category", 
				"link_address"=>"receipts/view-receipt-categories",
				"link_icon"=>"fa-eye",
				"link_page"=>$page,
				"link_right"=>"V",
			),
			array(
				"link_name"=>"Receipt Category Items", 
				"link_address"=>"receipts/view-category-items",
				"link_icon"=>"fa-eye",
				"link_page"=>$page,
				"link_right"=>"V",
			),
			array(
				"link_name"=>"Account2 List", 
				"link_address"=>"receipts/view-account2-list",
				"link_icon"=>"fa-eye",
				"link_page"=>$page,
				"link_right"=>"V",
			)
		);
		
		return $links;
	}

	public function receiptNo(){
		$db = new Db();
		$select = $db->select("SELECT TOP 1 rec_receipt_no FROM receipts ORDER BY rec_id DESC");
		extract($select[0][0]);
		$r = str_replace('REC','', $rec_receipt_no);
		$r += 1;
		return 'REC'.str_pad($r, 5, "0", STR_PAD_LEFT);
	}
	
	public function addReceiptAction(){
		error_reporting(1);
		$dbss = new DbSunServer();		

		$customers = array();
        $db = new Db();
        $select = $db->select("SELECT * FROM customer ORDER BY customer_short_name ASC");
        foreach($select[0] as $row){
            extract($row);
            $customers[$customer_id] = $customer_short_name;
        }

		echo '<a href="'.return_url().'receipts/view-receipts"><i class="fa fa-fw fa-arrow-left"></i>View Receipts</a>';
		if(isset($_POST['submit'])){
			
			$date = $_POST['date'];
			$receipt_no = $this->receiptNo(); //$_POST['receipt_no'];
			$category = $_POST['category'];
			$customer_name = $_POST['customer_name'];
			$amount = $_POST['amount'];
			$description = $_POST['description'];
			$curr = $_POST['curr'];
			$vat_status = $_POST['vat_status'];
			$section = $_POST['section'];
			$project = $_POST['project'];
			$staff = $_POST['staff'];
			$bank_id = $_POST['bank_id'];
			$new = $_POST['new'];
			$account2 = $_POST['account2'];
			$new_account2 = $_POST['new_account2'];
			$new_account22 = $_POST['new_account22'];
			
			$time = time();
			$user = user_id();


			$db = new Db();
			
			$errors = array();

			if(empty($date)){
				$errors[]="Enter Date";
			}
			if(empty($receipt_no)){
				$errors[]="Enter Receipt No.";
			}
			if(empty($category)){
				$errors[]="Select Category";
			}
			if(empty($customer_name) && empty($new)){
				$errors[]="Select Customer name or Add new customer";
			}
			if(($vat_status=="")){
				$errors[]="Select VAT status";
			}
			if(($curr=="")){
				$errors[]="Select Currency";
			}
			if(($bank_id=="")){
				$errors[]="Select Bank";
			}
			if(($account2=="" && ($new_account2==""||$new_account22==""))){
				$errors[]="Select Account 2 or Add ";
			}

			if($this->isThere("receipts", ["rec_receipt_no"=>$receipt_no])){
				$errors[]="Receipt No.($receipt_no) already exists";
			}
			
			if(empty($errors)){			
				
				if(!empty($new_account2)){
					//insert into the items table				
					$insert = $db->insert("account2", ["account_name"=>$new_account2,"account_number"=>$new_account22, "account_date_added"=>time(), "account_added_by"=>user_id()]);
					$uid = user_id();
					$select = $db->select("SELECT TOP 1 account_id as ac2 FROM account2 WHERE account_added_by='$uid' ORDER BY account_id DESC");
					extract($select[0][0]);
					$account2 = $ac2;
				}
				
				
				if(!empty($new)){
					//insert into the items table				
					$insert = $db->insert("receipt_category_item", ["rci_name"=>$new, "rci_date_added"=>time(), "rci_added_by"=>user_id(), "rci_category"=>$category]);
					
					$select = $db->select("SELECT TOP 1 rci_id FROM receipt_category_item ORDER BY rci_id DESC");
					extract($select[0][0]);
					$customer_name = $rci_id;
				}
				
				$ref =  time().user_id();
				$user_id = user_id();
				//$db->update("receipt_amount", ["ra_rec_id"=>$ref], ["ra_added_by"=>user_id(), "ra_"]);
				$db->query("UPDATE receipt_amount SET ra_ref = '$ref' WHERE ra_ref IS NULL AND ra_added_by = '$user_id'");
			
				$db = new Db();
				$am = str_replace(',', '', $amount);
				$cus =  "";//$this->rgf("customer", $customer_name, "customer_id", "customer_short_name");
				$x = $db->insert("receipts",["rec_date_added"=>$time, "rec_added_by"=>user_id(),"rec_category"=>$category, "rec_receipt_no"=>$receipt_no.''.$cus, "rec_amount"=>$am, "rec_customer_id"=>$customer_name, "rec_description"=>$description, "rec_date"=>strtotime($date), "rec_vat_status"=>$vat_status, "rec_staff"=>$staff, "rec_project"=>$project, "rec_section"=>$section, "rec_curr"=>$curr, "rec_bank"=>$bank_id, "rec_account2"=>$account2, "rec_ref"=>$ref]);
								
				if(!$db->error()){
					FeedBack::success();
					FeedBack::refresh(3);
					//FeedBack::refresh(3, return_url().'receipts/all-customers');
				}else{
					FeedBack::error($db->error());
				}
			}else{
				FeedBack::errors($errors);
			}
		}
		?>
		<div class="col-md-12">
			<br/>
			<div class="col-lg-12">
			<div class="panel panel-default">
				<div class="panel-heading">
					Add Receipt Form
				</div>
				<div class="panel-body">
					<div id="must">All field with asterisk(*) are mandatory.</div>

					<div class="row">
						<div class="col-lg-12">
							<form role="form" action="" method="post">
								<div class="row">
									<div class="col-md-6">
										<div class="form-group">
											<label>Account One<span class="must">*</span></label>
											<div class="form-line">
												<select class="select2 form-control"  name="bank_id" id="account_name">
													<option value="">Select</option>
													<?php
													
													foreach($this->bank as $bank => $ba){
														extract($row);
														
														if($bank_id == $bank)
															echo '<option value="'.$bank.'" selected="selected">'.$ba.' ('.$bank.')</option>';
														else
															echo '<option value="'.$bank.'" >'.$ba.' ('.$bank.')</option>';
													} 
													
													if($category == '180000')
														echo '<option selected="selected" value="180000">Control Account (180000)</option>';
													else
														echo '<option value="180000">Control Account (180000)</option>';
													?>
												</select>
											</div>
										</div>
									</div>
									<div class="col-md-6">
										<div class="form-group">
											<label>Account Two<span class="must">*</span></label>
											<div class="form-line" id="newListAccount">
												<select class="select2 form-control"  name="account2" id="list1">
													<option value="">Select</option>
													<?php
													$db = new Db();
													$select = $db->select("SELECT * FROM account2");
													foreach($select[0] as $row){
														extract($row);
														
														if($account2 == $account_id)
															echo '<option value="'.$account_id.'" selected="selected">'.$account_name.' ('.$account_number.')</option>';
														else
															echo '<option value="'.$account_id.'" >'.$account_name.' ('.$account_number.')</option>';
													} 
													?>
												</select>
											</div>
											<div id="newTextAccount" class="form-line">
												<input id="text1" type="text" class="form-control" placeholder="Enter New Account 2 name" name="new_account2" value="<?php echo $new_account2; ?>"/>
												<input id="text2" type="text" class="form-control" placeholder="Enter Account Number" name="new_account22" value="<?php echo $new_account22; ?>"/>
											</div>
											<div id="list_tigger2" style="margin-top:10px; color:blue; text-decoration:underline; cursor:pointer;"><i class="fa fa-fw fa-plus" ></i> Add New Account 2</div>
											<div id="list_tigger3" style="margin-top:10px; color:blue; text-decoration:underline; cursor:pointer;"><i class="fa fa-fw fa-list" ></i> Use Exiting List</div>
											<script type="text/javascript">
												
												$("#list_tigger3").hide();
												$("#newTextAccount").hide();
												$("#list_tigger2").click(function(){
													$(this).hide();
													$('#newListAccount').hide();
													$('#list_tigger2').hide();
													$('#list_tigger3').show();
													$("#newTextAccount").show();
													$("#list1").val("");
												});
												$("#list_tigger3").click(function(){
													$(this).hide();
													$('#newListAccount').show();
													$('#list_tigger2').show();
													$('#list_tigger3').hide();
													$("#newTextAccount").hide();
													$("#text1").val("");
													$("#text2").val("");
												});
												
											</script>
											<?php if(!empty($new_account2)){ ?>
											<script type="text/javascript">
												$('#newListAccount').hide();
												$('#list_tigger2').hide();
												$('#list_tigger3').show();
												$("#newTextAccount").show();
											</script>
											<?php } ?>
										</div>
									</div>
									<div class="clearfix"></div>
									<div class="col-md-2">
										<div class="form-group">
											<?php
											if(empty($receipt_no)){
												$receipt_no = $this->receiptNo();
											}
											?>
											<label>Receipt No.<span class="must">*</span></label>
											<div class="form-line">
												<input class="form-control" type="text" name="receipt_no" id="receipt_no" placeholder="" value="<?php echo @$receipt_no; ?>">
											</div>
										</div>
									</div>
									<div class="col-md-2">
										<div class="form-group">
											<label>Date<span class="must">*</span></label>
											<div class="form-line">
												<input class="form-control" type="date" name="date" id="date" style="padding-left:0; padding-right:5px;" placeholder="" value="<?php echo date('Y-m-d'); ?>">
											</div>
										</div>
									</div>
									<div class="col-md-2">
										<div class="form-group">
											<label>Category<span class="must">*</span></label>
											<div class="form-line">
												<select class="select2 form-control"  name="category" id="list">
													<option value="">Select</option>
													<?php
													$sel = $db->select("SELECT * FROM receipt_category ORDER BY rc_name ASC");
													foreach($sel[0] as $row){
														extract($row);
														
														if($category == $rc_id)
															echo '<option value="'.$rc_id.'" selected="selected">'.$rc_name.'</option>';
														else
															echo '<option value="'.$rc_id.'">'.$rc_name.'</option>';
													} 
													?>
												</select>
											</div>
										</div>
									</div>
									<div class="col-md-3">
										<div class="form-group">
											<label>Customer Name<span class="must">*</span></label>	
											<div class="" id="list_of_customers">
												<select class="select2 form-control" name="customer_name" id="loadList">
													<option value="">Select</option>
													<?php
													if($category==3){
														$select = $db->select("SELECT * FROM customer ORDER BY customer_full_name ASC");
														//echo $db->num_rows();
														foreach($select[0] as $row){
															extract($row);
															if($customer_name == $customer_id)
																echo '<option selected="selected" value="'.$customer_id.'">'.$customer_full_name.'</option>';
															else
																echo '<option value="'.$customer_id.'">'.$customer_full_name.'</option>';
														}
													}elseif($category == 4){
														$db=new Db();
														$select = $db->select("SELECT * FROM of_customer ORDER BY ofc_customer_name ASC");
														//echo $db->num_rows();
														foreach($select[0] as $row){
															extract($row);
															if($customer_name == $ofc_id)
																echo '<option selected="selected" value="'.$ofc_id.'">'.$ofc_customer_name.'</option>';
															else
																echo '<option value="'.$ofc_id.'">'.$ofc_customer_name.'</option>';
														}
													}else{
														$sql = "SELECT * FROM receipt_category_item WHERE rci_category = '$category'";
														$select = $db->select($sql);
														$no = 1;
														if($db->num_rows()){
														foreach($select[0] as $row){
															extract($row);
															if($customer_name == $rci_id)
																echo '<option selected = "selected" value="'.$rci_id.'">'.$rci_name.'</option>';
															else
																echo '<option value="'.$rci_id.'">'.$rci_name.'</option>';
														}
														}else{	
															echo '<option value="">No Customers</option>';
														}
													}
													?>
												</select>
												<div id="list_tigger" style="margin-top:10px; color:blue; text-decoration:underline; cursor:pointer;"><i class="fa fa-fw fa-plus" ></i> Add New Customer</div>
											</div>
											<div id="new_customer">
												<input type="text" id="new" name="new" class="form-control" placeholder="Enter New Customer" value="<?php echo $new; ?>"/>	
												<?php if(empty($telephone)) $telephone = '+256'; ?>
												<input type="text" id="telephone" name="new" class="form-control" placeholder="Enter Telephone Customer" value="<?php echo $telephone; ?>"/>										
												<input type="text" id="email" name="email" class="form-control" placeholder="Enter Email" value="<?php echo $email; ?>"/>										
												<div id="existing" style="margin-top:10px; color:blue; text-decoration:underline; cursor:pointer;"><i class="fa fa-fw fa-list" ></i>Use exiting List</div>
											</div>
											
											<script type="text/javascript">
												$('#new_customer').hide();
												$('#list_tigger').hide();
												$(document).ready(function() {
													$("#list_tigger").click(function() {
														$('#list_of_customers').hide();
														$('#new_customer').show();
														$('#loadList').html('<option value="">Select</option>');
													});
													
													$("#existing").click(function() {
														$('#list_of_customers').show();
														$('#new_customer').hide();
														$('#new').val("");
													});
													
													$("#list").change(function(event){
														var category = $("#list").val();
														//$("#new").val("");
														
														if(category != 3 && category != 4 && category != ""){
															$('#list_tigger').show();
														}else{
															$('#list_tigger').hide();
															$('#list_of_customers').show();
															$('#new_customer').hide();
															$('#new').val("");
														}
														
														var request;
														if (window.XMLHttpRequest){//for Chrome, Firefox, IE7+, Opera, Safari
															request = new XMLHttpRequest();
														}
														else{//for IE5, IE6
															request = new ActiveXObject("Microsoft.XMLHTTP");
														}	
														request.open("POST",'<?php echo return_url();?>ajax/loadItems.php', true);
														request.setRequestHeader("Content-type", "application/x-www-form-urlencoded");	

														request.onreadystatechange = function() {
															if(request.readyState == 4 && request.status == 200) {
																//4 = The connection is complete, the data was sent or retrieved.
																//200 = The file has been retrieved and you are free to do something with it
																$("#loadList").html(request.responseText);
															}
														}

														request.send("category="+category);
													});
												});
												
											</script>
											
											<?php if(isset($_POST['new']) && !empty($new)){?>
											<script type="text/javascript">
											$(document).ready(function() {
												$('#list_of_customers').hide();
												$('#new_customer').show();
												$('#loadList').html('<option value="">Select</option>');
											});
											</script>
											<?php } ?>
											
											<?php if(isset($_POST['customer_name']) && !empty($customer_name)){?>
											<script type="text/javascript">
											$(document).ready(function() {
												$('#list_of_customers').show();
												$('#new_customer').hide();
												$('#new').val("");
											});
											</script>
											<?php } ?>
										</div>
									</div>
									<div class="col-md-2" style="display:none;">
										<div class="form-group">
											<label>Amount<span class="must">*</span></label>	
											<div class="form-line">
												<input class="number form-control" type="" value="<?php echo $amount; ?>" id="amount">
											</div>
										</div>
									</div>
									<div class="col-md-3" style="padding-left:0">
										<div class="form-group">
											<label>Curr<span class="must">*</span></label>	
											<div class="form-line">												
												<select class="form-control" id="curr" >
													<option value="">Select</option>
													<option <?php if($curr=="UGX") echo ' selected="selected" '; ?> value="UGX">UGX</option>
													<option <?php if($curr=="USD") echo ' selected="selected" '; ?> value="USD">USD</option>
													<option <?php if($curr=="EUR") echo ' selected="selected" '; ?> value="EUR">EUR</option>
												</select>
											</div>
										</div>
									</div>
									<div class="clearfix"></div>
								
									<div class="col-lg-12">
										  <br>
							       
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
							                    <div class="col-md-4" style="font-weight:bold;color:red;">
							                        Amount
							                    </div>
							                    <div class="col-md-3" style="font-weight:bold;color:red;">
							                        Description
							                    </div>
							                   
							    
							                </div>
							                <div class="row section">
							                    <div class="added-section" id="1">
							                        <div class="col-md-1">
							                            1.
							                        </div>
							                        <div class="col-md-4">
							                             <div class="form-line">
							                                <input type="text" class="form-control number" value="" id="amount1" name="">
							                            </div>  
							                        </div>
							                       
							                        <div class="col-md-6">
							                            <div class="form-line">
							                               <textarea id="description1" class="form-control"></textarea>
							                            </div>
							                        </div>
							                
							                    </div>
							                    
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
							                       
							                        html += '</div>';
							                        html += '<div class="col-md-4"><input type="text" class="form-control" value="" id="amount'+random+'" name=""></div>';
							                          html += '<div class="col-md-6"><textarea id="description'+random+'" class="form-control"></textarea></div>';
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
									
									<div class="col-lg-12">
										<div class="form-group">
											<label>Description / Narration<span class="must"></span></label>
											<div class="form-line">
												<textarea class="form-control" id="description"><?php echo $description; ?></textarea>
											</div>
										</div>
									</div>
									<div class="col-lg-3">
										<div class="form-group">
											<label>Payment Type<span class="must"></span></label>
											<div class="form-line">
												<select class="select2 form-control" id="paymentType">
													<option value="">Select</option>
													<?php 
													foreach($this->payment_type as $vat => $list){
														if($vat==$rec_payment_type)
														echo '<option selected value="'.$vat.'">'.$list.'</option>';
															else
														echo '<option value="'.$vat.'">'.$list.'</option>';		
													}
													?>
												</select>
											</div>
										</div>
									</div>
									<div class="col-lg-3">
										<div class="form-group">
											<label>Exchange Rate<span class="must"></span></label>
											<div class="form-line">
												<input class="form-control" type="" id="exchangeRate" value=""/>
											</div>
										</div>
									</div>
									<div class="clearfix"></div>
									
									<div class="col-lg-3">
										<div class="form-group">
											<label>VAT Status<span class="must"></span></label>
											<div class="form-line">
												<select class="select2 form-control" id="vat_status">
													<option value="">Select</option>
													<option <?php if($vat_status==1) echo ' selected="selected" '; ?> value="2">Vatable</option>
													<option <?php if($vat_status==2) echo ' selected="selected" '; ?> value="2">Not Vatable</option>
													<?php 
													foreach($this->vat_list as $vat => $list){
														echo '<option value="'.$vat.'">'.$list.' ('.$vat.')</option>';
													}
													?>
												</select>
											</div>
										</div>
									</div>
									<?php 
										$sections = $dbss->select("SELECT ANL_CODE, NAME FROM IDG_ANL_CODE WHERE ANL_CAT_ID='13'"); 
										$staffz = $dbss->select("SELECT ANL_CODE, NAME FROM IDG_ANL_CODE WHERE ANL_CAT_ID='09'"); 
										$projects = $dbss->select("SELECT ANL_CODE, NAME FROM IDG_ANL_CODE WHERE ANL_CAT_ID='14'"); 
										
										
										
									?>
									<div class="col-lg-3">
										<div class="form-group">
											<label>Section<span class="must"></span></label>
											<div class="form-line">
												<select class="form-control select2" id="section">
													<option value="">Select</option>
													<?php 
													foreach($sections[0] as $section123){
														extract($section123);
														if($ANL_CODE == $section)
															echo '<option selected = "selected" value="'.$ANL_CODE.'">'.$ANL_CODE.' - '.$NAME.'</option>';
														else
															echo '<option value="'.$ANL_CODE.'">'.$ANL_CODE.' - '.$NAME.'</option>';
													}
													?>
												</select>
											</div>
										</div>
									</div>
									<div class="col-lg-3">
										<div class="form-group">
											<label>Staff<span class="must"></span></label>
											<div class="form-line">
												<select class="form-control select2" id="staff">
													<option value="">Select</option>
													<?php 
													foreach($staffz[0] as $section123){
														extract($section123);
														if($ANL_CODE == $staff)
															echo '<option selected = "selected" value="'.$ANL_CODE.'">'.$ANL_CODE.' - '.$NAME.'</option>';
														else
															echo '<option value="'.$ANL_CODE.'">'.$ANL_CODE.' - '.$NAME.'</option>';
													}
													?>
												</select>
											</div>
										</div>
									</div>
									<div class="col-lg-3">
										<div class="form-group">
											<label>Project<span class="must"></span></label>
											<div class="form-line">
												<select class="form-control select4" id="project">
													<option value="">Select</option>
													<?php 
													foreach($projects[0] as $section123){
														extract($section123);
														if($ANL_CODE == $project)
															echo '<option selected = "selected" value="'.$ANL_CODE.'">'.$ANL_CODE.' - '.$NAME.'</option>';
														else
															echo '<option value="'.$ANL_CODE.'">'.$ANL_CODE.' - '.$NAME.'</option>';
													}
													?>
												</select>
											</div>
										</div>
									</div>
									<script>
										$('.select2').select2();
										$('.select4').select2();
										$('.select3').select2();
									</script>
									<div class="col-lg-12">
										<button type="button" name="submit" id="addReceiptBtn" class="btn btn-primary"><i class="fa fa-fw fa-save"></i> Save</button><span id="addReceiptStatus"></span>
									</div>
									<script type="text/javascript">
									    $(document).off('click','#addReceiptBtn').on('click', '#addReceiptBtn', function(e){
									        var urlPath = $('#urlPath').val();	
									        var account_name = $('#account_name').val();
									        var account2 = $('#list1').val();
									        var new_account2 = $('#text1').val();
									        var new_account22 = $('#text2').val();
									        var receipt_no = $('#receipt_no').val();
									        var date = $('#date').val();
									        var category = $('#list').val();
									        var customer_name = $('#loadList').val();
									        var new_customer = $('#new').val();
									        var telephone = $('#telephone').val();
									        var description = $('#description').val();
									        var vat_status = $('#vat_status').val();
									        var section = $('#section').val();
									        var staff = $('#staff').val();
									        var project = $('#project').val();
									        var curr = $('#curr').val();
									        var paymentType = $('#paymentType').val();
									        var exchangeRate = $('#exchangeRate').val();
									        var email = $('#email').val();
									       
									        
									        var errors = new Array();
									        
									        
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
									        if(category==""){
									            errors.push("Select Category");
									        }
									        if(customer_name=="" && new_customer==""){
									            errors.push("Select Customer name or Add new customer");
									        }

									        if(new_customer != ""){
									        	if(telephone==""){
									        		errors.push("Enter Telephone number");
									        	}
									        }
									        if(vat_status==""){
									            errors.push("Select VAT status");
									        }
									        if(curr==""){
									            errors.push("Select Currency");
									        }
									        if(account_name==""){
									            errors.push("Select Bank");
									        }
									        if(paymentType==""){
									            errors.push("Select Bank");
									        }
									         if(account2=="" && (new_account2=="" || new_account22=="")){
									            errors.push("Select Account 2 or Add");
									        }
									        
									        for(i=0; i<counterArray.length; i++){               
									            var amount = $('#amount'+counterArray[i]).val();
									            var description = $('#description'+counterArray[i]).val();
									           
									            if(!amount){
									                errors.push("Line "+(i+1)+": Enter Amount");
									            }
									            if(!description){
									                errors.push("Line "+(i+1)+": Enter Description");
									            }
									    
									        }

									        
									        
									        if(errors.length >= 1){
									            alert("Please Review the following:\n "+errors.join("\n"));
									        }else{
									            $('#addReceiptStatus').html('<img src="'+urlPath+'images/loading.gif" alt="loading..."/>');	
									            $(this).html("Adding Receipt");
									            $(this).attr("disabled", true);
									            var form_data = new FormData();
									            form_data.append('account_name', account_name);
									            form_data.append('account2', account2);
									            form_data.append('new_account22', new_account22);
									            form_data.append('receipt_no', receipt_no);
									            form_data.append('date', date);
									            form_data.append('category', category);
									            form_data.append('customer_name', customer_name);
									            form_data.append('new_customer', new_customer);
									            form_data.append('description', description);
									            form_data.append('vat_status', vat_status);
									            form_data.append('section', section);
									            form_data.append('staff', staff);
									            form_data.append('project', project);
									            form_data.append('curr', curr);
									            form_data.append('paymentType', paymentType);
									            form_data.append('exchangeRate', exchangeRate);
									            form_data.append('telephone', telephone);
									            form_data.append('email', email);

									             form_data.append('totalItems', counterArray.length);
									            for(i=0; i<counterArray.length; i++){
									                form_data.append('amount'+i, $('#amount'+counterArray[i]).val());
									                form_data.append('description'+i, $('#description'+counterArray[i]).val());
									                
									            }

									            $.ajax({
									            url: urlPath+"ajax/addReceipt.php",
									            type: "POST",
									            data: form_data,
									            contentType: false,
									            cache: false,
									            processData:false,
									            success: function(data){
									                //alert(data);
									                location.reload();
									                $('#addReceiptStatus').html('Successfully Saved');
									            }
									        
									        }); 
									        }

									    });
									    </script>   
								</div>
							</form>
						</div>
					</div>
				</div>
			</div>
		</div>
		
		

	<?php
	}
	
	public function editReceiptAction(){
		$id = portion(3);
		$db = new Db();
		$select = $db->select("SELECT * FROM receipts WHERE rec_id = '$id'");
		extract($select[0][0]);
		$bank_id = $rec_bank;
		$account2 = $rec_account2;
		$receipt_no = $rec_receipt_no;
		$category = $rec_category;
		$customer_name = $rec_customer_id;
		$curr = $rec_curr;
		$description = $rec_description;
		$vat_status = $rec_vat_status;
		$section = $rec_section;
		$staff = $rec_staff;
		$project = $rec_project;
		
		//echo '<pre>';
		//print_r($select[0]);
		//echo '</pre>';
		//error_reporting(1);
		$dbss = new DbSunServer();		

		$customers = array();
        $db = new Db();
        $select = $db->select("SELECT * FROM customer ORDER BY customer_short_name ASC");
        foreach($select[0] as $row){
            extract($row);
            $customers[$customer_id] = $customer_short_name;
        }

		echo '<a href="'.return_url().'receipts/view-receipts"><i class="fa fa-fw fa-arrow-left"></i>View All Receipts</a>';
		
		echo '<a href="'.return_url().'receipts/view-receipt/'.$id.'"><i class="fa fa-fw fa-eye"></i>View Receipt</a>';
		if(isset($_POST['submit'])){
			
			$date = $_POST['date'];
			$receipt_no = $this->receiptNo(); //$_POST['receipt_no'];
			$category = $_POST['category'];
			$customer_name = $_POST['customer_name'];
			$amount = $_POST['amount'];
			$description = $_POST['description'];
			$curr = $_POST['curr'];
			$vat_status = $_POST['vat_status'];
			$section = $_POST['section'];
			$project = $_POST['project'];
			$staff = $_POST['staff'];
			$bank_id = $_POST['bank_id'];
			$new = $_POST['new'];
			$account2 = $_POST['account2'];
			$new_account2 = $_POST['new_account2'];
			$new_account22 = $_POST['new_account22'];
			
			$time = time();
			$user = user_id();


			$db = new Db();
			
			$errors = array();

			if(empty($date)){
				$errors[]="Enter Date";
			}
			if(empty($receipt_no)){
				$errors[]="Enter Receipt No.";
			}
			if(empty($category)){
				$errors[]="Select Category";
			}
			if(empty($customer_name) && empty($new)){
				$errors[]="Select Customer name or Add new customer";
			}
			if(($vat_status=="")){
				$errors[]="Select VAT status";
			}
			if(($curr=="")){
				$errors[]="Select Currency";
			}
			if(($bank_id=="")){
				$errors[]="Select Bank";
			}
			if(($account2=="" && ($new_account2==""||$new_account22==""))){
				$errors[]="Select Account 2 or Add ";
			}

			if($this->isThere("receipts", ["rec_receipt_no"=>$receipt_no])){
				$errors[]="Receipt No.($receipt_no) already exists";
			}
			
			if(empty($errors)){			
				
				if(!empty($new_account2)){
					//insert into the items table				
					$insert = $db->insert("account2", ["account_name"=>$new_account2,"account_number"=>$new_account22, "account_date_added"=>time(), "account_added_by"=>user_id()]);
					$uid = user_id();
					$select = $db->select("SELECT TOP 1 account_id as ac2 FROM account2 WHERE account_added_by='$uid' ORDER BY account_id DESC");
					extract($select[0][0]);
					$account2 = $ac2;
				}
				
				
				if(!empty($new)){
					//insert into the items table				
					$insert = $db->insert("receipt_category_item", ["rci_name"=>$new, "rci_date_added"=>time(), "rci_added_by"=>user_id(), "rci_category"=>$category]);
					
					$select = $db->select("SELECT TOP 1 rci_id FROM receipt_category_item ORDER BY rci_id DESC");
					extract($select[0][0]);
					$customer_name = $rci_id;
				}
				
				$ref =  $rec_ref;
				$user_id = user_id();
				//$db->update("receipt_amount", ["ra_rec_id"=>$ref], ["ra_added_by"=>user_id(), "ra_"]);
				$db->query("UPDATE receipt_amount SET ra_ref = '$ref' WHERE ra_ref IS NULL AND ra_added_by = '$user_id'");
			
				$db = new Db();
				$am = str_replace(',', '', $amount);
				$cus =  "";//$this->rgf("customer", $customer_name, "customer_id", "customer_short_name");
				$x = $db->update("receipts",["rec_date_added"=>$time, "rec_added_by"=>user_id(),"rec_category"=>$category, "rec_receipt_no"=>$receipt_no.''.$cus, "rec_amount"=>$am, "rec_customer_id"=>$customer_name, "rec_description"=>$description, "rec_date"=>strtotime($date), "rec_vat_status"=>$vat_status, "rec_staff"=>$staff, "rec_project"=>$project, "rec_section"=>$section, "rec_curr"=>$curr, "rec_bank"=>$bank_id, "rec_account2"=>$account2, "rec_ref"=>$ref],["rec_id"=>$id]);
								
				if(!$db->error()){
					FeedBack::success();
					FeedBack::refresh(3);
					//FeedBack::refresh(3, return_url().'receipts/all-customers');
				}else{
					FeedBack::error($db->error());
				}
			}else{
				FeedBack::errors($errors);
			}
		}
		?>
		<div class="col-md-12">
			<br/>
			<div class="col-lg-12">
			<div class="panel panel-default">
				<div class="panel-heading">
					Add Receipt Form
				</div>
				<div class="panel-body">
					<div id="must">All field with asterisk(*) are mandatory.</div>

					<div class="row">
						<div class="col-lg-12">
							<form role="form" action="" method="post">
								<div class="row">
									<div class="col-md-6">
										<div class="form-group">
											<label>Account One<span class="must">*</span></label>
											<div class="form-line">
												<select class="form-control"  name="bank_id" id="account_name">
													<option value="">Select</option>
													<?php
													
													foreach($this->bank as $bank => $ba){
														extract($row);
														
														if($bank_id == $bank)
															echo '<option value="'.$bank.'" selected="selected">'.$ba.' ('.$bank.')</option>';
														else
															echo '<option value="'.$bank.'" >'.$ba.' ('.$bank.')</option>';
													} 
													
													if($bank_id == '180000')
														echo '<option selected="selected" value="180000">Control Account (180000)</option>';
													else
														echo '<option value="180000">Control Account (180000)</option>';
													?>
												</select>
											</div>
										</div>
									</div>
									<div class="col-md-6">
										<div class="form-group">
											<label>Account Two<span class="must">*</span></label>
											<div class="form-line" id="newListAccount">
												<select class="form-control"  name="account2" id="list1">
													<option value="">Select</option>
													<?php
													$db = new Db();
													$select = $db->select("SELECT * FROM account2");
													foreach($select[0] as $row){
														extract($row);
														
														if($account2 == $account_id)
															echo '<option value="'.$account_id.'" selected="selected">'.$account_name.' ('.$account_number.')</option>';
														else
															echo '<option value="'.$account_id.'" >'.$account_name.' ('.$account_number.')</option>';
													} 
													?>
												</select>
											</div>
											<div id="newTextAccount" class="form-line">
												<input id="text1" type="text" class="form-control" placeholder="Enter New Account 2 name" name="new_account2" value="<?php echo $new_account2; ?>"/>
												<input id="text2" type="text" class="form-control" placeholder="Enter Account Number" name="new_account22" value="<?php echo $new_account22; ?>"/>
											</div>
											<div id="list_tigger2" style="margin-top:10px; color:blue; text-decoration:underline; cursor:pointer;"><i class="fa fa-fw fa-plus" ></i> Add New Account 2</div>
											<div id="list_tigger3" style="margin-top:10px; color:blue; text-decoration:underline; cursor:pointer;"><i class="fa fa-fw fa-list" ></i> Use Exiting List</div>
											<script type="text/javascript">
												
												$("#list_tigger3").hide();
												$("#newTextAccount").hide();
												$("#list_tigger2").click(function(){
													$(this).hide();
													$('#newListAccount').hide();
													$('#list_tigger2').hide();
													$('#list_tigger3').show();
													$("#newTextAccount").show();
													$("#list1").val("");
												});
												$("#list_tigger3").click(function(){
													$(this).hide();
													$('#newListAccount').show();
													$('#list_tigger2').show();
													$('#list_tigger3').hide();
													$("#newTextAccount").hide();
													$("#text1").val("");
													$("#text2").val("");
												});
												
											</script>
											<?php if(!empty($new_account2)){ ?>
											<script type="text/javascript">
												$('#newListAccount').hide();
												$('#list_tigger2').hide();
												$('#list_tigger3').show();
												$("#newTextAccount").show();
											</script>
											<?php } ?>											
											<span><i class=""></i></span>
										</div>
									</div>
									<div class="clearfix"></div>
									<div class="col-md-2">
										<div class="form-group">
											<?php
											if(empty($receipt_no)){
												$receipt_no = $this->receiptNo();
											}
											?>
											<label>Receipt No.<span class="must">*</span></label>
											<div class="form-line">
												<input class="form-control" disabled="" type="text" name="receipt_no" id="receipt_no" placeholder="" value="<?php echo @$receipt_no; ?>">
											</div>
										</div>
									</div>
									<div class="col-md-2">
										<div class="form-group">
											<label>Date<span class="must">*</span></label>
											<div class="form-line">
												<input disabled class="form-control" type="date" id="date" style="padding-left:0; padding-right:5px;" placeholder="" value="<?php echo date('Y-m-d', $rec_date); ?>">
												<input class="form-control" type="hidden" name="date" style="padding-left:0; padding-right:5px;" placeholder="" value="<?php echo date('Y-m-d', $rec_date); ?>">
											</div>
										</div>
									</div>
									<div class="col-md-2">
										<div class="form-group">
											<label>Category<span class="must">*</span></label>
											<div class="form-line">
												<select class="form-control select2"  name="category" id="list">
													<option value="">Select</option>
													<?php
													$sel = $db->select("SELECT * FROM receipt_category ORDER BY rc_name ASC");
													foreach($sel[0] as $row){
														extract($row);
														
														if($category == $rc_id)
															echo '<option value="'.$rc_id.'" selected="selected">'.$rc_name.'</option>';
														else
															echo '<option value="'.$rc_id.'">'.$rc_name.'</option>';
													} 
													?>
												</select>
											</div>
										</div>
									</div>
									<div class="col-md-3">
										<div class="form-group">
											<label>Customer Name<span class="must">*</span></label>	
											<div class="" id="list_of_customers">
												<select class="form-control select2" name="customer_name" id="loadList">
													<option value="">Select</option>
													<?php
													if($category==3){
														$select = $db->select("SELECT * FROM customer ORDER BY customer_full_name ASC");
														//echo $db->num_rows();
														foreach($select[0] as $row){
															extract($row);
															if($customer_name == $customer_id)
																echo '<option selected="selected" value="'.$customer_id.'">'.$customer_full_name.'</option>';
															else
																echo '<option value="'.$customer_id.'">'.$customer_full_name.'</option>';
														}
													}elseif($category == 4){
														$db=new Db();
														$select = $db->select("SELECT * FROM of_customer ORDER BY ofc_customer_name ASC");
														//echo $db->num_rows();
														foreach($select[0] as $row){
															extract($row);
															if($customer_name == $ofc_id)
																echo '<option selected="selected" value="'.$ofc_id.'">'.$ofc_customer_name.'</option>';
															else
																echo '<option value="'.$ofc_id.'">'.$ofc_customer_name.'</option>';
														}
													}else{
														$sql = "SELECT * FROM receipt_category_item WHERE rci_category = '$category'";
														$select = $db->select($sql);
														$no = 1;
														if($db->num_rows()){
														foreach($select[0] as $row){
															extract($row);
															if($customer_name == $rci_id)
																echo '<option selected = "selected" value="'.$rci_id.'">'.$rci_name.'</option>';
															else
																echo '<option value="'.$rci_id.'">'.$rci_name.'</option>';
														}
														}else{	
															echo '<option value="">No Customers</option>';
														}
													}
													?>
												</select>
												<div id="list_tigger" style="margin-top:10px; color:blue; text-decoration:underline; cursor:pointer;"><i class="fa fa-fw fa-plus" ></i> Add New Customer</div>
											</div>
											<div id="new_customer">
												<input type="text" id="new" name="new" class="form-control" placeholder="Enter New Customer" value="<?php echo $new; ?>"/>	
												<?php if(empty($telephone)) $telephone = '+256'; ?>
												<input type="text" id="telephone" name="new" class="form-control" placeholder="Enter Telephone Customer" value="<?php echo $telephone; ?>"/>											
												<div id="existing" style="margin-top:10px; color:blue; text-decoration:underline; cursor:pointer;"><i class="fa fa-fw fa-list" ></i>Use exiting List</div>
											</div>
											
											<script type="text/javascript">
												$('#new_customer').hide();
												$('#list_tigger').hide();
												$(document).ready(function() {
													$("#list_tigger").click(function() {
														$('#list_of_customers').hide();
														$('#new_customer').show();
														$('#loadList').html('<option value="">Select</option>');
													});
													
													$("#existing").click(function() {
														$('#list_of_customers').show();
														$('#new_customer').hide();
														$('#new').val("");
													});
													
													$("#list").change(function(event){
														var category = $("#list").val();
														//$("#new").val("");
														
														if(category != 3 && category != 4 && category != ""){
															$('#list_tigger').show();
														}else{
															$('#list_tigger').hide();
															$('#list_of_customers').show();
															$('#new_customer').hide();
															$('#new').val("");
														}
														
														var request;
														if (window.XMLHttpRequest){//for Chrome, Firefox, IE7+, Opera, Safari
															request = new XMLHttpRequest();
														}
														else{//for IE5, IE6
															request = new ActiveXObject("Microsoft.XMLHTTP");
														}	
														request.open("POST",'<?php echo return_url();?>ajax/loadItems.php', true);
														request.setRequestHeader("Content-type", "application/x-www-form-urlencoded");	

														request.onreadystatechange = function() {
															if(request.readyState == 4 && request.status == 200) {
																//4 = The connection is complete, the data was sent or retrieved.
																//200 = The file has been retrieved and you are free to do something with it
																$("#loadList").html(request.responseText);
															}
														}

														request.send("category="+category);
													});
												});
												
											</script>
											
											<?php if(isset($_POST['new']) && !empty($new)){?>
											<script type="text/javascript">
											$(document).ready(function() {
												$('#list_of_customers').hide();
												$('#new_customer').show();
												$('#loadList').html('<option value="">Select</option>');
											});
											</script>
											<?php } ?>
											
											<?php if(isset($_POST['customer_name']) && !empty($customer_name)){?>
											<script type="text/javascript">
											$(document).ready(function() {
												$('#list_of_customers').show();
												$('#new_customer').hide();
												$('#new').val("");
											});
											</script>
											<?php } ?>
										</div>
									</div>
									<div class="col-md-2" style="display:none;">
										<div class="form-group">
											<label>Amount<span class="must">*</span></label>	
											<div class="form-line">
												<input class="number form-control" type="" value="<?php echo $amount; ?>" id="amount">
											</div>
										</div>
									</div>
									<div class="col-md-3" style="padding-left:0">
										<div class="form-group">
											<label>Curr<span class="must">*</span></label>	
											<div class="form-line">												
												<select class="form-control" id="curr" >
													<option value="">Select</option>
													<option <?php if($rec_curr=="UGX") echo ' selected="selected" '; ?> value="UGX">UGX</option>
													<option <?php if($rec_curr=="USD") echo ' selected="selected" '; ?> value="USD">USD</option>
													<option <?php if($rec_curr=="EUR") echo ' selected="selected" '; ?> value="EUR">EURO</option>
												</select>
											</div>
										</div>
									</div>
									<div class="clearfix"></div>
								
									<div class="col-lg-12">
									  <br>
							       		 <?php
							            if($rec_ref){
							            $db = new Db();
							            $desc1 = "";
							            $sql = "SELECT * FROM receipt_amount WHERE ra_ref = '$rec_ref' ORDER BY ra_id ASC";
							            $select = $db->select($sql);
							            if($db->num_rows())
							            extract($select[0][0]);
							          }
							          ?>
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
							                    <div class="col-md-4" style="font-weight:bold;color:red;">
							                        Amount
							                    </div>
							                    <div class="col-md-3" style="font-weight:bold;color:red;">
							                        Description
							                    </div>
							                   
							    
							                </div>
							                <div class="row section">
							                    <div class="added-section" id="1">
							                        <div class="col-md-1">
							                            1.
							                        </div>
							                        <div class="col-md-4">
							                             <div class="form-line">
							                                <input type="text" class="form-control number" value="<?php echo $ra_amount;?>" id="amount1" name="">
							                            </div>  
							                        </div>
							                       
							                        <div class="col-md-6">
							                            <div class="form-line">
							                               <textarea id="description1" class="form-control"><?php echo $ra_desc;?></textarea>
							                            </div>
							                        </div>
							                
							                    </div>

							                     <?php 
						                            if($ra_ref){
						                            	//echo ">>>>".$ra_id;
						                            	//echo "SELECT * FROM receipt_amount WHERE ra_ref = '$rec_ref' AND ra_id != '$ra_id' ORDER BY ra_id ASC";
						                            $sql = "SELECT * FROM receipt_amount WHERE ra_ref = '$rec_ref' AND ra_id != '$ra_id' ORDER BY ra_id ASC";
						                            $selectx = $db->select($sql);
						                            $x=1;
						                            foreach($selectx[0] as $row){
						                                extract($row);
						                                $x++;
						                                echo '<div class="clearfix"></div>';
						                                echo '<div class="added-section" id="'.$x.'">';
						                                echo '<div class="col-md-1 line-number" >'.$x.'.</div>';
						                                echo '<div class="col-md-4">';
						                               		echo ' <input type="text" class="form-control number" value="'.$ra_amount.'" id="amount'.$x.'" name="">';
						                                 echo'</div>';
						                                  echo'<div class="col-md-6">
							                                    <div class="form-line">
							                                         <textarea id="description'.$x.'" class="form-control">'.$ra_desc.'</textarea>
							                                    </div>
						                                    </div>';
								                            echo'<div class="col-md-1">
								                                    <button type="button" class="text-danger" id="remove-wd"><i class="fa fa-fw fa-times"></i></button>
								                                </div>';
						                                echo'</div>';
						                              // echo'<div class="clearfix"></div>'; 
						                                
						                            }
						            
						                            }
						                        ?>
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
							                        html += '<div class="added-section" id="'+random+'">';
							                        html += '<div class="col-md-1 line-number"></div>';
							                       
							                        html += '</div>';
							                        html += '<div class="col-md-4"><input type="text" class="form-control" value="" id="amount'+random+'" name=""></div>';
							                          html += '<div class="col-md-6"><textarea id="description'+random+'" class="form-control"></textarea></div>';
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
									
									<div class="col-lg-12">
										<div class="form-group">
											<label>Description / Narration<span class="must"></span></label>
											<div class="form-line">
												<textarea class="form-control" id="description"><?php echo $description; ?></textarea>
											</div>
										</div>
									</div>
									<div class="col-lg-3">
										<div class="form-group">
											<label>Payment Type<span class="must"></span></label>
											<div class="form-line">
												<select class="select2 form-control" id="vat_status">
													<option value="">Select</option>
													<?php 
													foreach($this->payment_type as $vat => $list){
														if($vat==$rec_payment_type)
														echo '<option selected value="'.$vat.'">'.$list.'</option>';
															else
														echo '<option value="'.$vat.'">'.$list.'</option>';		
													}
													?>
												</select>
											</div>
										</div>
									</div>
									<div class="clearfix"></div>
									<div class="col-lg-3">
										<div class="form-group">
											<label>VAT Status<span class="must"></span></label>
											<div class="form-line">
												<select class="select2 form-control" id="vat_status">
													<option value="">Select</option>
													<option <?php if($rec_vat_status==1) echo ' selected="selected" '; ?> value="2">Vatable</option>
													<option <?php if($rec_vat_status==2) echo ' selected="selected" '; ?> value="2">Not Vatable</option>
													<?php 
													foreach($this->vat_list as $vat => $list){
														if($vat==$rec_vat_status)
														echo '<option selected value="'.$vat.'">'.$list.' ('.$vat.')</option>';
															else
														echo '<option value="'.$vat.'">'.$list.' ('.$vat.')</option>';		
													}
													?>
												</select>
											</div>
										</div>
									</div>
									<?php 
										$sections = $dbss->select("SELECT ANL_CODE, NAME FROM IDG_ANL_CODE WHERE ANL_CAT_ID='13'"); 
										$staffz = $dbss->select("SELECT ANL_CODE, NAME FROM IDG_ANL_CODE WHERE ANL_CAT_ID='09'"); 
										$projects = $dbss->select("SELECT ANL_CODE, NAME FROM IDG_ANL_CODE WHERE ANL_CAT_ID='14'"); 
										
										
										
									?>
									<div class="col-lg-3">
										<div class="form-group">
											<label>Section<span class="must"></span></label>
											<div class="form-line">
												<select class="form-control select2" id="section">
													<option value="">Select</option>
													<?php 
													foreach($sections[0] as $section123){
														extract($section123);
														if(trim($ANL_CODE) == trim($rec_section))
															echo '<option selected value="'.$ANL_CODE.'">'.$ANL_CODE.' - '.$NAME.'</option>';
														else
															echo '<option value="'.$ANL_CODE.'">'.$ANL_CODE.' - '.$NAME.'</option>';
													}
													?>
												</select>
											</div>
										</div>
									</div>
									<div class="col-lg-3">
										<div class="form-group">
											<label>Staff<span class="must"></span></label>
											<div class="form-line">
												<select class="form-control select2" id="staff">
													<option value="">Select</option>
													<?php 
													foreach($staffz[0] as $section123){
														extract($section123);
														if(trim($ANL_CODE) == trim($rec_staff))
															echo '<option selected = "selected" value="'.$ANL_CODE.'">'.$ANL_CODE.' - '.$NAME.'</option>';
														else
															echo '<option value="'.$ANL_CODE.'">'.$ANL_CODE.' - '.$NAME.'</option>';
													}
													?>
												</select>
											</div>
										</div>
									</div>
									<div class="col-lg-3">
										<div class="form-group">
											<label>Project<span class="must"></span></label>
											<div class="form-line">
												<select class="form-control select4" id="project">
													<option value="">Select</option>
													<?php 
													foreach($projects[0] as $section123){
														extract($section123);
														if(trim($ANL_CODE) == trim($rec_project))
															echo '<option selected = "selected" value="'.$ANL_CODE.'">'.$ANL_CODE.' - '.$NAME.'</option>';
														else
															echo '<option value="'.$ANL_CODE.'">'.$ANL_CODE.' - '.$NAME.'</option>';
													}
													?>
												</select>
											</div>
										</div>
									</div>
									<script>
										$('.select2').select2();
										$('.select4').select2();
										$('.select3').select2();
									</script>
									<div class="col-lg-12">
										<input type="hidden" id="reference" value="<?php echo$rec_ref;?>">	
										<button type="button" name="submit" id="editReceiptBtn" class="btn btn-primary"><i class="fa fa-fw fa-save"></i> Save</button><span id="addReceiptStatus"></span>
									</div>
									<script type="text/javascript">
									    $(document).off('click','#editReceiptBtn').on('click', '#editReceiptBtn', function(e){
									        var urlPath = $('#urlPath').val();	
									        var account_name = $('#account_name').val();
									        var account2 = $('#list1').val();
									        var new_account2 = $('#text1').val();
									        var new_account22 = $('#text2').val();
									        var receipt_no = $('#receipt_no').val();
									        var date = $('#date').val();
									        var category = $('#list').val();
									        var customer_name = $('#loadList').val();
									        var new_customer = $('#new').val();
									        var description = $('#description').val();
									        var vat_status = $('#vat_status').val();
									        var section = $('#section').val();
									        var staff = $('#staff').val();
									        var project = $('#project').val();
									        var curr = $('#curr').val();
									        var reference = $('#reference').val();

									       
									        
									        var errors = new Array();
									        
									        
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
									        if(category==""){
									            errors.push("Select Category");
									        }
									        if(customer_name=="" && new_customer==""){
									            errors.push("Select Customer name or Add new customer");
									        }
									        if(vat_status==""){
									            errors.push("Select VAT status");
									        }
									        if(curr==""){
									            errors.push("Select Currency");
									        }
									        if(account_name==""){
									            errors.push("Select Bank");
									        }
									         if(account2=="" && (new_account2=="" || new_account22=="")){
									            errors.push("Select Account 2 or Add");
									        }
									        
									        for(i=0; i<counterArray.length; i++){               
									            var amount = $('#amount'+counterArray[i]).val();
									            var description = $('#description'+counterArray[i]).val();
									           
									            if(!amount){
									                errors.push("Line "+(i+1)+": Enter Amount");
									            }
									            if(!description){
									                errors.push("Line "+(i+1)+": Enter Description");
									            }
									    
									        }

									        
									        
									        if(errors.length >= 1){
									            alert("Please Review the following:\n "+errors.join("\n"));
									        }else{
									            $('#addReceiptStatus').html('<img src="'+urlPath+'images/loading.gif" alt="loading..."/>');	
									            $(this).html("Saving Receipt");
									            $(this).attr("disabled", true);
									            var form_data = new FormData();
									            form_data.append('account_name', account_name);
									            form_data.append('account2', account2);
									            form_data.append('new_account22', new_account22);
									            form_data.append('receipt_no', receipt_no);
									            form_data.append('date', date);
									            form_data.append('category', category);
									            form_data.append('customer_name', customer_name);
									            form_data.append('new_customer', new_customer);
									            form_data.append('description', description);
									            form_data.append('vat_status', vat_status);
									            form_data.append('section', section);
									            form_data.append('staff', staff);
									            form_data.append('project', project);
									            form_data.append('curr', curr);
									            form_data.append('reference', reference);

									             form_data.append('totalItems', counterArray.length);
									            for(i=0; i<counterArray.length; i++){
									                form_data.append('amount'+i, $('#amount'+counterArray[i]).val());
									                form_data.append('description'+i, $('#description'+counterArray[i]).val());
									                
									            }

									            $.ajax({
									            url: urlPath+"ajax/editReceipt.php",
									            type: "POST",
									            data: form_data,
									            contentType: false,
									            cache: false,
									            processData:false,
									            success: function(data){
									                //alert(data);
									                location.reload();
									                $('#addReceiptStatus').html('Successfully Saved');
									            }
									        
									        }); 
									        }

									    });
									    </script>   
									</div>
								</div>
							</form>
						</div>
					</div>
				</div>
			</div>
		</div>
		
		
		

	<?php
	}
	
	public function editReceiptOLDAction(){
		echo '<a href="'.return_url().'receipts/view-receipts"><i class="fa fa-fw fa-arrow-left"></i>View Receipts</a>';

		$id = portion(3);
		$db = new Db();
		$select = $db->select("SELECT * FROM receipts WHERE rec_id = '$id' ");
		extract($select[0][0]);

		$date = date('Y-m-d', $rec_date_added);
		$receipt_no = $rec_receipt_no; //$_POST['receipt_no'];
		$category = $rec_category;
		$customer_name = $rec_customer;
		$amount = $rec_amount;
		$description = $rec_description;

		if(isset($_POST['submit'])){
			
			$date = $_POST['date'];
			$receipt_no = $this->receiptNo(); //$_POST['receipt_no'];
			$category = $_POST['category'];
			$customer_name = $_POST['customer_name'];
			$amount = $_POST['amount'];
			$description = $_POST['description'];
			
			$time = time();
			$user = user_id();


			$db = new Db();
			
			$errors = array();

			if(empty($date)){
				$errors[]="Enter Date";
			}
			if(empty($receipt_no)){
				$errors[]="Enter Receipt No.";
			}
			if(empty($category)){
				$errors[]="Select Category";
			}
			if(empty($customer_name)){
				$errors[]="Select Customer name or Add New Customer";
			}
			if(($amount=="")){
				$errors[]="Enter Amount";
			}

			if($this->isThereEdit("receipts", ["rec_receipt_no"=>$receipt_no, "rec_id"=>$id])){
				$errors[]="Receipt No.($receipt_no) already exists";
			}
			
			if(empty($errors)){
				$db = new Db();
				$am = str_replace(',', '', $amount);
				$x = $db->update("receipts",["rec_date_added"=>$time, "rec_added_by"=>user_id(),"rec_category"=>$category, "rec_receipt_no"=>$receipt_no, "rec_amount"=>$am, "rec_customer_id"=>$customer_id, "rec_description"=>$description], ["rec_id"=>$id]);
								
				if(!$db->error()){
					FeedBack::success();
					FeedBack::refresh(3);
					//FeedBack::refresh(3, return_url().'receipts/all-customers');
				}else{
					FeedBack::error($db->error());
				}
			}else{
				FeedBack::errors($errors);
			}
		}
		?>
		<div class="col-md-12">
			<br/>
			<div class="col-lg-12">
			<div class="panel panel-default">
				<div class="panel-heading">
					Edit Receipt Form
				</div>
				<div class="panel-body">
					<div id="must">All field with asterisk(*) are mandatory.</div>

					<div class="row">
						<div class="col-lg-12">
							<form role="form" action="" method="post">
								<div class="row">
									<div class="col-md-2">
										<div class="form-group">
											<?php
											if(empty($receipt_no)){
												$receipt_no = $this->receiptNo();
											}
											?>
											<label>Receipt No.<span class="must">*</span></label>
											<div class="form-line">
												<input class="form-control" disabled="" type="text" name="receipt_no" placeholder="" value="<?php echo @$receipt_no; ?>">
											</div>
										</div>
									</div>
									<div class="col-md-3">
										<div class="form-group">
											<label>Date<span class="must">*</span></label>
											<div class="form-line">
												<input class="form-control" type="date" name="date" placeholder="" value="<?php echo @$date; ?>">
											</div>
										</div>
									</div>
									<div class="col-md-2">
										<div class="form-group">
											<label>Category<span class="must">*</span></label>
											<div class="form-line">
												<select class="form-control"  name="category">
													<option value="2">Energy Sales</option>
												</select>
											</div>
										</div>
									</div>
									<div class="col-md-3">
										<div class="form-group">
											<label>Customer Name<span class="must">*</span></label>	
											<div class="form-line">
												<select class="form-control" name="customer_name">
													<option value="">Select</option>
													<?php
													$i=0;
													foreach($customers as $id => $name){
														if($id == $customer_name)
															echo '<option value="'.$id.'" selected="selected">'.($name).'</option>';
														else
															echo '<option value="'.$id.'">'.($name).'</option>';
													}
													?>
												</select>
											</div>
										</div>
									</div>
									<div class="col-md-2">
										<div class="form-group">
											<label>Amount<span class="must">*</span></label>	
											<div class="form-line">
												<input class="number form-control" type="" value="<?php echo $amount; ?>" name="amount">
											</div>
										</div>
									</div>
									<div class="clearfix"></div>
									<div class="col-lg-12">
										<div class="form-group">
											<label>Description<span class="must"></span></label>
											<div class="form-line">
												<textarea class="form-control" name="description"><?php echo $description; ?></textarea>
											</div>
										</div>
									</div>
									
									<div class="col-lg-12">
										<button onclick = "" type="submit" name="submit" class="btn btn-primary"><i class="fa fa-fw fa-save"></i> Save</button>
									</div>
								</div>
							</form>
						</div>
					</div>
				</div>
			</div>
		</div>
	<?php
	}

	public function cancelReceiptAction(){
		// echo '<a href="'.return_url().'receipts/view-receipts"><i class="fa fa-fw fa-arrow-left"></i>View Receipts</a>';
		// echo '&nbsp; &nbsp; ';
		// echo '<a href="'.return_url().'receipts/add-receipt"><i class="fa fa-fw fa-plus"></i>Add Receipt</a>';
		// echo '&nbsp; &nbsp; ';
		// echo '<a href="'.return_url().'receipts/view-receipt-categories"><i class="fa fa-fw fa-eye"></i>View Receipt Categories</a>';
		// echo '&nbsp; &nbsp; ';
		$id = portion(3);


		$db = new Db();
		$select = $db->select("SELECT * FROM receipts WHERE rec_id = '$id' ");
		extract($select[0][0]);

		if(!$rec_cancelled){

		echo '<h3>Cancel Receipt <span style="color:red">'.$this->rgf("receipts", $id, "rec_id", "rec_receipt_no").'</span>:</h3>';

		echo  '<textarea id="comment" style="width:100%;height:100px;" placeholder="Enter Reason for cancelling."></textarea>';

		echo '<button id="cancelReceipt" data-id="'.$id.'" class="btn btn-xs btn-danger"><i class="fa fa-fw fa-times"></i>Cancel Receipt</button>';
		echo '<div id="cancelReceiptStatus"></div>';

		}


		$date = date('Y-m-d', $rec_date_added);
		$receipt_no = $rec_receipt_no; //$_POST['receipt_no'];
		$category = $rec_category;
		$customer_name = $rec_customer;
		$amount = $rec_amount;
		$description = $rec_description;


		if(isset($_POST['submit'])){
			
			$date = $_POST['date'];
			$receipt_no = $this->receiptNo(); //$_POST['receipt_no'];
			$category = $_POST['category'];
			$customer_name = $_POST['customer_name'];
			$amount = $_POST['amount'];
			$description = $_POST['description'];
			
			$time = time();
			$user = user_id();


			$db = new Db();
			
			$errors = array();

			if(empty($date)){
				$errors[]="Enter Date";
			}
			if(empty($receipt_no)){
				$errors[]="Enter Receipt No.";
			}
			if(empty($category)){
				$errors[]="Select Category";
			}
			if(empty($customer_name)){
				$errors[]="Select Customer name";
			}
			if(($amount=="")){
				$errors[]="Enter Amount";
			}

			if($this->isThereEdit("receipts", ["rec_receipt_no"=>$receipt_no, "rec_id"=>$id])){
				$errors[]="Receipt No.($receipt_no) already exists";
			}
			
			if(empty($errors)){
				$db = new Db();
				$am = str_replace(',', '', $amount);
				$x = $db->update("receipts",["rec_date_added"=>$time, "rec_added_by"=>user_id(),"rec_category"=>$category, "rec_receipt_no"=>$receipt_no, "rec_amount"=>$am, "rec_customer_id"=>$customer_id, "rec_description"=>$description], ["rec_id"=>$id]);
								
				if(!$db->error()){
					FeedBack::success();
					FeedBack::refresh(3);
					//FeedBack::refresh(3, return_url().'receipts/all-customers');
				}else{
					FeedBack::error($db->error());
				}
			}else{
				FeedBack::errors($errors);
			}
		}
		?>
		<div class="col-md-12">
			<?php 
						
		
			?>
			<br/>
			<div class="col-lg-12">
			<div id="printMe" style="width:700px;margin:auto;<?php if($rec_cancelled) echo 'color:red !important;'; ?>" >
			<style>@page {size: A5;  font-size:12px; @top-left {content: "";}} </style>
			<div class="panel panel-default">
				
				<div class="panel-body">

					<div class="row">
						<div class="col-lg-12" style="position:relative">							
							<img src="<?php display_url();?>images/uetcl_header.png" width="100%">
							<div class="" style="background-color:#ccc;color:#000;border:5px double black; width:150px; margin:20px auto; text-align:center; padding:2px 10px;font-weight:bold;<?php if($rec_cancelled) echo 'color:red !important;'; ?>"><?php if($rec_cancelled) echo 'CANCELLED '; ?> RECEIPT</div>
							<div style="padding-bottom:2px;margin:0;">
							<b>Date:</b> <span style="color:blue; border-bottom:2px dotted #000;padding:0 10px;<?php if($rec_cancelled) echo 'color:red !important;'; ?>"><?php echo date('Y-m-d', $rec_date); ?></span>
							</div>
							
							<div style="padding-bottom:2px;margin:0;">
							<b>Receipt No.:</b> <span style="color:blue; border-bottom:2px dotted #000;padding:0 10px;<?php if($rec_cancelled) echo 'color:red !important;'; ?>"><?php echo $rec_receipt_no; ?></span>
							</div>
							<br/><br/>
							<div style="font-size:14px;">
							<b>Received with Thanks From:</b><p style="color:blue; border-bottom:2px dotted black;<?php if($rec_cancelled) echo 'color:red !important;'; ?>"> 
							<?php 
							//echo $this->rgf($rec_category, $rec_customer_id);
							echo $this->receiptCat($rec_category, $rec_customer_id); 
							?></p>
							<br/>
							<?php 
							$db = new Db();
							$sql = "SELECT * FROM receipt_amount WHERE ra_ref = '$rec_ref'";
							$select = $db->select($sql);
							echo '<table border="1" cellspacing="0" cellpadding="5px" style="width:100%;font-size:14px;" >';
							$tra_amount=0;
							foreach($select[0] as $row){
								extract($row);
								echo '<tr>';
								echo '<td>'.$ra_desc.'</td>';
								echo '<td style="text-align:right;">'.number_format($ra_amount).'</td>';
								echo '</tr>';
								$tra_amount += $ra_amount;
							}
							echo '<tr><td><b>Total</b></td><td style="text-align:right;"><b>'.number_format($tra_amount).'</b></td></tr>';
							echo '</table>';
							$rec_amount = $tra_amount;
							?>
							<br/>
							<b>The sum of: </b><span style="color:blue; padding:0 10px; border-bottom:2px dotted black;<?php if($rec_cancelled) echo 'color:red !important;'; ?>"><?php echo FeedBack::number_to_words($rec_amount); 
							if($rec_curr=="USD") echo ' Dollars'; 
							elseif($rec_curr=="EUR") echo ' Euros'; 
							else echo ' Uganda Shillings'; 
							?></span>
							<br/><br/>
							<b>Being payment of:</b>
							<p style="color:blue; border-bottom:2px dotted black;<?php if($rec_cancelled) echo 'color:red !important;'; ?>"> <?php echo nl2br($rec_description); ?></p>
							<p style="font-family: arial; font-size:12px;">Payment Type: <b><?php echo $rec_payment_type; ?></b> &nbsp;&nbsp;&nbsp;&nbsp;&nbsp;
							<?php 
							if($rec_exchange_rate){
								echo 'Exchange Rate: <b>'.number_format($rec_exchange_rate,2).'</b>';
							}
							?>
							</p>
							
							<br/><br/>
							<div style="right:20px; bottom:10px; position:absolute;">
							<div style="float:left; width:40px;"> <?php echo $this->curCode($rec_curr); ?> </div>
							<div style="float:left;border:2px solid black; width:150px;color:blue;text-align:center;<?php if($rec_cancelled) echo 'color:red !important;'; ?>">
							<?php echo number_format($rec_amount); ?>
							</div>
							</div>
							<!-- <br/><br/><br/>-->
							
							
							_______________________________<br/>Signature
							</div>
						</div>
					</div>
				</div>
			</div>
			</div>
		</div>
	<?php
	}
	
	public function viewReceiptAction(){
		// echo '<a href="'.return_url().'receipts/view-receipts"><i class="fa fa-fw fa-arrow-left"></i>View Receipts</a>';
		// echo '&nbsp; &nbsp; ';
		// echo '<a href="'.return_url().'receipts/add-receipt"><i class="fa fa-fw fa-plus"></i>Add Receipt</a>';
		// echo '&nbsp; &nbsp; ';
		// echo '<a href="'.return_url().'receipts/view-receipt-categories"><i class="fa fa-fw fa-eye"></i>View Receipt Categories</a>';
		// echo '&nbsp; &nbsp; ';
		$id = portion(3);

		$db = new Db();
		$select = $db->select("SELECT * FROM receipts WHERE rec_id = '$id' ");
		extract($select[0][0]);


		if(!$rec_cancelled){
			echo '<a href="'.return_url().'receipts/cancel-receipt/'.$id.'" class="btn btn-xs btn-danger"><i class="fa fa-fw fa-times"></i>Cancel Receipt</a>';

			echo '&nbsp;&nbsp;&nbsp;<button class="btn btn-default btn-xs" id="sendSMS" data-sms="'.$this->receiptCatTelephone($rec_category, $rec_customer_id).'"><i class="fa fa-fw fa-comments"></i> Send SMS ('.$this->receiptCatTelephone($rec_category, $rec_customer_id).')</button>';

			echo '&nbsp;&nbsp;&nbsp;<button class="btn btn-default btn-xs" id="sendEmail" data-sms="'.$this->receiptCatEmail($rec_category, $rec_customer_id).'"><i class="fa fa-fw fa-envelope"></i> Send Email ('.$this->receiptCatEmail($rec_category, $rec_customer_id).')</button>';
			echo '<div id="sendSMSstatus"></div>';

		}

		//Feedback::sms("Your Loan payment of: ".($amount2)." has been received. Thnx", '256788229210');


		$date = date('Y-m-d', $rec_date_added);
		$receipt_no = $rec_receipt_no; //$_POST['receipt_no'];
		$category = $rec_category;
		$customer_name = $rec_customer;
		$amount = $rec_amount;
		$description = $rec_description;

		if(isset($_POST['submit'])){
			
			$date = $_POST['date'];
			$receipt_no = $this->receiptNo(); //$_POST['receipt_no'];
			$category = $_POST['category'];
			$customer_name = $_POST['customer_name'];
			$amount = $_POST['amount'];
			$description = $_POST['description'];
			
			$time = time();
			$user = user_id();


			$db = new Db();
			
			$errors = array();

			if(empty($date)){
				$errors[]="Enter Date";
			}
			if(empty($receipt_no)){
				$errors[]="Enter Receipt No.";
			}
			if(empty($category)){
				$errors[]="Select Category";
			}
			if(empty($customer_name)){
				$errors[]="Select Customer name";
			}
			if(($amount=="")){
				$errors[]="Enter Amount";
			}

			if($this->isThereEdit("receipts", ["rec_receipt_no"=>$receipt_no, "rec_id"=>$id])){
				$errors[]="Receipt No.($receipt_no) already exists";
			}
			
			if(empty($errors)){
				$db = new Db();
				$am = str_replace(',', '', $amount);
				$x = $db->update("receipts",["rec_date_added"=>$time, "rec_added_by"=>user_id(),"rec_category"=>$category, "rec_receipt_no"=>$receipt_no, "rec_amount"=>$am, "rec_customer_id"=>$customer_id, "rec_description"=>$description], ["rec_id"=>$id]);
								
				if(!$db->error()){
					FeedBack::success();
					FeedBack::refresh(3);
					//FeedBack::refresh(3, return_url().'receipts/all-customers');
				}else{
					FeedBack::error($db->error());
				}
			}else{
				FeedBack::errors($errors);
			}
		}
		?>
		<div class="col-md-12">
			<?php 
			
			echo '<a class="pull-right float-right" href="" target="" onclick="return print(\'printMe\');"><i class="fa fa-print fa-fw"></i> Print</a>'; 
			
		
			?>
			<br/>
			<div class="col-lg-12">
			<div id="printMe" style="width:700px;margin:auto;">
			<style>@media print {size: A5; font-family:monospace;  font-size:16px; @top-left {content: "Receipt";}} </style>
			<div class="panel panel-default">
				
				<div class="panel-body">

					<div class="row">
						<div class="col-lg-12" style="position:relative">
							<h2 style="text-align:center;font-size:25px;font-family:monospace;">UGANDA ELECTRICITY TRANSMISSION COMPANY LTD</h2>
							<table style="width:100%;">
								<tr>
									<td style="width:40%; font-weight: bold;">P.O.Box 7625<br/>Kampala, Uganda<br/>Plot 10, Hannington Road<br/><br/></td>
									<td><img src="<?php echo return_url(); ?>images/uedcl.PNG" style="width:100px"/></td>
									<td style="width:40%; font-weight:bold; text-align:right;">Tel: +256 41 233433/4<br/>Fax: +256 41 341789<br/>Telex: 61026<br/>E-mail: transco@uetcl.com</td>
								</tr>

							</table>
							<div style="width:100%; border-bottom:5px double #000;"></div>

							<img style="display:none;" src="<?php display_url();?>images/uetcl_header.png" width="100%">
							<div class="" style="font-family:monospace; font-size:18px; color:#000; width:250px; margin:5px auto; text-align:center; padding:2px 10px;font-weight:bold;<?php if($rec_cancelled) echo 'color:red !important;'; ?>"><?php if($rec_cancelled) echo 'CANCELLED '; ?> *** RECEIPT ***</div>
							<div style="font-family:monospace; float:left;padding-bottom:2px;margin:0;">
							<b style="font-family:monospace;font-size:16px; ">Date:</b> <span style="font-family:monospace;font-size:16px; color:#000;padding:0 10px;<?php if($rec_cancelled) echo 'color:red !important;'; ?>"><?php echo date('dS-F-Y', $rec_date); ?></span>
							</div>
							
							<div style="font-family:monospace;font-size:16px; float:right;padding-bottom:2px;margin:0;">
							<b>Receipt No.:</b> <span style="color:#000; padding:0 10px;<?php if($rec_cancelled) echo 'color:red !important;'; ?>"><?php echo $rec_receipt_no; ?></span>
							</div>
							<div style="clear: both;"></div>
							<div style="font-family:monospace; font-size:16px;">
							<b>Received with Thanks From: </b><span style="<?php if($rec_cancelled) echo 'color:red !important;'; ?>"> 
							<?php 
							//echo $this->rgf($rec_category, $rec_customer_id);
							echo $this->receiptCat($rec_category, $rec_customer_id); 
							?></span>
							<br/>
							<?php 
							$db = new Db();
							$sql = "SELECT * FROM receipt_amount WHERE ra_ref = '$rec_ref'";
							$select = $db->select($sql);
							//echo '<br/><table border="0" cellspacing="0" cellpadding="5px" style="font-family:monospace; width:100%;font-size:16px;" >';
							$tra_amount=0;
							$i=1;
							foreach($select[0] as $row){
								extract($row);
								//echo '<tr>';
								//echo '<td style="font-family:monospace; ">'.($i++).'.</td>';
								//echo '<td style="font-family:monospace; ">'.$ra_desc.'</td>';
								//echo '<td style="font-family:monospace; text-align:right;">'.number_format($ra_amount).'</td>';
								//echo '</tr>';
								$tra_amount += $ra_amount;
							}
							//echo '<tr><td></td><td style="font-family:monospace; text-align:right;"><b>Total: </b></td><td style="text-align:right;"><b>'.number_format($tra_amount).'</b></td></tr>';
							//echo '</table>';
							$rec_amount = $tra_amount;

							//==============================

							echo '<input type="hidden" value="'.number_format($rec_amount,2).' '.$this->curCode($rec_curr).'" id="amount"/>';
							echo '<input type="hidden" value="'.$this->receiptCatTelephone($rec_category, $rec_customer_id).'" id="telephone"/>';
							echo '<input type="hidden" value="'.$rec_description.'" id="description"/>';
							echo '<input type="hidden" value="'.$this->receiptCat($rec_category, $rec_customer_id).'" id="name"/>';
							echo '<input type="hidden" value="'.$rec_receipt_no.'" id="receiptNo"/>';

							//=================================
							?>
							<br/><b>The sum of: </b><span style="font-family:monospace; color:#000; padding:0 10px;<?php if($rec_cancelled) echo 'color:red !important;'; ?>"><?php echo FeedBack::number_to_words($rec_amount); 
							if($rec_curr=="USD") echo ' Dollars'; 
							elseif($rec_curr=="EUR") echo ' Euros'; 
							else echo ' Uganda Shillings'; 
							?></span>
							<br/><br/>
							<b>Being payment of:</b>
							<p style="font-family:monospace; color:#000; <?php if($rec_cancelled) echo 'color:red !important;'; ?>"> <?php echo nl2br($rec_description); ?></p>
							<p style="font-family:monospace;  font-size:16px;"><b>Payment Type: </b><?php echo $rec_payment_type; ?> &nbsp;&nbsp;&nbsp;&nbsp;&nbsp;
							<?php 
							if((int)$rec_exchange_rate){
								echo 'Exchange Rate: <b>'.number_format($rec_exchange_rate,2).'</b>';
							}
							?>
							</p>
							
							<div style="font-family:monospace; right:20px; bottom:10px; position:absolute;">
							<div style="font-family:monospace; float:left; width:40px;">  </div>
							<div style="font-size:18px; float:left;font-weight:bold; min-width:150px;color:black;text-align:right;<?php if($rec_cancelled) echo 'color:red !important;'; ?>">
								<?php echo $this->curCode($rec_curr).':'; ?>
							<?php echo number_format($rec_amount); ?>
							</div>
							</div>
							<!-- <br/><br/><br/>-->
							
							
							_______________________________<br/><span style="font-family:monospace; ">Signature</span>
							</div>
						</div>
					</div>
				</div>
			</div>
			</div>
		</div>
	<?php
	}
	
	public function curCode($id){
		if($id==1){
			return 'UGX';
		}elseif($id == 2){
			return 'USD';
		}else{
			return $id;
		}
		
	}
	
	public function exporter(){
	
		//header info for browser
		header("Content-Type: application/vnd.ms-excel");
		header("Content-Disposition: attachment; filename=".$filename.".xls");
		header("Pragma: no-cache"); 
		header("Expires: 0");
		$file_ending = "xls";
		
		$headers = array();
		$headers[] = 'LI';
		$headers[] = 'Account';
		$headers[] = 'Receipt Narration';
		$headers[] = 'Receipt No.';	
		$headers[] = 'Receipt Date';
		$headers[] = 'Period';
		$headers[] = 'Base Amount';
		$headers[] = 'Forex Amount';
		$headers[] = 'Debit/Credit';
		$headers[] = 'Currency';
		$headers[] = 'Section';
		$headers[] = 'Staff';
		$headers[] = 'Project';
	}

	public function viewReceiptsAction(){
		
		echo '<a href="'.return_url().'receipts/add-receipt"><i class="fa fa-fw fa-plus"></i>Add Receipt</a>';
		
		echo '&nbsp; &nbsp; ';
		echo '<a href="'.return_url().'receipts/view-receipt-categories"><i class="fa fa-fw fa-eye"></i>View Receipt Categories</a>';
		
		echo '<div style="margin-bottom:20px;"></div>';
		
		$customers = array();
        $db = new Db();
        $select = $db->select("SELECT distinct rec_customer_id,rec_category FROM receipts");
        foreach($select[0] as $row){
            extract($row);
            $customers[$rec_customer_id] = $this->receiptCat($rec_category, $rec_customer_id);
        }
		
		////////////////////////////////////////////////////////////////////////
		echo '<div class="row">';
    	echo '<div class="col-md-12">';
    	echo '<form action="" method="post" class="search">';

    	if(isset($_POST['search'])){
    		$factory = $_POST['factory'];
    		$month = $_POST['month'];
    		$year = $_POST['year'];
    		$receipt_category = $_POST['receipt_category'];

    		if(empty($year)||empty($month)){
    			FeedBack::error("Please select Year and Month");
    		}else{

    			if($year == "all"){
    				$start_date = strtotime('2020-10-20 12:00:00 am');
    			}elseif($year !="all" && $month=="all"){    				
    				$start_date = strtotime("$year-01-01 12:00:00 am");
    			}else{    				
    				$start_date = strtotime("$year-$month-01 12:00:00 am");
    			}

    			if($month == "all" && $year == "all"){
    				$end_date = time();
    			}elseif($month=="all"){
    				$end_date = strtotime("$year-12-01 12:00:00 am");
    				$end_date = strtotime(date('Y-m-t', $end_date).' 11:59:59 pm');
    			}else{
    				$end_date = strtotime("$year-$month-01 12:00:00 am");
    				$end_date = strtotime(date('Y-m-t', $end_date).' 11:59:59 pm');
    			}
    		}

    		// echo 'Start:'.FeedBack::date_fm($start_date);
    		// echo ' <br/>';
    		// echo 'End :'.FeedBack::date_fm($end_date);

    	}

    	if(isset($_POST['searchPeroid'])){
    		$factory = $_POST['factory'];
    		$from = $_POST['from'];
    		$to = $_POST['to'];
    		$receipt_category = $_POST['receipt_category'];

    		if(empty($from) || empty($to) ){
    			FeedBack::error("Please Enter Both Dates (From and To)");
    		}elseif($from && $to){
    			$start_date = strtotime($from.' 12:00:00 am');
    			$end_date = strtotime($to.' 11:59:59 pm');
    		}

    	}

    	if(empty($start_date) && empty($end_date)){
    		$start_date = strtotime(date('Y-m-d').' 12:00:00 am');
    		$end_date = time();
    	}

    	$from = date('Y-m-d', $start_date);
    	$to = date('Y-m-d', $end_date);

    	echo '<div style="float:left; margin-right:20px;">';
    	echo '<label>Receipt Category&nbsp;&nbsp;</label>';
    	$db = new Db();
		$sql = "SELECT * FROM receipt_category ORDER BY rc_name ASC";
		$select = $db->select($sql);

    	echo '<select name="receipt_category">';
    	echo '<option value="">All</option>';
		foreach($select[0] as $row){
			extract($row);
			if($rc_id == $receipt_category)
				echo '<option selected value="'.$rc_id.'">'.$rc_name.'</option>';
			else
				echo '<option value="'.$rc_id.'">'.$rc_name.'</option>';
		}
		
    	echo '</select>';
    	echo '</div>';

    	echo '<div style="float:left; margin-right:20px;">';
    	echo '<label>Customer&nbsp;&nbsp;</label>';
    	echo '<select name="factory">';
    	echo '<option value="">All</option>';
    	$db = new Db();
		
		foreach($customers as $id=>$name){
			extract($row);
			if($id == $factory)
				echo '<option selected value="'.$id.'">'.$name.'</option>';
			else
				echo '<option value="'.$id.'">'.$name.'</option>';
		}
		
    	echo '</select>';
    	echo '</div>';

    	echo '<div style="float:right; margin-right:20px;">';
    	echo '<input type="text" style="display:none; width:500px;background-color:#fff3f3;border:1px solid #ccc;" value="" placeholder="Search Invoice"/>';
    	echo '</div>';

    	echo '<div class="clearfix" style="margin-bottom:10px;"></div>';

    	echo '<div style="float:left; margin-right:20px;">';
    	echo '<label>Year&nbsp;&nbsp;</label>';
    	echo '<select name="year">';
    	echo '<option value="">Select</option>';
    	// if($year == "all")
    	// 	echo '<option selected value="all">All</option>';
    	// else
    	// 	echo '<option value="all">All</option>';
    	for($i=2020; $i<=date('Y'); $i++){
    		if($year == $i)
    			echo '<option selected value="'.$i.'">'.$i.'</option>';
    		else
    			echo '<option value="'.$i.'">'.$i.'</option>';
    	}
    	echo '</select>';
    	echo '</div>';
    	
    	echo '<div style="float:left; margin-right:20px;">';
    	echo '<label>Month&nbsp;&nbsp;</label>';
    	echo '<select name="month">';
    	echo '<option value="">Select</option>';
    	if($month == "all")
    		echo '<option selected value="all">All</option>';
    	else
    		echo '<option value="all">All</option>';
    	for($i=1; $i<=12; $i++){
    		if($month == $i)
    			echo '<option selected value="'.$i.'">'.month_name($i).'</option>';
    		else
    			echo '<option value="'.$i.'">'.month_name($i).'</option>';
    	}
    	echo '</select>';
    	echo '</div>';
    	
    	echo '<div style="float:left; margin-right:20px;">';
    	echo '<button type="submit" name="search" class="btn btn-primary btn-xs pt-0 pb-0"><i class="fa fa-fw fa-search"></i> Show</button>';
    	echo '</div>';

    	echo '<div style="float:left; margin-right:20px; width:50px;">&nbsp;';
    	echo '</div>';

    	echo '<div style="float:left; margin-right:20px;">';
    	echo '<label>From&nbsp;&nbsp;</label>';
    	echo '<input type="date" name="from" value="'.$from.'"><br/>';
    	echo '</div>';
    	echo '<div style="float:left; margin-right:20px;">';
    	echo '<label>To&nbsp;&nbsp;</label>';
    	echo '<input type="date" name="to" value="'.$to.'">';
    	echo '</div>';
    	
    	echo '<div style="float:left; margin-right:20px;">';
    	echo '<button type="submit" name="searchPeroid" class="btn btn-primary btn-xs pt-0 pb-0"><i class="fa fa-fw fa-search"></i> Show</button>';
    	echo '</div>';

    	echo '<div class="clearfix"></div>';
    	echo '</form>';
    	echo '</div>';
    	echo '</div>';

		////////////////////////////////////////////////////////////////////////////////
		
    	if($start_date && $end_date){
    		$sd = FeedBack::date_fm($start_date);
    		$ed = FeedBack::date_fm($end_date);

    		if(empty($factory))
    			$ti = $this->rgf("receipt_category", $receipt_category, "rc_id", "rc_name")." Receipts from $sd to $ed";
    		else
    			$ti = $this->rgf("receipt_category", $receipt_category, "rc_id", "rc_name")." Receipts from $sd to $ed";

    		echo "<h5>$ti</h5>";
    	}

    	$db = new Db();

    	if($receipt_category){
    		$xx = "rec_category = '$receipt_category' AND ";
    	}else{
    		$xx = "";
    	}
    	if(empty($factory))
			$sql = "SELECT * FROM receipts, receipt_amount WHERE $xx rec_ref = ra_ref AND rec_date >= '$start_date' AND rec_date <= '$end_date' AND (rec_cancelled IS NULL or rec_cancelled = 0) ORDER BY rec_receipt_no ASC";
    	else
    		$sql = "SELECT * FROM receipts, receipt_amount WHERE $xx rec_ref = ra_ref AND rec_date >= '$start_date' AND rec_date <= '$end_date' AND rec_customer_id = '$factory' AND (rec_cancelled IS NULL or rec_cancelled = 0) ORDER BY rec_receipt_no ASC";

    	$select = $db->select($sql);


		echo '<table border="1" cellspacing="0" cellpadding="2" id="table">';

		echo '<tr>';
		echo '<th>No.</th>';
		echo '<th>Receipt Date</th>';
		echo '<th>Receipt No.</th>';
		echo '<th>Customer</th>';
		echo '<th>Category</th>';
		echo '<th>Amount</th>';
		echo '<th>CUR</th>';
		echo '<th>Section</th>';
		echo '<th>Staff</th>';
		echo '<th>Project</th>';
		echo '<th>Account</th>';
		echo '<th>Vat</th>';
		echo '<th>Action</th>';

		$db = new Db();
		//$sql = "SELECT * FROM receipts, receipt_amount WHERE rec_ref = ra_ref AND rec_date >= '$from' AND rec_date <= '$to' ORDER BY rec_receipt_no ASC";
		//$select = $db->select($sql);
		$no = 1;
		$total_amount = 0;
		foreach($select[0] as $row){
			extract($row);
			echo '<tr>';
			echo '<td width="30px">'.($no++).'.</td>';
			echo '<td>'.FeedBack::date_s($rec_date).'</td>';
			echo '<td>'.($rec_receipt_no).'</td>';
			echo '<td>'.$this->receiptCat($rec_category, $rec_customer_id).'</td>';
			echo '<td>'.$this->rgf("receipt_category", $rec_category, "rc_id", "rc_name").'</td>';
			echo '<td class="text-right">'.number_format($ra_amount, 1).'</td>';
			echo '<td class="text-left">'.($rec_curr).'</td>';
			echo '<td class="text-left" title="'.$this->sunName('13', $rec_section).'">'.($rec_section).'</td>';
			echo '<td class="text-left" title="'.$this->sunName('09', $rec_staff).'">'.($rec_staff).'</td>';
			echo '<td class="text-left" title="'.$this->sunName('14', $rec_project).'">'.($rec_project).'</td>';
			echo '<td class="" title="'.$this->bank[$rec_bank].'">'.($rec_bank).'</td>';
			echo '<td class="" title="'.$this->vat_list[$rec_vat_status].'">'.($rec_vat_status).'</td>';
			
			echo '<td class="text-right">';

			//echo '<a href="'.return_url().'receipts/delete-receipt/'.$rec_id.'" class="btn btn-danger btn-xs">Delete</a>';
			//echo '<a href="'.return_url().'receipts/edit-receipt/'.$rec_id.'" class="btn btn-success btn-xs">Edit</a>';
			
			echo '<a href="'.return_url().'receipts/view-receipt/'.$rec_id.'" class="btn btn-primary btn-xs">View</a>';

			echo '</td>';
			echo '</tr>';
			$total_amount += $rec_amount;
		}

		echo '<tr>';
		echo '<th colspan="5">Total</th>';
		echo '<th class="text-right" style="text-align:right">'.number_format($total_amount).'</th>';		
		echo '<th colspan="7">&nbsp;</th>';
		echo '</tr>';

		echo '</table>';
		
		echo '<br/>';
		
		
		echo '<a href="'.return_url().'classes/exporter2.php?p1='.$start_date.'&p2='.$end_date.'"><i class="fa fa-fw fa-table"></i> Export JV</a>';
		
	}
	
	
	public function viewCancelledReceiptsAction(){
		
		echo '<a href="'.return_url().'receipts/add-receipt"><i class="fa fa-fw fa-plus"></i>Add Receipt</a>';
		
		echo '&nbsp; &nbsp; ';
		echo '<a href="'.return_url().'receipts/view-receipt-categories"><i class="fa fa-fw fa-eye"></i>View Receipt Categories</a>';
		
		echo '<div style="margin-bottom:20px;"></div>';
		
		$customers = array();
        $db = new Db();
        $select = $db->select("SELECT distinct rec_customer_id,rec_category FROM receipts");
        foreach($select[0] as $row){
            extract($row);
            $customers[$rec_customer_id] = $this->receiptCat($rec_category, $rec_customer_id);
        }
		
		////////////////////////////////////////////////////////////////////////
		echo '<div class="row">';
    	echo '<div class="col-md-12">';
    	echo '<form action="" method="post" class="search">';

    	if(isset($_POST['search'])){
    		$factory = $_POST['factory'];
    		$month = $_POST['month'];
    		$year = $_POST['year'];
    		$receipt_category = $_POST['receipt_category'];

    		if(empty($year)||empty($month)){
    			FeedBack::error("Please select Year and Month");
    		}else{

    			if($year == "all"){
    				$start_date = strtotime('2020-10-20 12:00:00 am');
    			}elseif($year !="all" && $month=="all"){    				
    				$start_date = strtotime("$year-01-01 12:00:00 am");
    			}else{    				
    				$start_date = strtotime("$year-$month-01 12:00:00 am");
    			}

    			if($month == "all" && $year == "all"){
    				$end_date = time();
    			}elseif($month=="all"){
    				$end_date = strtotime("$year-12-01 12:00:00 am");
    				$end_date = strtotime(date('Y-m-t', $end_date).' 11:59:59 pm');
    			}else{
    				$end_date = strtotime("$year-$month-01 12:00:00 am");
    				$end_date = strtotime(date('Y-m-t', $end_date).' 11:59:59 pm');
    			}
    		}

    		// echo 'Start:'.FeedBack::date_fm($start_date);
    		// echo ' <br/>';
    		// echo 'End :'.FeedBack::date_fm($end_date);

    	}

    	if(isset($_POST['searchPeroid'])){
    		$factory = $_POST['factory'];
    		$from = $_POST['from'];
    		$to = $_POST['to'];
    		$receipt_category = $_POST['receipt_category'];

    		if(empty($from) || empty($to) ){
    			FeedBack::error("Please Enter Both Dates (From and To)");
    		}elseif($from && $to){
    			$start_date = strtotime($from.' 12:00:00 am');
    			$end_date = strtotime($to.' 11:59:59 pm');
    		}

    	}

    	if(empty($start_date) && empty($end_date)){
    		$start_date = strtotime(date('Y-m-d').' 12:00:00 am');
    		$end_date = time();
    	}

    	$from = date('Y-m-d', $start_date);
    	$to = date('Y-m-d', $end_date);

    	echo '<div style="float:left; margin-right:20px;">';
    	echo '<label>Receipt Category&nbsp;&nbsp;</label>';
    	$db = new Db();
		$sql = "SELECT * FROM receipt_category ORDER BY rc_name ASC";
		$select = $db->select($sql);

    	echo '<select name="receipt_category">';
    	echo '<option value="">All</option>';
		foreach($select[0] as $row){
			extract($row);
			if($rc_id == $receipt_category)
				echo '<option selected value="'.$rc_id.'">'.$rc_name.'</option>';
			else
				echo '<option value="'.$rc_id.'">'.$rc_name.'</option>';
		}
		
    	echo '</select>';
    	echo '</div>';

    	echo '<div style="float:left; margin-right:20px;">';
    	echo '<label>Customer&nbsp;&nbsp;</label>';
    	echo '<select name="factory">';
    	echo '<option value="">All</option>';
    	$db = new Db();
		
		foreach($customers as $id=>$name){
			extract($row);
			if($id == $factory)
				echo '<option selected value="'.$id.'">'.$name.'</option>';
			else
				echo '<option value="'.$id.'">'.$name.'</option>';
		}
		
    	echo '</select>';
    	echo '</div>';

    	echo '<div style="float:right; margin-right:20px;">';
    	echo '<input type="text" style="display:none; width:500px;background-color:#fff3f3;border:1px solid #ccc;" value="" placeholder="Search Invoice"/>';
    	echo '</div>';

    	echo '<div class="clearfix" style="margin-bottom:10px;"></div>';

    	echo '<div style="float:left; margin-right:20px;">';
    	echo '<label>Year&nbsp;&nbsp;</label>';
    	echo '<select name="year">';
    	echo '<option value="">Select</option>';
    	// if($year == "all")
    	// 	echo '<option selected value="all">All</option>';
    	// else
    	// 	echo '<option value="all">All</option>';
    	for($i=2020; $i<=date('Y'); $i++){
    		if($year == $i)
    			echo '<option selected value="'.$i.'">'.$i.'</option>';
    		else
    			echo '<option value="'.$i.'">'.$i.'</option>';
    	}
    	echo '</select>';
    	echo '</div>';
    	
    	echo '<div style="float:left; margin-right:20px;">';
    	echo '<label>Month&nbsp;&nbsp;</label>';
    	echo '<select name="month">';
    	echo '<option value="">Select</option>';
    	if($month == "all")
    		echo '<option selected value="all">All</option>';
    	else
    		echo '<option value="all">All</option>';
    	for($i=1; $i<=12; $i++){
    		if($month == $i)
    			echo '<option selected value="'.$i.'">'.month_name($i).'</option>';
    		else
    			echo '<option value="'.$i.'">'.month_name($i).'</option>';
    	}
    	echo '</select>';
    	echo '</div>';
    	
    	echo '<div style="float:left; margin-right:20px;">';
    	echo '<button type="submit" name="search" class="btn btn-primary btn-xs pt-0 pb-0"><i class="fa fa-fw fa-search"></i> Show</button>';
    	echo '</div>';

    	echo '<div style="float:left; margin-right:20px; width:50px;">&nbsp;';
    	echo '</div>';

    	echo '<div style="float:left; margin-right:20px;">';
    	echo '<label>From&nbsp;&nbsp;</label>';
    	echo '<input type="date" name="from" value="'.$from.'"><br/>';
    	echo '</div>';
    	echo '<div style="float:left; margin-right:20px;">';
    	echo '<label>To&nbsp;&nbsp;</label>';
    	echo '<input type="date" name="to" value="'.$to.'">';
    	echo '</div>';
    	
    	echo '<div style="float:left; margin-right:20px;">';
    	echo '<button type="submit" name="searchPeroid" class="btn btn-primary btn-xs pt-0 pb-0"><i class="fa fa-fw fa-search"></i> Show</button>';
    	echo '</div>';

    	echo '<div class="clearfix"></div>';
    	echo '</form>';
    	echo '</div>';
    	echo '</div>';

		////////////////////////////////////////////////////////////////////////////////
		
    	if($start_date && $end_date){
    		$sd = FeedBack::date_fm($start_date);
    		$ed = FeedBack::date_fm($end_date);

    		if(empty($factory))
    			$ti = "Cancelled ".$this->rgf("receipt_category", $receipt_category, "rc_id", "rc_name")." Receipts from $sd to $ed";
    		else
    			$ti = "Cancelled ".$this->rgf("receipt_category", $receipt_category, "rc_id", "rc_name")." Receipts from $sd to $ed";

    		echo "<h5>$ti</h5>";
    	}

    	$db = new Db();

    	if($receipt_category){
    		$xx = "rec_category = '$receipt_category' AND ";
    	}else{
    		$xx = "";
    	}
    	if(empty($factory))
			$sql = "SELECT * FROM receipts, receipt_amount WHERE $xx rec_ref = ra_ref AND rec_date >= '$start_date' AND rec_date <= '$end_date' AND (rec_cancelled IS NOT NULL) ORDER BY rec_receipt_no ASC";
    	else
    		$sql = "SELECT * FROM receipts, receipt_amount WHERE $xx rec_ref = ra_ref AND rec_date >= '$start_date' AND rec_date <= '$end_date' AND rec_customer_id = '$factory' AND (rec_cancelled IS NOT NULL) ORDER BY rec_receipt_no ASC";

    	$select = $db->select($sql);


		echo '<table border="1" cellspacing="0" cellpadding="2" id="table">';

		echo '<tr>';
		echo '<th>No.</th>';
		echo '<th>Receipt Date</th>';
		echo '<th>Receipt No.</th>';
		echo '<th>Customer</th>';
		echo '<th>Category</th>';
		echo '<th>Amount</th>';
		echo '<th>CUR</th>';
		echo '<th>Section</th>';
		echo '<th>Staff</th>';
		echo '<th>Project</th>';
		echo '<th>Account</th>';
		echo '<th>Vat</th>';
		echo '<th>Action</th>';

		$db = new Db();
		//$sql = "SELECT * FROM receipts, receipt_amount WHERE rec_ref = ra_ref AND rec_date >= '$from' AND rec_date <= '$to' ORDER BY rec_receipt_no ASC";
		//$select = $db->select($sql);
		$no = 1;
		$total_amount = 0;
		foreach($select[0] as $row){
			extract($row);
			echo '<tr>';
			echo '<td width="30px">'.($no++).'.</td>';
			echo '<td>'.FeedBack::date_s($rec_date).'</td>';
			echo '<td>'.($rec_receipt_no).'</td>';
			echo '<td>'.$this->receiptCat($rec_category, $rec_customer_id).'</td>';
			echo '<td>'.$this->rgf("receipt_category", $rec_category, "rc_id", "rc_name").'</td>';
			echo '<td class="text-right">'.number_format($ra_amount, 1).'</td>';
			echo '<td class="text-left">'.($rec_curr).'</td>';
			echo '<td class="text-left" title="'.$this->sunName('13', $rec_section).'">'.($rec_section).'</td>';
			echo '<td class="text-left" title="'.$this->sunName('09', $rec_staff).'">'.($rec_staff).'</td>';
			echo '<td class="text-left" title="'.$this->sunName('14', $rec_project).'">'.($rec_project).'</td>';
			echo '<td class="" title="'.$this->bank[$rec_bank].'">'.($rec_bank).'</td>';
			echo '<td class="" title="'.$this->vat_list[$rec_vat_status].'">'.($rec_vat_status).'</td>';
			
			echo '<td class="text-right">';

			//echo '<a href="'.return_url().'receipts/delete-receipt/'.$rec_id.'" class="btn btn-danger btn-xs">Delete</a>';
			//echo '<a href="'.return_url().'receipts/edit-receipt/'.$rec_id.'" class="btn btn-success btn-xs">Edit</a>';
			
			echo '<a href="'.return_url().'receipts/view-receipt/'.$rec_id.'" class="btn btn-primary btn-xs">View</a>';

			echo '</td>';
			echo '</tr>';
			$total_amount += $rec_amount;
		}

		echo '<tr>';
		echo '<th colspan="5">Total</th>';
		echo '<th class="text-right" style="text-align:right">'.number_format($total_amount).'</th>';		
		echo '<th colspan="7">&nbsp;</th>';
		echo '</tr>';

		echo '</table>';
		
		echo '<br/>';
		
		
		// echo '<a href="'.return_url().'classes/exporter2.php?p1='.$start_date.'&p2='.$end_date.'"><i class="fa fa-fw fa-table"></i> Export JV</a>';
		
	}
	
	private function sunName($category, $anl_code){

		$dbss = new DbSunServer();
		$sections = $dbss->select("SELECT NAME FROM IDG_ANL_CODE WHERE ANL_CAT_ID='$category' AND ANL_CODE = '$anl_code'"); 
		extract($sections[0][0]);
		
		return $NAME;
	
	}
	
	private function receiptCat($category, $customer_id){
		if($category == 4){//fibre optics
			$item = $this->rgf("of_customer", $customer_id, "ofc_id", "ofc_customer_name");
			$cat = $this->rgf("receipt_category", $category, 'rc_id', 'rc_name');
		}elseif($category == 3){			
			$item = $this->rgf("customer", $customer_id, "customer_id", "customer_short_name");
			$cat = $this->rgf("receipt_category", $category, 'rc_id', 'rc_name');
		}else{		
			//return $category.' '.$customer_id;
			$item = $this->rgf("receipt_category_item", $customer_id, "rci_id", "rci_name");
			$cat = $this->rgf("receipt_category", $category, 'rc_id', 'rc_name');
		}
		
		if(strtolower($item) == "any body" || strtolower($item) == "" || strtolower($item) == "not applicable"){
			return $cat;
		}else{
			return $item;  
		}
	}
	
	private function receiptCatTelephone($category, $customer_id){
		if($category == 4){//fibre optics
			$item = $this->rgf("of_customer", $customer_id, "ofc_id", "ofc_tel");
		}elseif($category == 3){			
			$item = $this->rgf("customer", $customer_id, "customer_id", "customer_tel");
		}else{		
			$item = $this->rgf("receipt_category_item", $customer_id, "rci_id", "rci_telephone");
		}

		if(empty($item)) $item = "+256788229210";
		
		return $item;
	}

	private function receiptCatEmail($category, $customer_id){
		if($category == 4){//fibre optics
			$item = $this->rgf("of_customer", $customer_id, "ofc_id", "ofc_email");
		}elseif($category == 3){			
			$item = $this->rgf("customer", $customer_id, "customer_id", "customer_email");
		}else{		
			$item = $this->rgf("receipt_category_item", $customer_id, "rci_id", "rci_email");
		}

		if(empty($item)) $item = "josemusiitwa@gmail.com";
		
		return $item;
	}
	
	public function deleteReceiptCategoryAction(){		
		$id = portion(3);
		$this->deletor("receipt_category", "rc_id",  $id, 'receipts/view-receipt-categories');
	}
	
	public function deleteAccount2(){		
		$id = portion(3);
		$this->deletor("account2", "account_id",  $id, 'receipts/view-account2-list');
	}
	
	
	public function removeAmount(){		
		$id = portion(3);
		$this->deletor("receipt_amount", "ra_id",  $id, 'receipts/add-receipt');
	}
	
	public function removeAmountEdit(){		
		$id = portion(3);	
		$id2 = portion(4);
		$this->deletor("receipt_amount", "ra_id",  $id, 'receipts/edit-receipt/'.$id2);
	}
	

	public function deleteReceiptAction(){
		$id = portion(3);
		$this->deletor("receipts", "rec_id",  $id, 'receipts/view-receipts');
	}

	public function deleteCategoryItemAction(){
		$id = portion(3);
		$id2 = portion(4);
		$this->deletor("receipt_category_item", "rci_id",  $id, 'receipts/view-category-items/'.$id2);
	}
	
	public function viewReceiptCategories(){
		?>
		<div class="">
			
		<?php 
		echo '<a href="'.return_url().'receipts/view-receipts"><i class="fa fa-fw fa-arrow-left"></i>View Receipts</a>';
		echo '&nbsp;';
		echo '&nbsp;';
		echo '&nbsp;';
		echo '<a href="'.return_url().'receipts/add-receipt"><i class="fa fa-fw fa-plus"></i>Add Receipt</a>';
		?><br/>
			<br/>
			<div class="">	

			<div class="row">
				<div class="col-lg-12">
					<form role="form" action="" method="post">
						<?php
						if(isset($_POST['submit'])){
							$name = $_POST['name'];
							
							$db = new Db();
							$insert = $db->insert("receipt_category", ["rc_name"=>$name, "rc_date_added"=>time(), "rc_added_by"=>user_id()]);
							
							if(empty($db->error())){
								Feedback::success();
								Feedback::refresh();
							}else{
								Feedback::error($db->error());
							}
						}
						?>
						<div class="row">
							<div class="col-md-6">
								<div class="form-group">
									<label>Add New Receipt Category<span class="must"></span></label>
									<div class="form-line">
										<input autocomplete="off" class="form-control" type="text" name="name" placeholder="Enter Category name" value="<?php echo @$name; ?>">
									</div>
								</div>
							</div>
							<div class="col-md-1">
								<div class="form-group">
									<label>&nbsp;<span class="must">&nbsp;</span></label>
									<button onclick = "" type="submit" name="submit" class="btn btn-primary"><i class="fa fa-fw fa-save"></i> Save</button>
								</div>
							</div>
						</div>
					</form>
				</div>
			</div>
				
		</div>
		
		<table border="1" cellspacing="0" cellpadding="2" id="table">
				<tr>
					<th>No</th>
					<th>Receipt Category</th>
					<th>Items</th>
					<th>Action</th>
				</tr>
				<?php 
				$i=1;
				$db = new Db();
				$select = $db->select("SELECT * FROM receipt_category");
				//echo $db->num_rows();
				foreach($select[0] as $row){
					extract($row);
					echo '<tr>';
					echo '<td width="1px">'.($i++).'.</td>';
					echo '<td>'.($rc_name).'</td>';
					echo '<td>'; 
					if($rc_id == 4){//fibre optics
						$db->select("SELECT ofc_id FROM of_customer");
						echo number_format($db->num_rows());
					}elseif($rc_id == 3){//energy sales
						$db->select("SELECT customer_id FROM customer");
						echo number_format($db->num_rows());					
					}else{
						$db->select("SELECT rci_id FROM receipt_category_item WHERE rci_category = '$rc_id'");
						echo number_format($db->num_rows());					
					}
					echo '</td>';
					echo '<td width="200px">';
					if($rc_id != 3 && $rc_id != 4 )
					echo '<a href="'.return_url().'receipts/delete-receipt-category/'.$rc_id.'" class="btn  btn-xs btn-danger">Delete</a>';
					echo '&nbsp;'; 
					echo '<a href="#" class="btn btn-success btn-xs " id="add-hint'.$i.'">Edit</a>';
					echo '&nbsp;'; 
					echo '<a href="'.return_url().'receipts/view-category-items/'.$rc_id.'" class="btn btn-primary btn-xs ">View Items</a></td>';
					echo '</tr>';

					$co = $i;
					?>

					<div id="myModal<?php echo $co; ?>" class="net-advance-form modal">
					 	<div class="modal-content">
					 		<form action="" method="post">
						 		<?php
						 		if(isset($_POST['saveChanges'.$co])){
						 			$cat_name = $_POST['cat_name'];

						 			$db->update("receipt_category", [
						 				"rc_name"=>$cat_name,
						 			], ["rc_id"=>$rc_id]);

						 			Feedback::redirect(return_url().'receipts/view-receipt-categories');
						 		}
						 		?>
						    	<span id="close<?php echo $co; ?>" class="close" style="color:red;">&times;</span>
						    	<h4>Edit Receipt Category</h4>
						    	<textarea required class="form-control" style="height:100px;" name="cat_name"><?php echo $rc_name; ?></textarea>
						    	<br/>
						    	<button name="saveChanges<?php echo $co; ?>" class="btn btn-success"> <i class="fa fa-fw fa-save"></i> Save Changes</button>
					    	</form>
						</div>

					</div>
					<script>
					document.getElementById("<?php echo 'add-hint'.$co; ?>").onclick = function() {
					document.getElementById("<?php echo 'myModal'.$co; ?>").style.display = "block";
					}
					document.getElementById("<?php echo 'close'.$co; ?>").onclick = function() {
					document.getElementById("<?php echo 'myModal'.$co; ?>").style.display = "none";
					}
					window.onclick = function(event) {
					if (event.target == document.getElementById("<?php echo 'myModal'.$co; ?>")) {
					document.getElementById("<?php echo 'myModal'.$co; ?>").style.display = "none";
					}
					}
					</script>

					<?php
				}
				?>
			</table>
		<?php
	}
	
	
	public function viewAccount2ListAction(){
		?>
		<div class="">
			
		<?php 
		echo '<a href="'.return_url().'receipts/view-receipts"><i class="fa fa-fw fa-arrow-left"></i>View Receipts</a>';
		echo '&nbsp;';
		echo '&nbsp;';
		echo '&nbsp;';
		echo '<a href="'.return_url().'receipts/add-receipt"><i class="fa fa-fw fa-plus"></i>Add Receipt</a>';
		?><br/>
			<br/>
			<div class="">	

			<div class="row">
				<div class="col-lg-12">
					<form role="form" action="" method="post">
						<?php
						if(isset($_POST['submit'])){
							$name = $_POST['name'];
							$number = $_POST['number'];
							
							$errors = array();
							if($name == "")
								$errors[] = "Enter Account name";
							if($number == "")
								$errors[] = "Enter Account number";
							
							if(empty($errors)){
								$db = new Db();
								$insert = $db->insert("account2", ["account_name"=>$name,"account_number"=>$number, "account_date_added"=>time(), "account_added_by"=>user_id()]);
								
								if(empty($db->error())){
									Feedback::success();
									Feedback::refresh();
								}else{
									Feedback::error($db->error());
								}
							}else{
								Feedback::errors($errors);
							}
						}
						?>
						<div class="row">
							<div class="col-md-3">
								<div class="form-group">
									<label>Add New Account name<span class="must"></span></label>
									<div class="form-line">
										<input autocomplete="off" class="form-control" type="text" name="name" placeholder="Enter Account name" value="<?php echo @$name; ?>">
									</div>
								</div>
							</div>
							<div class="col-md-3">
								<div class="form-group">
									<label>Account Number<span class="must"></span></label>
									<div class="form-line">
										<input autocomplete="off" class="form-control" type="text" name="number" placeholder="Enter Account number" value="<?php echo @$number; ?>">
									</div>
								</div>
							</div>
							<div class="col-md-1">
								<div class="form-group">
									<label>&nbsp;<span class="must">&nbsp;</span></label>
									<button onclick = "" type="submit" name="submit" class="btn btn-primary"><i class="fa fa-fw fa-save"></i> Save</button>
								</div>
							</div>
						</div>
					</form>
				</div>
			</div>
				
		</div>
		
		<table border="1" cellspacing="0" cellpadding="2" id="table">
				<tr>
					<th>No</th>
					<th>Account Name</th>
					<th>Account Number</th>
					<th>Action</th>
				</tr>
				<?php 
				$i=1;
				$db = new Db();
				$select = $db->select("SELECT * FROM account2");
				//echo $db->num_rows();
				foreach($select[0] as $row){
					extract($row);
					echo '<tr>';
					echo '<td width="1px">'.($i++).'.</td>';
					echo '<td>'.($account_name).'</td>';
					echo '<td>'.($account_number).'</td>';
					
					echo '<td width="200px">';
					//echo '<a href="'.return_url().'receipts/delete-account2/'.$account_id.'" class="btn  btn-xs btn-danger">Delete</a>';
					echo '&nbsp;'; 
					echo '<a href="#" class="btn btn-success btn-xs " id="add-hint'.$i.'">Edit</a>';
					echo '&nbsp;'; 
					echo '</tr>';

					$co = $i;
					?>

					<div id="myModal<?php echo $co; ?>" class="net-advance-form modal">
					 	<div class="modal-content">
					 		<form action="" method="post">
						 		<?php
						 		if(isset($_POST['saveChanges'.$co])){
						 			$account_number = $_POST['account_number'];
						 			$account_name = $_POST['account_name'];

						 			$db->update("account2", [
						 				"account_name"=>$account_name,
						 				"account_number"=>$account_number,
						 			], ["account_id"=>$account_id]);

						 			Feedback::redirect(return_url().'receipts/view-account2-list');
						 		}
						 		?>
						    	<span id="close<?php echo $co; ?>" class="close" style="color:red;">&times;</span>
						    	<h4>Edit Account</h4>
								<br/>
								Account Name:<br/>
								<input type="text" value="<?php echo $account_name; ?>" name="account_name"><br/><br/>
								Account Number:<br/>
								<input type="text" value="<?php echo $account_number; ?>" name="account_number">
						    	<br/><br/>
						    	<button name="saveChanges<?php echo $co; ?>" class="btn btn-success"> <i class="fa fa-fw fa-save"></i> Save Changes</button>
					    	</form>
						</div>

					</div>
					<script>
					document.getElementById("<?php echo 'add-hint'.$co; ?>").onclick = function() {
					document.getElementById("<?php echo 'myModal'.$co; ?>").style.display = "block";
					}
					document.getElementById("<?php echo 'close'.$co; ?>").onclick = function() {
					document.getElementById("<?php echo 'myModal'.$co; ?>").style.display = "none";
					}
					window.onclick = function(event) {
					if (event.target == document.getElementById("<?php echo 'myModal'.$co; ?>")) {
					document.getElementById("<?php echo 'myModal'.$co; ?>").style.display = "none";
					}
					}
					</script>

					<?php
				}
				?>
			</table>
		<?php
	}
	
	public function viewCategoryItems(){
		
		echo '<a href="'.return_url().'receipts/view-receipts"><i class="fa fa-fw fa-arrow-left"></i>View Receipts</a>';
		echo '&nbsp; &nbsp; ';
		echo '<a href="'.return_url().'receipts/add-receipt"><i class="fa fa-fw fa-plus"></i>Add Receipt</a>';
		echo '&nbsp; &nbsp; ';
		echo '<a href="'.return_url().'receipts/view-receipt-categories"><i class="fa fa-fw fa-eye"></i>View Receipt Categories</a>';
		echo '<br/>';
		echo '<br/>';
		$id = portion(3);
		if(empty($id)){
			$id = 3;
		}
		$name = "";
		$j = $this->receiptStatic();
		if($id==3){
			$name = $j["energy"][1];
		}elseif($id == 4){
			$name = $j["optic"][1];
		}else{
			$name = $this->rgf("receipt_category", $id, "rc_id", "rc_name");
		}
		
		echo '<form action="" method="post">';
		if(isset($_POST['another_category'])){
			$category = $_POST['category'];
			
			if($category){
				FeedBack::redirect(return_url().'receipts/view-category-items/'.$category);
			}
		}
		echo 'Select Another Category<br/>';
		echo '<select name="category">';
		$db = new Db();
		$select = $db->select("SELECT * FROM receipt_category");
		foreach($select[0] as $row){
			extract($row);
			if($rc_id == $id)
				echo '<option selected="selected" value="'.$rc_id.'">'.$rc_name.'</option>';
			else
				echo '<option value="'.$rc_id.'">'.$rc_name.'</option>';
		}
		echo '</select>';
		echo ' &nbsp; &nbsp; <input value="Show" type="submit" name="another_category"/>';
		echo '</form>';
		echo '<br/>';
				
		echo '<h4>'.$name.' category Items(Dropdown)</h4>';
		
		if($id == 3){//energy		
			?>
				<div class="row">
				<div class="col-lg-12">
					<form role="form" action="" method="post">
						<?php
						if(isset($_POST['submit'])){
							$cat_item = $_POST['cat_item'];
							
							$db = new Db();
							$insert = $db->insert("receipt_category_item", ["rci_name"=>$cat_item, "rci_date_added"=>time(), "rci_added_by"=>user_id(), "rci_category"=>$id]);
							
							if(empty($db->error())){
								Feedback::success();
								Feedback::refresh();
							}else{
								Feedback::error($db->error());
							}
						}
						?>
						<div class="row">
							<div class="col-md-6">
								<div class="form-group">
									<label>Add New Category Item<span class="must"></span></label>
									<div class="form-line">
										<input autocomplete="off" class="form-control" type="text" name="cat_item" placeholder="Enter Category Item" value="<?php echo @$cat_item; ?>">
									</div>
								</div>
							</div>
							<div class="col-md-1">
								<div class="form-group">
									<label>&nbsp;<span class="must">&nbsp;</span></label>
									<button onclick = "" type="submit" name="submit" class="btn btn-primary"><i class="fa fa-fw fa-save"></i> Save</button>
								</div>
							</div>
						</div>
					</form>
				</div>
			</div>
				
		</div>
		
			<?php	
			echo '<table border="1" cellspacing="0" cellpadding="2" id="table">';
			echo '<tr>';
			echo '<th>No.</th>';
			echo '<th>Customer</th>';
			echo '<th>Total Receipts</th>';
			echo '</tr>';
			
			$no = 1;
			$select = $db->select("SELECT * FROM customer");
			//echo $db->num_rows();
			foreach($select[0] as $row){
				extract($row);
				
				echo '<tr>';
				echo '<td width="20px">'.($no++).'.</td>';
				echo '<td>'.$customer_full_name.'</td>';
				echo '<td>'; 
				$sel = $db->select("SELECT * FROM receipts WHERE rec_category = '$id' AND rec_customer_id = '$customer_id'");
				echo $db->num_rows();
				echo '</td>';
				echo '</tr>';
			}
			
			echo '</table>';
		}elseif($id == 4){
			?>
				<div class="row">
				<div class="col-lg-12">
					<form role="form" action="" method="post">
						<?php
						if(isset($_POST['submit'])){
							$cat_item = $_POST['cat_item'];
							
							$db = new Db();
							$insert = $db->insert("receipt_category_item", ["rci_name"=>$cat_item, "rci_date_added"=>time(), "rci_added_by"=>user_id(), "rci_category"=>$id]);
							
							if(empty($db->error())){
								Feedback::success();
								Feedback::refresh();
							}else{
								Feedback::error($db->error());
							}
						}
						?>
						<div class="row">
							<div class="col-md-6">
								<div class="form-group">
									<label>Add New Category Item<span class="must"></span></label>
									<div class="form-line">
										<input autocomplete="off" class="form-control" type="text" name="cat_item" placeholder="Enter Category Item" value="<?php echo @$cat_item; ?>">
									</div>
								</div>
							</div>
							<div class="col-md-1">
								<div class="form-group">
									<label>&nbsp;<span class="must">&nbsp;</span></label>
									<button onclick = "" type="submit" name="submit" class="btn btn-primary"><i class="fa fa-fw fa-save"></i> Save</button>
								</div>
							</div>
						</div>
					</form>
				</div>
			</div>
				
		</div>
		
			<?php
			//optic			
			echo '<table border="1" cellspacing="0" cellpadding="2" id="table">';
			echo '<tr>';
			echo '<th>No.</th>';
			echo '<th>Customer</th>';
			echo '<th>Total Receipts</th>';
			echo '</tr>';
			
			$no = 1;
			$select = $db->select("SELECT * FROM of_customer");
			//echo $db->num_rows();
			foreach($select[0] as $row){
				extract($row);
				
				echo '<tr>';
				echo '<td width="20px">'.($no++).'.</td>';
				echo '<td>'.$ofc_customer_name.'</td>';
				echo '<td>';
				$sel = $db->select("SELECT * FROM receipts WHERE rec_category = '$id' AND rec_customer_id = '$ofc_id'");
				echo $db->num_rows();
				echo '</td>';
				echo '</tr>';
			}
			
			echo '</table>';
		}else{
			?>
			<div class="">	

			<div class="row">
				<div class="col-lg-12">
					<form role="form" action="" method="post">
						<?php
						if(isset($_POST['submit'])){
							$cat_item = $_POST['cat_item'];
							
							$db = new Db();
							$insert = $db->insert("receipt_category_item", ["rci_name"=>$cat_item, "rci_date_added"=>time(), "rci_added_by"=>user_id(), "rci_category"=>$id]);
							
							if(empty($db->error())){
								Feedback::success();
								Feedback::refresh();
							}else{
								Feedback::error($db->error());
							}
						}
						?>
						<div class="row">
							<div class="col-md-6">
								<div class="form-group">
									<label>Add New Category Item<span class="must"></span></label>
									<div class="form-line">
										<input autocomplete="off" class="form-control" type="text" name="cat_item" placeholder="Enter Category Item" value="<?php echo @$cat_item; ?>">
									</div>
								</div>
							</div>
							<div class="col-md-1">
								<div class="form-group">
									<label>&nbsp;<span class="must">&nbsp;</span></label>
									<button onclick = "" type="submit" name="submit" class="btn btn-primary"><i class="fa fa-fw fa-save"></i> Save</button>
								</div>
							</div>
						</div>
					</form>
				</div>
			</div>
				
		</div>
		
		<table border="1" cellspacing="0" cellpadding="2" id="table">
			<tr>
			<th width="20px">No.</th>
			<th>Customer</th>
			<th>Total Receipts</th>
			<th>Action</th>
			</tr>
			<?php
			$sql = "SELECT * FROM receipt_category_item WHERE rci_category = '$id'";
			$select = $db->select($sql);
			$i = 1;
			foreach($select[0] as $row){
				extract($row);
				echo '<tr>';
				echo '<td>'.($i++).'.</td>';
				echo '<td>'.$rci_name.'</td>';
				echo '<td width="150px">';
				$sel = $db->select("SELECT * FROM receipts WHERE rec_category = '$id' AND rec_customer_id = '$rci_id'");
				echo $db->num_rows();
				echo '</td>';
				echo '<td width="150px">';
				
					echo '<a href="'.return_url().'receipts/delete-category-item/'.$rci_id.'/'.portion(3).'" class="btn  btn-xs btn-danger">Delete</a>';
					echo '&nbsp;'; 
					echo '&nbsp;'; 
					echo '&nbsp;'; 
					echo '<a href="#" class="btn btn-success btn-xs" id="add-hint'.$i.'">Edit</a>';
					echo '&nbsp;'; 
					echo '</tr>';

					$co = $i;
					?>

					<div id="myModal<?php echo $co; ?>" class="net-advance-form modal">
					 	<div class="modal-content">
					 		<form action="" method="post">
						 		<?php
						 		if(isset($_POST['saveChanges'.$co])){
						 			$cat_name = $_POST['cat_name'];

						 			$db->update("receipt_category_item", [
						 				"rci_name"=>$cat_name,
						 			], ["rci_id"=>$rci_id]);

						 			Feedback::redirect(return_url().'receipts/view-category-items/'.portion(3));
						 		}
						 		?>
						    	<span id="close<?php echo $co; ?>" class="close" style="color:red;">&times;</span>
						    	<h4>Edit Category Item</h4>
						    	<textarea required class="form-control" style="height:100px;" name="cat_name"><?php echo $rci_name; ?></textarea>
						    	<br/>
						    	<button name="saveChanges<?php echo $co; ?>" class="btn btn-success"> <i class="fa fa-fw fa-save"></i> Save Changes</button>
					    	</form>
						</div>

					</div>
					<script>
					document.getElementById("<?php echo 'add-hint'.$co; ?>").onclick = function() {
					document.getElementById("<?php echo 'myModal'.$co; ?>").style.display = "block";
					}
					document.getElementById("<?php echo 'close'.$co; ?>").onclick = function() {
					document.getElementById("<?php echo 'myModal'.$co; ?>").style.display = "none";
					}
					window.onclick = function(event) {
					if (event.target == document.getElementById("<?php echo 'myModal'.$co; ?>")) {
					document.getElementById("<?php echo 'myModal'.$co; ?>").style.display = "none";
					}
					}
					</script>

					<?php
			}
			?>
		</table>
		<?php
		}
		
		
		?>
		
		
		<?php
	}

}
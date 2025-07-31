<?php
Class OpticFibreCustomers extends BeforeAndAfter{
	public $page = "CUSTOMERS";
	
	public function __construct(){
		$access = new AccessRights();
		//$access->pageAccess(user_id(), $this->page, 'V');
	}
	public $set_class = "";
	public $set_method = "";
	public $set_customer = "";
	public $set_year = "";
	public $set_month = "";

	public function parameters($href){
		$portion = explode('/',str_replace(return_url(), '', $href));

		$this->set_class = $portion[0];
		$this->set_method = $portion[1];
		$this->set_customer = $portion[2];
		$this->set_year = $portion[3];
		$this->set_month = $portion[4];
	}
	public function getLinks(){
		$page = "CUSTOMERS";
		
		

		$t = array(
				"link_name"=>"Customer Details", 
				"link_address"=>"optic-fibre-customers/optic-fibre-customer-details/".portion(3),
				"link_icon"=>"fa-eye",
				"link_page"=>$page,
				"link_right"=>"V",
			);
		$links = array(
			array(
				"link_name"=>"Add Customer", 
				"link_address"=>"optic-fibre-customers/add-customer",
				"link_icon"=>"fa-plus",
				"link_page"=>$page,
				"link_right"=>"A",
			),
			array(
				"link_name"=>"Customer List", 
				"link_address"=>"optic-fibre-customers/all-customers",
				"link_icon"=>"fa-eye",
				"link_page"=>$page,
				"link_right"=>"V",
			), 

		);
		
		return $links;
	}

	public function addCustomerAction(){
		if(isset($_POST['submit'])){
			$customer_name = $_POST['customer_name'];
			$postal_code = $_POST['postal_code'];
			$street = $_POST['street'];
			$email = $_POST['email'];
			$telephone = $_POST['telephone'];
			$address = trim($_POST['address']);
			$short_name = trim($_POST['short_name']);
			$tin_number = $_POST['tin_number'];
			$cust_type = trim($_POST['cust_type']);
			$journal_no = trim($_POST['journal_no']);
			$errors = array();

			$start_date = trim($_POST['start_date']);			
			$end_date = trim($_POST['end_date']);

			$tstart_date = strtotime($start_date);
			$tend_date = strtotime($end_date);

			if($tstart_date > $tend_date){
				$errors[] = "Contract End Date <b>(".$end_date.")</b> should be greater than Contract Start Date <b>(".$start_date.")</b>";
			}

			$time = time();
			$user = user_id();
			$db = new Db();
			

			if(empty($customer_name)){
				$errors[]="Enter Customer Name";
			}
			if(empty($short_name)){
				$errors[]="Enter Customer Short Name";
			}
			if(empty($postal_code)){
				$errors[]="Enter Postal Code and District";
			}
			if(empty($street)){
				$errors[]="Enter Plot no. and Street";
			}
			if(empty($email)){
				$errors[]="Enter Email";
			}
			if(empty($telephone)){
				$errors[]="Enter Telephone";
			}

			if($this->isThere("of_customer", ["ofc_customer_name"=>$customer_name])){
				$errors[]="Customer Name($customer_name) already exists";
			}
			
			if(empty($errors)){
				$db = new Db();
				$x = $db->insert("of_customer",
					[
						"ofc_customer_name"=>$customer_name,
						"ofc_journal_no"=>$journal_no, 
						"ofc_address"=>$address, 
						"ofc_added_by"=>user_id(), 
						"ofc_date_added"=>time(), 
						"ofc_street"=>$street, 
						"ofc_postal_code"=>$postal_code, 
						"ofc_tel"=>$telephone, 
						"ofc_email"=>$email,
						"ofc_tin_no"=>$tin_number,
						"ofc_cust_type"=>$cust_type,
						"ofc_short_name"=>$short_name

					]);
								
				if(!$db->error()){
					FeedBack::success();
					FeedBack::refresh(3, return_url().'optic-fibre-customers/optic-fibre-customer-details/'.$id);
				}else{
					FeedBack::error($db->error());
					
				}
			}else{
				FeedBack::errors($errors);
			}
		}
		?>
		<div class="col-md-12">
			<div class="col-lg-12">
			<br/>
			<div class="panel panel-default">
				<div class="panel-heading">
					Add Optic Fibre Customer Form
				</div>
				<div class="panel-body">
					<div id="must">All field with asterisk(*) are mandatory.</div>

					<div class="row">
						<div class="col-lg-12">
							<form role="form" action="" method="post">
								<div class="row">
									<div class="col-md-8">
										<div class="form-group">
											<label>Customer Name<span class="must">*</span></label>
											<div class="form-line">
												<input class="form-control" type="text" name="customer_name" placeholder="" value="<?php echo @$customer_name; ?>" autocomplete="off">
											</div>
										</div>
									</div>
									<div class="col-md-4">
										<div class="form-group">
											<label>Customer Short Name<span class="must">*</span></label>
											<div class="form-line">
												<input class="form-control" type="text" name="short_name" placeholder="" value="<?php echo @$short_name; ?>" autocomplete="off">
											</div>
										</div>
									</div>
									<div class="col-lg-12">
										<div class="form-group">
											<label>Address<span class="must">*</span></label>
											<div  class="col-lg-12">
												<div class="form-line">
													<label>Plot No. & Street</label>
													<textarea required style="height:50px;"class="form-control" name="street"><?php echo $street; ?></textarea>
												</div>
												<div class="form-line">
													<label>Postal code & District</label>
													<textarea style="height:50px;" class="form-control" name="postal_code"><?php echo $postal_code; ?></textarea>
												</div>
												<div class="row">
													<div class="col-md-3">
														<div class="form-line">
															<label>Telphone</label>
															<textarea style="height:50px;" class="form-control" name="telephone"><?php echo $telephone; ?></textarea>
														</div>
													</div>
													<div class="col-md-6">
														<div class="form-line">
															<label>Email</label>
															<textarea style="height:50px;" class="form-control" name="email"><?php echo $email; ?></textarea>
														</div>
													</div>
													<div class="col-md-3">
														<div class="form-line">
															<label>GL Account</label>
															<textarea style="height:50px;"class="form-control" name="journal_no"><?php echo $journal_no; ?></textarea>
														</div>
													</div>
													<div class="col-md-3">
														<label>TIN<span class="must"></span></label>
														<div class="form-line">
														  <input type="text" name="tin_number" value="<?php echo$customer_tin;?>" class="form-control" placeholder="TIN">
													   </div>
													  </div>
													  <div class="col-md-3">
														<label>Customer Type<span class="must"></span></label>
														<div class="form-line">
														  <select class="form-control" name="cust_type">
														  	<option <?php if($customer_type=='B2B') echo "selected";?> value="B2B">B2B</option>
														  	<option <?php if($customer_type=='B2G') echo "selected";?> value="B2G">B2G</option>
														  	<option <?php if($customer_type=='B2C') echo "selected";?> value="B2C">B2C</option>
														  	<option <?php if($customer_type=='B2F') echo "selected";?> value="B2F">B2F</option>
														  </select>
													   </div>
													  </div>
												</div>
											</div>
										</div>
									</div>
									<div class="clearfix"></div>
									<br/>
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
	
	public function addContract(){
		$customer = portion(3);
		$customer_name = $this->rgf("of_customer", $customer, "ofc_id", "ofc_customer_name");
		if(isset($_POST['submit'])){
			
			$errors = array();

			$narration = trim($_POST['narration']);	
			$start_date = trim($_POST['start_date']);			
			$end_date = trim($_POST['end_date']);		
			// = trim($_POST['end_date']);

			$tstart_date = strtotime($start_date);
			$tend_date = strtotime($end_date);

			if($tstart_date > $tend_date){
				$errors[] = "Contract End Date <b>(".$end_date.")</b> should be greater than Contract Start Date <b>(".$start_date.")</b>";
			}

			$time = time();
			$user = user_id();
			$db = new Db();
			
			
			if(empty($errors)){
				$db = new Db();

				$insert = $db->insert("contract", ["contract_start_date"=>$tstart_date, "contract_end_date"=>$tend_date, "contract_ofc_id"=>$customer, "contract_date_added"=>time(), "contract_added_by"=>user_id()]);
								
				if(!$db->error()){
					FeedBack::success();
					FeedBack::refresh(3, return_url().'optic-fibre-customers/optic-fibre-customer-details/'.$customer);
				}else{
					FeedBack::error($db->error());
					
				}
			}else{
				FeedBack::errors($errors);
			}
		}
		?>
		<div class="col-md-12">
			<div class="col-lg-12">
			<br/>
			<div class="panel panel-default">
				<div class="panel-heading">
					Add Contract to <?php echo $customer_name; ?>
				</div>
				<div class="panel-body">
					<div id="must">All field with asterisk(*) are mandatory.</div>

					<div class="row">
						<div class="col-lg-12">
							<form role="form" action="" method="post">
								<div class="row">
									<div class="col-md-12">
										<div class="form-group">
											<label>Customer Name<span class="must">*</span></label>
											<div class="form-line">
												<input disabled class="form-control" type="text" name="customer_name" placeholder="" value="<?php echo @$customer_name; ?>" autocomplete="off">
											</div>
										</div>
									</div>
									<div class="clearfix"></div>
									<div class="col-md-6">
										<div class="form-group">
											<label>Contract Start Date<span class="must">*</span></label>
											<div class="form-line">
												<input class="form-control" type="date" name="start_date" placeholder="" value="<?php echo @$start_date; ?>" autocomplete="off">
											</div>
										</div>
									</div>
									<div class="col-md-6">
										<div class="form-group">
											<label>Contract End Date<span class="must">*</span></label>
											<div class="form-line">
												<input class="form-control" type="date" name="end_date" placeholder="" id="endstart" value="<?php echo @$end_date; ?>" autocomplete="off">
											</div>
										</div>
									</div>
									
									<div class="clearfix"></div>
									<br/>
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


	public function editContract(){
		$contract_id = portion(3);
		$customer = portion(4);
		$db = new Db();
		$select = $db->select("SELECT * FROM contract WHERE contract_id = '$contract_id'");
		extract($select[0][0]);
		$start_date = date('Y-m-d', $contract_start_date);
		$end_date = date('Y-m-d', $contract_end_date);

		$customer_name = $this->rgf("of_customer", $customer, "ofc_id", "ofc_customer_name");
		if(isset($_POST['submit'])){
			
			$errors = array();

			$narration = trim($_POST['narration']);	
			$start_date = trim($_POST['start_date']);			
			$end_date = trim($_POST['end_date']);		
			// = trim($_POST['end_date']);

			$tstart_date = strtotime($start_date);
			$tend_date = strtotime($end_date);

			if($tstart_date > $tend_date){
				$errors[] = "Contract End Date <b>(".$end_date.")</b> should be greater than Contract Start Date <b>(".$start_date.")</b>";
			}

			$time = time();
			$user = user_id();
			$db = new Db();
			
			
			if(empty($errors)){
				$db = new Db();

				$insert = $db->update("contract", ["contract_start_date"=>$tstart_date, "contract_end_date"=>$tend_date, "contract_ofc_id"=>$customer, "contract_date_added"=>time(), "contract_added_by"=>user_id()],["contract_id"=>$contract_id]);
								
				if(!$db->error()){
					FeedBack::success();
					FeedBack::refresh(3, return_url().'optic-fibre-customers/optic-fibre-customer-details/'.$customer);
				}else{
					FeedBack::error($db->error());
					
				}
			}else{
				FeedBack::errors($errors);
			}
		}
		?>
		<div class="col-md-12">
			<div class="col-lg-12">
			<br/>
			<div class="panel panel-default">
				<div class="panel-heading">
					Add Contract to <?php echo $customer_name; ?>
				</div>
				<div class="panel-body">
					<div id="must">All field with asterisk(*) are mandatory.</div>

					<div class="row">
						<div class="col-lg-12">
							<form role="form" action="" method="post">
								<div class="row">
									<div class="col-md-12">
										<div class="form-group">
											<label>Customer Name<span class="must">*</span></label>
											<div class="form-line">
												<input disabled class="form-control" type="text" name="customer_name" placeholder="" value="<?php echo @$customer_name; ?>" autocomplete="off">
											</div>
										</div>
									</div>
									<div class="clearfix"></div>
									<div class="col-md-6">
										<div class="form-group">
											<label>Contract Start Date<span class="must">*</span></label>
											<div class="form-line">
												<input class="form-control" type="date" name="start_date" placeholder="" value="<?php echo @$start_date; ?>" autocomplete="off">
											</div>
										</div>
									</div>
									<div class="col-md-6">
										<div class="form-group">
											<label>Contract End Date<span class="must">*</span></label>
											<div class="form-line">
												<input class="form-control" type="date" name="end_date" placeholder="" id="endstart" value="<?php echo @$end_date; ?>" autocomplete="off">
											</div>
										</div>
									</div>
									
									<div class="clearfix"></div>
									<br/>
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


	
	public function terminateContract(){
		$contract_id = portion(3);
		$customer = portion(4);
		$db = new Db();
		$select = $db->select("SELECT * FROM contract WHERE contract_id = '$contract_id'");
		extract($select[0][0]);
		$start_date = date('Y-m-d', $contract_start_date);
		$end_date = date('Y-m-d', $contract_end_date);

		$customer_name = $this->rgf("of_customer", $customer, "ofc_id", "ofc_customer_name");
		if(isset($_POST['submit'])){
			
			$errors = array();
	
			$reason = trim($_POST['reason']);			
			$date = trim($_POST['date']);		
			// = trim($_POST['end_date']);
			if(empty($date))
				$errors[]="Select Termination Date";
			if(empty($reason))
				$errors[]="Enter Reason";

			$tdate = strtotime($date);
			
			$time = time();
			$user = user_id();
			$db = new Db();
			
			if(empty($errors)){
				$db = new Db();

				$insert = $db->update("contract", ["contract_t_date"=>$tdate, "contract_t_reason"=>$reason],["contract_id"=>$contract_id]);
								
				if(!$db->error()){
					FeedBack::success();
					FeedBack::refresh(3, return_url().'optic-fibre-customers/optic-fibre-customer-details/'.$customer);
				}else{
					FeedBack::error($db->error());
					
				}
			}else{
				FeedBack::errors($errors);
			}
		}
		?>
		<div class="col-md-12">
			<div class="col-lg-12">
			<br/>
			<div class="panel panel-default">
				<div class="panel-heading">
					Termination Contract for <?php echo $customer_name; ?>
				</div>
				<div class="panel-body">
					<div id="must">All field with asterisk(*) are mandatory.</div>

					<div class="row">
						<div class="col-lg-12">
							<form role="form" action="" method="post">
								<div class="row">
									<div class="col-md-12">
										<div class=""><br/>
											<label>Customer Name:</label> <?php echo @$customer_name; ?>
										</div>
										<div class="">
											<label>Contract Start Date: </label> <?php echo @$start_date; ?>
										</div>
										<div class="">
											<label>Contract End Date: </label> <?php echo @$end_date; ?><br/><br/>
										</div>
									</div>
									<div class="col-md-3">
										<div class="form-group">
											<label>Termination Date <span class="must">*</span></label>
											<div class="form-line">
												<input class="form-control" type="date" name="date" placeholder="" id="endstart" value="<?php echo @$date; ?>" autocomplete="off">
											</div>
										</div>
									</div>
									<div class="col-md-12">
										<div class="form-group">
											<label>Termination Reason<span class="must">*</span></label>
											<div class="form-line">
												<textarea class="form-control" name="reason"><?php echo $reason; ?></textarea>
											</div>
										</div>
									</div>
									
									<div class="clearfix"></div>
									<br/>
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


	public function editCustomerDetailsAction(){
		
		$id = ($this->set_customer)?$this->set_customer:portion(3);

		$s = new Db();
		$ss = $s->select("SELECT * FROM of_customer WHERE ofc_id = '$id'");
		extract($ss[0][0]);

		$postal_code = $ofc_postal_code;
		$street = $ofc_street;
		$email = $ofc_email;
		$telephone = $ofc_tel;
		$address = trim($ofc_address);

		$s = new Db();
		$ss = $s->select("SELECT * FROM contract WHERE contract_ofc_id = '$id'");
		extract($ss[0][0]);

		if(isset($_POST['submit'])){

			$customer_name = $_POST['customer_name'];
			$short_name = $_POST['short_name'];
			$postal_code = $_POST['postal_code'];
			$street = $_POST['street'];
			$email = $_POST['email'];
			$journal_no = $_POST['journal_no'];
			$telephone = $_POST['telephone'];
			$address = trim($_POST['address']);

			$tin_number = $_POST['tin_number'];
			$cust_type = trim($_POST['cust_type']);
			
			$errors = array();

			$start_date = trim($_POST['start_date']);			
			$end_date = trim($_POST['end_date']);

			$tstart_date = strtotime($start_date);
			$tend_date = strtotime($end_date);

			if($tstart_date > $tend_date){
				$errors[] = "Contract End Date <b>(".$end_date.")</b> should be greater than Contract Start Date <b>(".$start_date.")</b>";
			}

			$time = time();
			$user = user_id();
			$db = new Db();
			

			if(empty($customer_name)){
				$errors[]="Enter Customer Name";
			}
			
			// if(empty($customer_name)){
			// 	$errors[]="Enter Customer Name";
			// }
			// if(empty($postal_code)){
			// 	$errors[]="Enter Postal Code and District";
			// }
			// if(empty($street)){
			// 	$errors[]="Enter Plot no. and Street";
			// }
			// if(empty($email)){
			// 	$errors[]="Enter Email";
			// }
			// if(empty($telephone)){
			// 	$errors[]="Enter Telephone";
			// }

			if($this->isThereEdit("of_customer", ["ofc_customer_name"=>$customer_name, "ofc_id"=>$id])){
				$errors[]="Customer Name($customer_name) already exists";
			}
			
			if(empty($errors)){
				$db = new Db();
				$x = $db->update("of_customer",[
					"ofc_customer_name"=>$customer_name,
					"ofc_journal_no"=>$journal_no,
					"ofc_address"=>$address, 
					"ofc_added_by"=>user_id(), 
					"ofc_date_added"=>time(), 
					"ofc_street"=>$street, 
					"ofc_postal_code"=>$postal_code, 
					"ofc_email"=>$email, 
					"ofc_tel"=>$telephone,
					"ofc_tin_no"=>$tin_number,
					"ofc_cust_type"=>$cust_type,
					"ofc_short_name"=>$short_name
				], ["ofc_id"=>$id]);

				$j = new Db();
				$select = $j->select("SELECT TOP 1 contract_id FROM contract WHERE contract_ofc_id = '$id' ORDER BY contract_id DESC");				

				if($j->num_rows()){
					extract($select[0][0]);
					$insert = $db->update("contract", ["contract_start_date"=>$tstart_date, "contract_end_date"=>$tend_date, "contract_date_added"=>time(), "contract_added_by"=>user_id()],["contract_id"=>$contract_id]);
				}else{
					$insert = $db->insert("contract", ["contract_start_date"=>$tstart_date, "contract_end_date"=>$tend_date, "contract_ofc_id"=>$id, "contract_date_added"=>time(), "contract_added_by"=>user_id()]);
				}

				if(!$db->error()){
					FeedBack::success();
					FeedBack::refresh(3, return_url().'optic-fibre-customers/all-customers');
				}else{
					FeedBack::error($db->error());
					
				}
			}else{
				FeedBack::errors($errors);
			}
		}
		?>
		<div class="col-md-12">
			<div class="col-lg-12">
			<br/>
			<div class="panel panel-default">
				<div class="panel-heading">
					Edit Optic Fibre Customer Form
				</div>
				<div class="panel-body">
					<div id="must">All field with asterisk(*) are mandatory.</div>

					<div class="row">
						<div class="col-lg-12">
							<form role="form" action="" method="post">
								<div class="row">
									<div class="col-md-8">
										<div class="form-group">
											<label>Customer Name<span class="must">*</span></label>
											<div class="form-line">
												<input class="form-control" type="text" name="customer_name" placeholder="" value="<?php echo @$ofc_customer_name; ?>" autocomplete="off">
											</div>
										</div>
									</div>
									<div class="col-md-4">
										<div class="form-group">
											<label>Customer Short Name<span class="must">*</span></label>
											<div class="form-line">
												<input class="form-control" type="text" name="short_name" placeholder="" value="<?php echo @$ofc_short_name; ?>" autocomplete="off">
											</div>
										</div>
									</div>
									<div class="clearfix"></div>
									<div class="col-md-6">
										<div class="form-group">
											<label>Contract Start Date<span class="must">*</span></label>
											<div class="form-line">
												<input class="form-control" type="date" name="start_date" placeholder="" value="<?php if($contract_start_date) echo date('Y-m-d', @$contract_start_date); ?>" autocomplete="off">
											</div>
										</div>
									</div>
									<div class="col-md-6">
										<div class="form-group">
											<label>Contract End Date<span class="must">*</span></label>
											<div class="form-line">
												<input class="form-control" type="date" name="end_date" placeholder="" id="endstart" value="<?php if($contract_end_date) echo date('Y-m-d', @$contract_end_date); ?>" autocomplete="off">
											</div>
										</div>
									</div>
									<div class="clearfix"></div>
									<div class="col-lg-12">
										<div class="form-group">
											<label>Address<span class="must">*</span></label>
											<div  class="col-lg-12">
												<div class="form-line">
													<label>Plot No. & Street</label>
													<textarea required style="height:50px;"class="form-control" name="street"><?php echo $street; ?></textarea>
												</div>
												<div class="form-line">
													<label>Postal code & District</label>
													<textarea style="height:50px;"class="form-control" name="postal_code"><?php echo $postal_code; ?></textarea>
												</div>
												<div class="row">
													<div class="col-md-3">
														<div class="form-line">
															<label>Telphone</label>
															<textarea style="height:50px;"class="form-control" name="telephone"><?php echo $telephone; ?></textarea>
														</div>
													</div>
													<div class="col-md-6">
														<div class="form-line">
															<label>Email</label>
															<textarea style="height:50px;"class="form-control" name="email"><?php echo $email; ?></textarea>
														</div>
													</div>
													<div class="col-md-3">
														<div class="form-line">
															<label>Journal No.</label>
															<textarea style="height:50px;"class="form-control" name="journal_no"><?php echo $ofc_journal_no; ?></textarea>
														</div>
													</div>
													<div class="col-md-3">
														<label>TIN<span class="must"></span></label>
														<div class="form-line">
														  <input type="text" name="tin_number" value="<?php echo$ofc_tin_no;?>" class="form-control" placeholder="TIN">
													   </div>
													  </div>
													  <div class="col-md-3">
														<label>Customer Type<span class="must"></span></label>
														<div class="form-line">
														  <select class="form-control" name="cust_type">
														  	<option <?php if($ofc_cust_type=='B2B') echo "selected";?> value="B2B">B2B</option>
														  	<option <?php if($ofc_cust_type=='B2G') echo "selected";?> value="B2G">B2G</option>
														  	<option <?php if($ofc_cust_type=='B2C') echo "selected";?> value="B2C">B2C</option>
														  	<option <?php if($ofc_cust_type=='B2F') echo "selected";?> value="B2F">B2F</option>
														  </select>
													   </div>
													  </div>
												</div>
											</div>
										</div>
									</div>
									<div class="clearfix"></div>
									<br/>
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


	public function AllCustomersAction(){


	$access = new AccessRights();
	?>
		<div class="col-md-12">
			<h3>All Optic Fibre Customers List</h3>
			<?php 
			
			if($this->notifyAll()){
				echo '<button class="pull-right btn btn-danger btn-xs">Total Raised Invoices: '.$this->notifyAll().'</button>';
				echo '<div class="clearfix"></div>';
				echo '<br/>';
			}

			$db = new Db();
			$select = $db->select("SELECT * FROM of_customer ORDER BY ofc_customer_name ASC");
			
			if(!$select){
				$db->error();
			}else{
				echo '<table cellspacing="0" cellpadding="2" border="1" width="100%" id="table">';
				echo '<theadq>';
				echo '<tr>';
				echo '<th width="30px">No.</th>';
				echo '<th>Customer Name</th>';
				echo '<th>Short Name</th>';
				echo '<th>Services</th>';
				//echo '<th width="100px">Raised Invoice</th>';
				if($access->sectionAccess(user_id(), $this->page, 'E') || $access->sectionAccess(user_id(), $this->page, 'D')){
					echo '<th width="">Action</th>';
				}
				echo '</tr>';
				echo '</theadw>';
				
					
				$i=1;
				echo '<tbodyw>';
				foreach($select[0] as $row){
					extract($row);
					$t=$this->total("fibre_service", "fs_customer_id", $ofc_id);
					if(0){
						if($this->notify($ofc_id))
							echo '<tr valign="top" style="background-color:#ecc2c2;">';
						else
							echo '<tr>';
					}else{
						echo '<tr>';
					}

					
					echo '<td><center>'.($i++).'.</center></td>';
					echo '<td>'.ucwords($ofc_customer_name).'</td>';
					echo '<td>'.ucwords($ofc_short_name).'</td>';
					if(0){
						echo '<td>'.number_format($t).'</td>';
						echo '<td>'.$this->notify($ofc_id).'</td>';
					}else{
						echo '<td>'.number_format($t).'</td>';
						//echo '<td></td>';
					}
					
					if($access->sectionAccess(user_id(), $this->page, 'A')){					
						echo '<td style="width:200px;">';
						if($access->sectionAccess(user_id(), $this->page, 'A')){
							echo ' &nbsp; '.$this->action('view','optic-fibre-customers/optic-fibre-customer-details/'.$ofc_id, 'View');
						}
						if($access->sectionAccess(user_id(), $this->page, 'A')){
							echo ' &nbsp; '.$this->action('edit','optic-fibre-customers/edit-customer-details/'.$ofc_id, 'Edit');
						}
						
						if($access->sectionAccess(user_id(), $this->page, 'A')){
							echo ' &nbsp; '.$this->action('delete','optic-fibre-customers/delete-customer-details/'.$ofc_id, 'Delete');
						}
						
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

	public function deleteCustomerDetails(){
		$id = portion(3);
		$this->deletor("of_customer", "ofc_id",  $id, 'optic-fibre-customers/all-customers/'.$id);
	}


	public function AllOpticFibreInvoicesAction(){

	$access = new AccessRights();
	?>
		<div class="col-md-12">
			<?php 
			
			if($this->notifyAll()){
				echo '<button class="pull-right btn btn-danger btn-xs">Total Raised Invoice: '.$this->notifyAll().'</button>';
				echo '<div class="clearfix"></div>';
				echo '<br/>';
			}

			$db = new Db();
			$select = $db->select("SELECT * FROM of_customer ORDER BY ofc_customer_name ASC");
			
			if(!$select){
				$db->error();
			}else{
				echo '<table cellspacing="0" cellpadding="2" border="1" width="100%" id="table">';
				echo '<theaddd>';
				echo '<tr>';
				echo '<th width="30px">No.</th>';
				echo '<th>Customer Name</th>';
				echo '<th>Contract Period</th>';
				echo '<th>Services</th>';
				echo '<th width="100px">Raised Invoices</th>';
				if($access->sectionAccess(user_id(), $this->page, 'E') || $access->sectionAccess(user_id(), $this->page, 'D')){
					echo '<th width="">Action</th>';
				}
				echo '</tr>';
				echo '</theaddd>';
				
					
				$i=1;
				echo '<tbody>';
				foreach($select[0] as $row){
					extract($row);
					$t=$this->total("fibre_service", "fs_customer_id", $ofc_id);
					if($t){
						if($this->notify($ofc_id))
							echo '<tr valign="top" style="background-color:#ecc2c2;">';
						else
							echo '<tr>';
					}else{
						echo '<tr>';
					}

					
					echo '<td><center>'.($i++).'.</center></td>';
					echo '<td>'.ucwords($ofc_customer_name).' ('.$ofc_short_name.')'.'</td>';
					echo '<td>';

					$b = new Db();
					$bb = $b->select("SELECT * FROM contract WHERE contract_ofc_id = '$ofc_id'");
					if($b->num_rows()){
						extract($bb[0][0]);
						echo Feedback::date_s($contract_start_date);
						echo ' - ';
						echo Feedback::date_s($contract_end_date);
					}

					echo '</td>';

					if($t){
						echo '<td>'.number_format($t).'</td>';
						echo '<td>'.$this->notify($ofc_id).'</td>';
					}else{
						echo '<td>'.number_format($t).'</td>';
						echo '<td></td>';
					}
					
					if($access->sectionAccess(user_id(), $this->page, 'A')){					
						echo '<td>';
						if($access->sectionAccess(user_id(), $this->page, 'A')){
							echo ' &nbsp; '.$this->action('view','optic-fibre-customers/optic-fibre-invoices/'.$ofc_id, 'View Invoices');
						}
						// if($access->sectionAccess(user_id(), $this->page, 'A')){
						// 	echo ' &nbsp; '.$this->action('edit','optic-fibre-customers/edit-customer-details/'.$ofc_id, 'Edit Details');
						// }
						
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


	public function allExpiredContractsAction(){


	$access = new AccessRights();
	?>
		<div class="col-md-12">
			<h3>All Optic Fibre Customers List</h3>
			<?php 
			$jj  = $this->expiredContracts();
			if(0){
				echo '<button class="pull-right btn btn-danger btn-xs">Total Raised Invoice: '.$this->notifyAll().'</button>';
				echo '<div class="clearfix"></div>';
				echo '<br/>';
			}

			$db = new Db();
			$select = $db->select("SELECT * FROM of_customer ORDER BY ofc_customer_name ASC");
			
			if(!$select){
				$db->error();
			}else{
				echo '<table cellspacing="0" cellpadding="2" border="1" width="100%" id="table">';
				echo '<thead>';
				echo '<tr>';
				echo '<th width="30px">No.</th>';
				echo '<th>Customer Name</th>';
				echo '<th>Period</th>';
				echo '<th width="">Duration</th>';
				if($access->sectionAccess(user_id(), $this->page, 'E') || $access->sectionAccess(user_id(), $this->page, 'D')){
					echo '<th width="">Action</th>';
				}
				echo '</tr>';
				echo '</thead>';
				
					
				$i=1;
				echo '<tbody>';
				foreach($select[0] as $row){
					extract($row);
					if(in_array($ofc_id, $jj)){
						
						$t=$this->total("fibre_service", "fs_customer_id", $ofc_id);
						if($t){							
							echo '<tr>';
						}else{
							echo '<tr>';
						}

						
						echo '<td><center>'.($i++).'.</center></td>';
						echo '<td>'.ucwords($ofc_customer_name).'</td>';
						echo '<td>';

						$b = new Db();
						$bb = $b->select("SELECT * FROM contract WHERE contract_ofc_id = '$ofc_id'");
						if($b->num_rows()){
							extract($bb[0][0]);
							echo Feedback::date_s($contract_start_date);
							echo ' - ';
							echo Feedback::date_s($contract_end_date);
						}

						echo '</td>';
						$tg = time() - $contract_end_date;
						echo '<td style="text-align: right;">'.FeedBack::minuteDifference($tg/60).'</td>';
						
						if($access->sectionAccess(user_id(), $this->page, 'A')){					
							echo '<td>';
							if($access->sectionAccess(user_id(), $this->page, 'A')){
								echo ' &nbsp; '.$this->action('view','optic-fibre-customers/optic-fibre-customer-details/'.$ofc_id, 'View Details');
							}
							if($access->sectionAccess(user_id(), $this->page, 'A')){
								echo ' &nbsp; '.$this->action('edit','optic-fibre-customers/edit-customer-details/'.$ofc_id, 'Edit Details');
							}
							
							echo '</td>';
						}

						echo '</tr>';
					}
					
				}
				echo '</tbody>';
				
				echo '</table>';			
							
				//echo $this->emailNotify();
			}
			
			?>
		</div>
		<?php
	}

	public function deleteContract(){
		$id = portion(3);
		$id2 = portion(4);
		$this->deletor("contract", "contract_id",  $id, 'optic-fibre-customers/optic-fibre-customer-details/'.$id2);
	}

	public function deleteGLAccountAction(){
		$id = portion(3);
		$id2 = portion(4);
		$db = new Db();
		$sql = "DELETE FROM gl_account WHERE gl_id = '$id2'";
		$delete = $db->query($sql);
		FeedBack::redirect(return_url().'optic-fibre-customers/add-gl-account/'.$id);
	}
	public function addGlAccountAction(){
		$id =  portion(3);
		$id2 =  portion(4);
		if($id2){	
		 $db = new Db();	
		  $ss = $db->select("SELECT * FROM gl_account WHERE gl_cust_id = '$id' AND gl_entity='FIBRE' AND gl_id='$id2'");
		  if($db->num_rows()){
		  	extract($ss[0][0]);
		  }
		}
		$db = new Db();
		$select = $db->select("SELECT * FROM of_customer WHERE ofc_id = '$id'");
		extract($select[0][0]);
		$efd=$fs_effective_date;
		echo "<h2>GL ACCOUNT</h2>";
		echo '<style>#y tr td{padding:10px;}</style>';
		echo '<table id="y">';

		echo '<tr valign="top">';
		echo '<td style="width:200px;"><b>Customer Name:</b></td>';
		echo '<td>'.$ofc_customer_name.'</td>';
		echo '</tr>';
		
		echo '<tr valign="top">';
		echo '<td style="width:200px;"><b>Tin:</b></td>';
		echo '<td>'.$ofc_tin_no.'</td>';
		echo '</tr>';
		echo '<tr valign="top">';
		echo '<td style="width:200px;"><b>Customer Type:</b></td>';
		echo '<td>'.$ofc_cust_type.'</td>';
		echo '</tr>';
		
		
		
		echo '</table>';
		echo"<br><br>";
			?>
			<div class="row">
				<div class="col-md-3">
					<div class="form-group">
						<label class="form-label">GL Account</label>
						<select class="select2 form-control" style="width: 100%" id="gl_accounts">
							<option value="">select</option>
							<?php 
							$db = new Db();
		                    $select = $db->select("SELECT * FROM dictionary WHERE d_category = '9'");
								foreach ($select[0] as $k) {
									extract($k);
									if($gl_account == $d_id)
									echo'<option selected value="'.@trim($d_id).'">'.@trim($d_description).' ('.@trim($d_code).')</option>';
								else
									echo'<option value="'.@trim($d_id).'">'.@trim($d_description).' ('.@trim($d_code).')</option>';

								}
							?>
						</select>
					</div>
				</div>
				<script type="text/javascript">
					   $('.select2').select2();
				</script>
				
				<div class="col-md-2">
					<div class="form-group">
						<label class="form-label">GL Label</label>
						<select class="form-control" id="gl_label">
							<option value="">--select---</option>
							<?php 
							$db = new Db();
		                    $select = $db->select("SELECT * FROM dictionary WHERE d_category = '12'");
								foreach ($select[0] as $k) {
									extract($k);
									if($d_id == $gl_label)
									echo'<option selected value="'.$d_id.'">'.$d_description.'</option>';
								else
									echo'<option value="'.$d_id.'">'.$d_description.'</option>';

								}
							?>
						</select>
					</div>
				</div>
				<div class="col-md-2">
					<div class="form-group">
						<label class="form-label">Ct/Dt</label>
						<select class="select3 form-control" id="ct_dt">
							<option value="">--select---</option>
							<option <?php if($gl_ct_bt=='Credit') echo "selected";?> value="Credit">Credit</option>
							<option <?php if($gl_ct_bt=='Debit') echo "selected";?> value="Debit">Debit</option>
						</select>
					</div>
				</div>
				<div class="col-md-12">
					<input type="hidden" id="cust_id" value="<?php echo $id;?>" name="">
					<input type="hidden" id="account_id" value="<?php echo $id2;?>" name="">

					<?php
					if($id2){
						$save_name ="Update";
					}else{
						$save_name ="Save";
					}
					?>
				<button class="btn btn-primary btn-xs" id="save_gl_account"><i class="fa fa-save"></i> <?php echo $save_name;?></button><span id="save_gl_account_status"></span>
					<script type="text/javascript">
				        $(document).off('click','#save_gl_account').on('click', '#save_gl_account', function(e){

				        var gl_accounts = $('#gl_accounts').val();
				        var gl_label = $('#gl_label').val();
				        var ct_dt = $('#ct_dt').val();
				        var cust_id = $('#cust_id').val();
				        var account_id = $('#account_id').val();
				        

				        var errors = new Array(); 
				        if(gl_accounts==""){
				            errors.push("Please Select GL Account");
				        }
				        if(gl_label==""){
				            errors.push("Please Select GL Label");
				        }
				         if(ct_dt==""){
				            errors.push("Please Select Ct/Dt");
				        }
				        

				        if(errors.length >= 1){
				            alert("Please Review the following:\n "+errors.join("\n"));
				        }else{
				            $(this).html("Adding Account");
				            $(this).attr("disabled", true);
				            var form_data = new FormData();
				            form_data.append('gl_accounts', gl_accounts);
				            form_data.append('gl_label', gl_label);
				            form_data.append('ct_dt', ct_dt);
				            form_data.append('cust_id', cust_id);
				            form_data.append('account_id', account_id);
				            form_data.append('account_entity', 'FIBRE');
				           

				            $.ajax({
				            url:"<?php echo return_url().'ajax/gl_accounts.php';?>",
				            type: "POST",
				            data: form_data,
				            contentType: false,
				            cache: false,
				            processData:false,

				            success: function(data){
				                //alert(data);
				                var values = JSON.parse(data);
				                if (values.message=='Error') {
				                    $('#save_gl_account').html("Adding Account");
				                    $('#save_gl_account').attr('disabled',false);
				                    $('#save_gl_account_status').html('Not Added');
				                }else{
				                    //location.reload();customers/energy-customer-details/201
				                     window.location.replace("<?php echo return_url().'optic-fibre-customers/add-gl-account/'.$id;?>");
				                    $('#save_gl_account_status').html('Successfully Saved');
				                }
				            }
				        
				        }); 
				        }

				    });
				    </script>        
			   
			    <?php
			    echo "<br><br>";
			    $select = $db->select("SELECT * FROM gl_account WHERE gl_cust_id = '$id' AND gl_entity='FIBRE'");
			    if($select){
			    	echo '<table id="table2" border="1">';
					echo '<thead>';
					echo '<tr>';
					echo '<th width="30px">No.</th>';
					echo '<th>GL Account</th>';
					echo '<th>GL Label</th>';
					echo '<th>Ct/Dt</th>';
					echo '<th style="width:150px;">Action</th>';
					echo '</tr>';
					echo '</thead>';
					$no = 1;
					foreach($select[0] as $row){
						extract($row);
						echo '<tr>';
						echo '<td>'.($no++).'.</td>';
						echo '<td>'.strtoupper($this->rgf("dictionary", $gl_account, "d_id", "d_name")).' (<b>'.strtoupper($this->rgf("dictionary", $gl_account, "d_id", "d_code")).'</b>)</td>';
						echo '<td>'.strtoupper($this->rgf("dictionary", $gl_label, "d_id", "d_name")).'</td>';
						echo '<td>'.$gl_ct_bt.'</td>';
						
						echo '<td style="text-align:right">
						<a href="'.return_url().'optic-fibre-customers/delete-gl-account/'.$id.'/'.$gl_id.'" class="btn btn-danger btn-xs"><i class="fa fa-fx fa-times"></i> Delete</a> &nbsp;
						<a href="'.return_url().'optic-fibre-customers/add-gl-account/'.$id.'/'.$gl_id.'" class="eagle-load1 btn btn-success btn-xs"><i class="fa fa-fx fa-edit"></i> Edit</a></td>';
						echo '</tr>';
					}

					echo '</table>';
				}
			    ?>
			     </div>
			</div>
			<?php
		echo '</div>';
		echo '</div>';

	}
	public function opticFibreCustomerDetailsAction(){


		//$class = ($this->set_class)?$this->set_class:portion(1);
		//$method = ($this->set_method)?$this->set_method:portion(2);
		$id = ($this->set_customer)?$this->set_customer:portion(3);
		//$year = ($this->set_year)?$this->set_year: portion(4);
		//$month = ($this->set_month)?$this->set_month: portion(5);

		//$id = portion(3);

		$db = new Db();
		$select = $db->select("SELECT * FROM of_customer WHERE ofc_id = '$id'");
		extract($select[0][0]);
		$efd=$fs_effective_date;

		echo '<style>#y tr td{padding:10px;}</style>';
		echo '<table id="y">';

		echo '<tr valign="top">';
		echo '<td style="width:200px;"><b>Customer Name:</b></td>';
		echo '<td>'.$ofc_customer_name.'</td>';
		echo '</tr>';
		
		echo '<tr valign="top">';
		echo '<td style="width:200px;"><b>Tin:</b></td>';
		echo '<td>'.$ofc_tin_no.'</td>';
		echo '</tr>';
		echo '<tr valign="top">';
		echo '<td style="width:200px;"><b>Customer Type:</b></td>';
		echo '<td>'.$ofc_cust_type.'</td>';
		echo '</tr>';
		
		echo '<tr valign="top">';
		echo '<td><b>Customer Address:</b></td>';
		echo '<td>';

		if(!empty($ofc_street)){
			echo $ofc_street.'<br/>';
			echo $ofc_postal_code.'<br/>';
			echo $ofc_tel.'<br/>';
			echo $ofc_email.'<br/>';
		}else{
			echo nl2br($ofc_address);
		}
		echo '</td>';
		echo '</tr>';
		
		echo '</table>';

		//if($this->notify($ofc_id, 0) <= 1)
		{
				

			echo '&nbsp;&nbsp;&nbsp;&nbsp; <a class="right" href="'.return_url().'optic-fibre-customers/add-contract/'.$id.'"><button class="btn btn-success">Add Contract</button></a> 

			 <center><a class=" btn btn-info" href="'.return_url().'optic-fibre-customers/add-gl-account/'.$id.'">GL ACCOUNT</a></center>
			';
		}

			echo '<a class="left" href="'.return_url().'optic-fibre-customers/edit-customer-details/'.$id.'"><button class="btn btn-warning">Edit Customer Details</button></a>';
			echo ' &nbsp;  &nbsp;  &nbsp;  &nbsp;  &nbsp; ';
			echo '<a class="left" href="'.return_url().'optic-fibre-customers/optic-fibre-invoices/'.$id.'"><button class="btn btn-primary">View Customer Invoices</button></a>';
		
		echo '<br/><br/>';
		echo '<table id="table" border="1">';
		echo '<tr>';
		echo '<th width="40px">No.</th>';
		echo '<th>Period</th>';
		echo '<th>Services</th>';
		echo '<th>Action</th>';
		echo '</tr>';
		$k=$i=1;
		$db = new Db();
		$select = $db->select("SELECT * FROM contract WHERE contract_ofc_id = '$id' ORDER BY contract_start_date ASC");
		foreach($select[0] as $row){
			$no=$k++;

			extract($row);
			echo '<tr>';
			echo '<td>'.($k++).'.</td>';
			echo '<td>';
			echo Feedback::date_s($contract_start_date);
			echo ' - ';
			echo Feedback::date_s($contract_end_date);
			echo '<br/>';
			echo Feedback::status2($contract_start_date, $contract_end_date, $ofc_id);
			echo '</td>';
			echo '<td>'.$this->total("fibre_service", "fs_contract", $contract_id).'</td>';
			echo '<td style="width:320px;">';
			$terminated = $this->isTerminated($contract_id);
			if(!$terminated){
				echo '<a href="#" class="btn btn-success btn-xs " id="add-service'.$no.'" title="Add Service to Contract">Add Service</a>';
				echo '&nbsp;&nbsp;';
				echo '<a href="'.return_url().'optic-fibre-customers/edit-contract/'.$contract_id.'/'.$id.'" class="btn btn-primary btn-xs " title="Edit Contract">Edit Contract</a>';
				echo '&nbsp;&nbsp;';
				echo '<a href="'.return_url().'optic-fibre-customers/terminate-contract/'.$contract_id.'/'.$id.'" class="btn btn-warning btn-xs " title="Terminate">Terminate</a>';
				echo '&nbsp;&nbsp;';
				echo '<a href="'.return_url().'optic-fibre-customers/delete-contract/'.$contract_id.'/'.$id.'" class="btn btn-danger btn-xs " title="Delete Contract">Delete</a>';
				echo '&nbsp;&nbsp;';
			}else{
				echo "<span class='text-danger'><b> Termination Date: ".Feedback::date_fm($terminated)."</b></span>";
			}
			echo '</td>';
			$co = $no;
			?>
			<div id="myModal<?php echo $co; ?>" class="net-advance-form modal">				
					 	<div class="modal-content"  style="width:600px">
						    	<span id="close<?php echo $co; ?>" class="close" style="color:red;">&times;</span>
							<h3 style="text-align:center;text-transform:uppercase;">Add Optic Fibre Service</h3>
					 		<form action="" method="post">
								<?php
								$effective_date = date('Y-m-d');
								if(isset($_POST['submit123'.$co])){
									$qty = $_POST['qty'];
									$effective_date = $_POST['effective_date'];
									$rate = $_POST['rate'];
									$description = trim($_POST['description']);
									$outlet = trim($_POST['outlet']);
									$desc = trim($_POST['desc']);
									$cores = $_POST['cores']; 

									$time = time();
									$user = user_id();
									$db = new Db();
									
									$errors = array();

									if(empty($qty)){
										$errors[]="Select Quantity";
									}
									if(empty($effective_date)){
										$errors[]="Select Effective Date";
									}
									if(empty($rate)){
										$errors[]="Enter Rate";
									}
									if(empty($description)){
										$errors[]="Enter Description";
									}
									if(0){
										$errors[]="Enter Description";
									}
									if(empty($outlet)){
										$errors[]="Enter Outlet";
									}
									// if(empty($cores)){
									// 	$errors[]="Enter Cores";
									// }
									//if($this->isThere("fibre_service", ["fs_description"=>$description])){
									if(0){
										$errors[] = "Service Description already exists";
									}
									$effective_date1 = strtotime($effective_date);
									if(empty($errors)){
										$db = new Db();
										$x = $db->insert("fibre_service",[
										"fs_customer_id"=>$id, 
										"fs_date_added"=>time(), 
										"fs_added_by"=>user_id(), 
										"fs_rate"=>str_replace(',','',$rate), 
										"fs_qty"=>$qty, 
										"fs_description"=>$description, 
										"fs_desc"=>$desc, 
										"fs_outlet"=>$outlet,
										"fs_cores"=>$cores, 
										"fs_contract"=>$contract_id, 
										"fs_effective_date"=>$effective_date1]);
														
										if(!$db->error()){
											FeedBack::success();
											FeedBack::redirect(return_url().'optic-fibre-customers/optic-fibre-customer-details/'.$id);
										}else{
											FeedBack::error();
										}
									}else{
										FeedBack::errors($errors);
									}
								}
								?>
								<div class="">
									<div class="">
										
											<div id="must">All field with asterisk(*) are mandatory.</div>
											
											<div class="row">
												<div class="col-lg-12">
													<form role="form" action="" method="post">
														<div class="row">
															<div class="col-md-6">
																<div class="form-group">
																	<label>Select Quantity<span class="must">*</span></label>
																	<div class="form-line">
																		<?php
																		if($isThere == 0){
																		 echo '<select name="qty" class="form-control">';
																			for($i=1; $i<=24; $i++){
																				$s = ($i>=2)?'s':'';
																				if($fs_qty == $i){
																					echo '<option selected="select" value="'.$i.'"> '.$i.' Month'.$s.'</option>';
																				}else{
																					echo '<option value="'.$i.'"> '.$i.' Month'.$s.'</option>';
																				}
																			}
																		echo '</select>';
																		}else{
																			echo '<select name="qty" disabled class="form-control">';
																				for($i=1; $i<24; $i++){
																					$s = ($i>=2)?'s':'';
																					if($fs_qty == $i){
																						echo '<option value="'.$i.'"> '.$i.' Month'.$s.'</option>';
																					}
																				}
																			echo '</select>';

																			echo '<input type="hidden" value="'.$fs_qty.'" name="qty"/>';
																		}
																		?>
																	</div>
																</div>
															</div>
															<div class="col-md-6">
																<div class="form-group">
																	<label>Effective Dates<span class="must">*</span></label>
																	<div class="form-line">
																		<?php
																		if(1){
																			$ef=date('Y-m-d');
																			echo '<input class="form-control" type="date" name="effective_date" placeholder="" value="'.$ef.'">';
																		}else{
																			$ef=date('Y-m-d', $fs_effective_date);
																			echo '<input class="form-control" type="date" name="effective_date" disabled placeholder="" value="'.$ef.'">';

																			echo '<input type="hidden" value="'.$ef.'" name="effective_date"/>';
																		}
																		 ?>
																	</div>
																</div>
															</div>
															<div class="col-md-6">
																<div class="form-group">
																	<label>Cores <span class="must">*</span></label>
																	<div class="form-line">
																		<input class="form-control number" type="text" name="cores" placeholder="" value="<?php echo @$fs_cores; ?>">
																	</div>
																</div>
															</div>
															<div class="col-md-6">
																<div class="form-group">
																	<label>Rate per core(USD)<span class="must">*</span></label>
																	<div class="form-line">
																		<input class="form-control number" type="text" name="rate" placeholder="" value="<?php echo ($fs_rate); ?>">
																	</div>
																</div>
															</div>
															<div class="clearfix"></div>
															<div class="col-lg-6">
																<div class="form-group">
																	<label>Select Service<span class="must">*</span></label>
																	<div class="form-line">
																		<select name="description" class="form-control">
																			<?php
																				$db = new Db();
																				$select = $db->select("SELECT * FROM services_fibre");
																				foreach($select[0] as $row){
																					extract($row);
																					if($service_id == $fs_description)
																					echo '<option value="'.$service_id.'" selected="selected">'.$service_name.'</option>';
																					else
																					echo '<option value="'.$service_id.'">'.$service_name.'</option>';
																				}
																			?>
																		</select>
																	</div>
																</div>
															</div>
															<div class="col-lg-6">
																<div class="form-group">
																	<label>Select Outlet<span class="must">*</span></label>
																	<div class="form-line">
																		<select name="outlet" class="form-control">
																			<?php
																				$db = new Db();
																				$select = $db->select("SELECT * FROM outlet");
																				foreach($select[0] as $row){
																					extract($row);
																					if($outlet_id == $fs_outlet)
																					echo '<option value="'.$outlet_id.'" selected="selected">'.$outlet_name.'</option>';
																					else
																					echo '<option value="'.$outlet_id.'">'.$outlet_name.'</option>';
																				}
																			?>
																		</select>
																	</div>
																</div>
															</div>
															<div class="col-lg-12">
																<div class="form-group">
																	<label>Enter Description<span class="must">*</span></label>
																	<div class="form-line">
																		<textarea class="form-control" name="desc"><?php echo $fs_desc; ?></textarea>
																		
																	</div>
																</div>
															</div>
															<div class="col-lg-12">
																<button onclick = "" type="submit" name="submit123<?php echo $co; ?>" class="btn btn-primary"><i class="fa fa-fw fa-save"></i> Save</button>
															</div>
														</div>
													</form>
												</div>
											</div>
					    	</form>
						</div>

					</div>
			<?php
			echo '</tr>';
			?>
			<script>
					document.getElementById("<?php echo 'add-service'.$co; ?>").onclick = function() {
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
		echo '</table>';

		echo '<br/><br/>';
		echo '<table id="table" border="1">';

		echo '<tr>';
		echo '<th width="30px">No</th>';
		echo '<th>Contract Date</th>';
		echo '<th>Effective Date</th>';
		echo '<th>Outlet</th>';
		echo '<th>Description</th>';
		echo '<th>Qty (Months)</th>';
		echo '<th>Cores</th>';
		echo '<th>Rate Per Core</th>';
		echo '<th>Action</th>';
		echo '</tr>';
		$no = 1;
		$n = 200;
		$db = new Db();
		
		$select = $db->select("SELECT * FROM fibre_service WHERE fs_customer_id = '$id'");
		foreach($select[0] as $row){
			extract($row);

			$terminated = $this->isTerminated($fs_contract);
			if($terminated){
				echo '<tr valign="top" style="color:red;">';
			}else{
				echo '<tr valign="top">';
			}

			echo '<td>'.($no++).'.</td>';
			$fss = $this->rgf("contract", $fs_contract, "contract_id", "contract_start_date");
			$fse = $this->rgf("contract", $fs_contract, "contract_id", "contract_end_date");
			echo '<td>'.FeedBack::date_s($fss).' - '.FeedBack::date_s($fse).'</td>';
			echo '<td>'.FeedBack::date_s($fs_effective_date).'</td>';
			echo '<td>'.$this->rgf("outlet", $fs_outlet, "outlet_id", "outlet_name").'</td>';
			echo '<td>'.nl2br($this->rgf("services_fibre",$fs_description,"service_id","service_name")).'<br/>'.$fs_desc.'</td>';
			echo '<td>'.($fs_qty).'</td>';
			echo '<td>'.($fs_cores).'</td>';
			echo '<td>'.number_format($fs_rate,2).'</td>';
			echo '<td>'; 
			
			if(!$terminated){
				echo '<a href="'.return_url().'optic-fibre-customers/delete-customer-service/'.$id.'/'.$fs_id.'" class="btn btn-danger btn-xs">Delete</a>&nbsp;&nbsp;<a href="#" class="btn btn-success btn-xs " id="add-hint'.$fs_id.'">Edit</a>';
			}
			
			
			echo  '</td>';
			$cok = $fs_id;
			?>
			<div id="myModal<?php echo $cok; ?>" class="net-advance-form modal">				
					 	<div class="modal-content"  style="width:600px">
						    	<span id="close<?php echo $cok; ?>" class="close" style="color:red;">&times;</span>
							<h3 style="text-align:center;text-transform:uppercase;">EDIT Optic Fibre Service</h3>
					 		<form action="" method="post">
								<?php
								if(isset($_POST['submit124'.$cok])){
									$qty = $_POST['qty'];
									$effective_date = $_POST['effective_date'];
									$rate = $_POST['rate'];
									$description = trim($_POST['description']);
									$outlet = trim($_POST['outlet']);
									$desc = trim($_POST['desc']);
									$cores = $_POST['cores']; 

									$time = time();
									$user = user_id();
									$db = new Db();
									
									$errors = array();

									if(empty($qty)){
										$errors[]="Select Quantity";
									}
									if(empty($effective_date)){
										$errors[]="Select Effective Date";
									}
									if(empty($rate)){
										$errors[]="Enter Rate";
									}
									if(empty($description)){
										$errors[]="Enter Description";
									}
									if(0){
										$errors[]="Enter Description";
									}
									if(empty($outlet)){
										$errors[]="Enter Outlet";
									}
									// if(empty($cores)){
									// 	$errors[]="Enter Cores";
									// }
									//if($this->isThere("fibre_service", ["fs_description"=>$description])){
									if(0){
										$errors[] = "Service Description already exists";
									}
									$effective_date1 = strtotime($effective_date);
									if(empty($errors)){
										$db = new Db();
										$x = $db->update("fibre_service",[
										"fs_customer_id"=>$id, 
										"fs_date_added"=>time(), 
										"fs_added_by"=>user_id(), 
										"fs_rate"=>str_replace(',','',$rate), 
										"fs_qty"=>$qty, 
										"fs_description"=>$description, 
										"fs_desc"=>$desc, 
										"fs_outlet"=>$outlet,
										"fs_cores"=>$cores, 
										"fs_effective_date"=>$effective_date1], ["fs_id"=>$fs_id]);
														
										if(!$db->error()){
											FeedBack::success();
											FeedBack::refresh(3, return_url().'optic-fibre-customers/optic-fibre-customer-details/'.$id);
										}else{
											FeedBack::error();
										}
									}else{
										FeedBack::errors($errors);
									}
								}
								?>
								<div class="">
									<div class="">
										
											<div id="must">All field with asterisk(*) are mandatory.</div>
											
											<div class="row">
												<div class="col-lg-12">
													<form role="form" action="" method="post">
														<div class="row">
															<div class="col-md-6">
																<div class="form-group">
																	<label>Select Quantity<span class="must">*</span></label>
																	<div class="form-line">
																		<?php
																		if($isThere == 0){
																		 echo '<select name="qty" class="form-control">';
																			for($i=1; $i<=24; $i++){
																				$s = ($i>=2)?'s':'';
																				if($fs_qty == $i){
																					echo '<option selected="select" value="'.$i.'"> '.$i.' Month'.$s.'</option>';
																				}else{
																					echo '<option value="'.$i.'"> '.$i.' Month'.$s.'</option>';
																				}
																			}
																		echo '</select>';
																		}else{
																			echo '<select name="qty" disabled class="form-control">';
																				for($i=1; $i<24; $i++){
																					$s = ($i>=2)?'s':'';
																					if($fs_qty == $i){
																						echo '<option value="'.$i.'"> '.$i.' Month'.$s.'</option>';
																					}
																				}
																			echo '</select>';

																			echo '<input type="hidden" value="'.$fs_qty.'" name="qty"/>';
																		}
																		?>
																	</div>
																</div>
															</div>
															<div class="col-md-6">
																<div class="form-group">
																	<label>Effective Date<span class="must">*</span></label>
																	<div class="form-line">
																		<?php
																		if($isThere == 0){
																			$ef=date('Y-m-d', $fs_effective_date);
																			echo '<input class="form-control" type="date" name="effective_date" placeholder="" value="'.$ef.'">';
																		}else{
																			$ef=date('Y-m-d', $fs_effective_date);
																			echo '<input class="form-control" type="date" name="effective_date" disabled placeholder="" value="'.$ef.'">';

																			echo '<input type="hidden" value="'.$ef.'" name="effective_date"/>';
																		}
																		 ?>
																	</div>
																</div>
															</div>
															<div class="col-md-6">
																<div class="form-group">
																	<label>Cores <span class="must">*</span></label>
																	<div class="form-line">
																		<input class="form-control number" type="text" name="cores" placeholder="" value="<?php echo @$fs_cores; ?>">
																	</div>
																</div>
															</div>
															<div class="col-md-6">
																<div class="form-group">
																	<label>Rate per core(USD)<span class="must">*</span></label>
																	<div class="form-line">
																		<input class="form-control number" type="text" name="rate" placeholder="" value="<?php echo ($fs_rate); ?>">
																	</div>
																</div>
															</div>
															<div class="clearfix"></div>
															<div class="col-lg-6">
																<div class="form-group">
																	<label>Select Service<span class="must">*</span></label>
																	<div class="form-line">
																		<select name="description" class="form-control">
																			<?php
																				$db = new Db();
																				$select = $db->select("SELECT * FROM services_fibre");
																				foreach($select[0] as $row){
																					extract($row);
																					if($service_id == $fs_description)
																					echo '<option value="'.$service_id.'" selected="selected">'.$service_name.'</option>';
																					else
																					echo '<option value="'.$service_id.'">'.$service_name.'</option>';
																				}
																			?>
																		</select>
																	</div>
																</div>
															</div>
															<div class="col-lg-6">
																<div class="form-group">
																	<label>Select Outlet<span class="must">*</span></label>
																	<div class="form-line">
																		<select name="outlet" class="form-control">
																			<?php
																				$db = new Db();
																				$select = $db->select("SELECT * FROM outlet");
																				foreach($select[0] as $row){
																					extract($row);
																					if($outlet_id == $fs_outlet)
																					echo '<option value="'.$outlet_id.'" selected="selected">'.$outlet_name.'</option>';
																					else
																					echo '<option value="'.$outlet_id.'">'.$outlet_name.'</option>';
																				}
																			?>
																		</select>
																	</div>
																</div>
															</div>
															<div class="col-lg-12">
																<div class="form-group">
																	<label>Enter Description<span class="must">*</span></label>
																	<div class="form-line">
																		<textarea class="form-control" name="desc"><?php echo $fs_desc; ?></textarea>
																		
																	</div>
																</div>
															</div>
															<div class="col-lg-12">
																<input type="hidden" name="" value="<?php echo $fs_id;?>">
																<button onclick = "" type="submit" name="submit124<?php echo $cok; ?>" class="btn btn-primary"><i class="fa fa-fw fa-save"></i> Save</button>
															</div>
														</div>
													</form>
												</div>
											</div>
					    	</form>
						</div>

					</div>
			<?php
			echo '</tr>';
			?>
			<script>
					document.getElementById("<?php echo 'add-hint'.$cok; ?>").onclick = function() {
					document.getElementById("<?php echo 'myModal'.$cok; ?>").style.display = "block";
					}
					document.getElementById("<?php echo 'close'.$cok; ?>").onclick = function() {
					document.getElementById("<?php echo 'myModal'.$cok; ?>").style.display = "none";
					}
					window.onclick = function(event) {
					if (event.target == document.getElementById("<?php echo 'myModal'.$co; ?>")) {
					document.getElementById("<?php echo 'myModal'.$cok; ?>").style.display = "none";
					}
					}
					</script>
					<?php
		}

		echo '</table>';


		
	}

	
	public function opticFibreInvoicesAction(){


		//$id = portion(3);
		$id = ($this->set_customer)?$this->set_customer:portion(3);


		$db = new Db();
		$select = $db->select("SELECT * FROM of_customer WHERE ofc_id = '$id'");
		extract($select[0][0]);
		$efd=$fs_effective_date;

		echo '<style>#y tr td{padding:10px;}</style>';
		echo '<table id="y">';

		echo '<tr valign="top">';
		echo '<td style="width:200px;"><h4 class="text-primary"><b>Customer Name:</b></h4></td>';
		echo '<td><h4 class="text-primary">'.$ofc_customer_name.' ('.$ofc_short_name.')'.'</h4></td>';
		echo '</tr>';
				
		echo '</table>';

		echo '<a class="right" href="'.return_url().'optic-fibre-customers/optic-fibre-customer-details/'.$id.'"><button class="btn btn-primary">Customer Details, Contracts & Services</button></a>';
		
		echo '<br/>';
		
		

		$all_dates = array();
		$start_dates = array();
		$sel = $db->select("SELECT contract_id FROM contract WHERE contract_ofc_id = '$ofc_id'");
		
		if($db->num_rows()){
			foreach($sel[0] as $row){
				extract($row);
				$days = (40*24*60*60);//+days
				$time_now = (time()+$days);//+days
				$terminated = ($this->isTerminated($contract_id))?$this->isTerminated($contract_id):(time()+$days);
				//echo '>>> '.$contract_id.' '.Feedback::date_fm($terminated).'<br/>';
				
				$sql = "SELECT TOP 1 * FROM fibre_service WHERE fs_contract = '$contract_id'  ORDER BY fs_effective_date ASC";
				$select = $db->select($sql);
				if($db->num_rows()){
					$n=0;
					foreach($select[0] as $row){
						extract($row);
						$x = array();
						$fsed = $fs_effective_date;
						while($fsed <= $time_now ){
							$x = $this->invoiceCreator($fsed, $fs_qty);							
							if($fsed < $terminated){
								if(strtotime($x['start_date']) >= $this->startNotification() ){
									$all_dates[$contract_id][$n] = $x['start_date'].' - '.$x['end_date'];
									$start_dates[$contract_id][$n++] = $x['start_date'];
								}
							}
							$fsed = strtotime($x['next_start']);
						}
					}
				}
			}
		}

		
		// echo '<pre>';
		// print_r($start_dates);
		// echo '</pre>';
		// echo count($all_dates);
		// echo '<br/>';
			
            $total2 = 0;
			$vat2 = 0;
			$total_vat2 = 0;
			$total = 0;
			$vat = 0;
			$total_vat = 0;
		foreach($all_dates as $contract => $services){
			
			$sd = $this->notified($id, $contract);

			echo '<h5>Contract: '.Feedback::date_s($this->rgf("contract", $contract, "contract_id", "contract_start_date")).' - '.Feedback::date_s($this->rgf("contract", $contract, "contract_id", "contract_end_date"));

			$terminated = $this->isTerminated($contract);
			if($terminated){
				echo ' <span class="bg-danger"> &nbsp;&nbsp; Termination Date: '.Feedback::date_fm($terminated).'</span>';
			}else{
				//echo ' <a href="'.return_url().'optic-fibre-customers/terminate-contract/'.$contract_id.'/'.$id.'/T" class="btn btn-warning btn-xs " title="Terminate">Terminate</a>';
			}

			echo '</h5>';
			$mcv = $this->rgf("fibre_service", $contract, "fs_contract", "fs_description");
			if($contract){
				if($mcv=="Annual Co.Location" || $mcv == 4){
					// $total = 0;
					// $vat = $total_vat = 0;
					//echo '<h5>Annual Co.Location Contract: '.Feedback::date_s($this->rgf("contract", $contract, "contract_id", "contract_start_date")).' - '.Feedback::date_s($this->rgf("contract", $contract, "contract_id", "contract_end_date")).'</h5>';


					echo '<table border="1" id="table">';
					echo '<tr>';
					echo '<th width="30px">No.</th>';
					echo '<th>Effective Date</th>';
					echo '<th>Qty</th>';
					echo '<th>Units</th>';
					echo '<th>Rate Per Month</th>';
					echo '<th>Total (VAT Ex)</th>';
					echo '<th>VAT</th>';
					echo '<th>Total (VAT In)</th>';
					echo '<th>Action</th>'; 
					echo '</tr>';
					        
						for($i=0; $i<count($services); $i++){
							$select = $db->select("SELECT * FROM fibre_service WHERE fs_contract = '$contract'");
							$tot = $db->num_rows();

							//echo $start_dates[$i].'<br/>';

							$k=0;

							foreach($select[0] as $row){
								extract($row);
								if(1){//POPOPOP
									//greater than
									if(!in_array(strtotime($start_dates[$contract][$i]), $sd) && strtotime($start_dates[$contract][$i]) > $this->startNotification()){
										echo '<tr valign="top" style="background-color:#ecc2c2;">';
									}else{
										echo '<tr valign="top">';
									}
									$k++;
									if($k==1){
										echo '<td rowspan="'.$tot.'">'.($i+1).'.</td>';
										echo '<td rowspan="'.$tot.'">'.$services[$i].'</td>';
										echo '<td rowspan="'.$tot.'">'.$fs_qty.'</td>';
										echo '<td rowspan="'.$tot.'">Months</td>';
									}
									
									
									//echo '<td align="right">'.number_format($fs_cores).'</td>';
									$fs_cores = 1;
									echo '<td align="right">'.number_format($fs_rate).'</td>';
									echo '<td align="right">'.number_format($fs_rate*$fs_qty*$fs_cores).' $</td>';
									$total += $fs_rate*$fs_qty;
									echo '<td align="right">'.number_format($fs_rate*$fs_cores*$fs_qty*18/100).' $</td>';
									$vat += $fs_rate*$fs_qty*18/100;
									echo '<td align="right">'.number_format($fs_rate*$fs_cores*$fs_qty*1.18).' $</td>';
									$total_vat += $fs_rate*$fs_qty*$fs_cores+$fs_rate*$fs_cores*$fs_qty*18/100;

									if($k==1){
										echo '<td rowspan="'.$tot.'"> &nbsp; <a href="'.return_url().'optic-fibre-customers/invoice/'.$id.'/'.strtotime($start_dates[$contract][$i]).'/'.$contract.'">Invoice</a></td>';
									}
									
									echo '</tr>';
								}
							}
						}


					echo '<tr>';
					echo '<td colspan="5" align="right" style="font-weight: bold;">Total: </td>';
					echo '<td align="right" style="font-weight: bold;">'.number_format($total).' $</td>';
					echo '<td align="right" style="font-weight: bold;">'.number_format($vat).' $</td>';
					echo '<td align="right" style="font-weight: bold;">'.number_format($total_vat).' $</td>';
					echo '<td>&nbsp;</td>';
					echo '</tr>';
					echo '</table>';
				}else{
					//{
						//echo '<h5>Contract: '.Feedback::date_s($this->rgf("contract", $contract, "contract_id", "contract_start_date")).' - '.Feedback::date_s($this->rgf("contract", $contract, "contract_id", "contract_end_date")).'</h5>';
						echo '<table border="1" id="table">';
						echo '<tr>';
						echo '<th width="30px">No.</th>';
						echo '<th>Effective Date</th>';
						echo '<th>Qty</th>';
						echo '<th>Units</th>';
						echo '<th>Cores</th>';
						echo '<th>Rate Per Core</th>';
						echo '<th>Total (VAT Ex)</th>';
						echo '<th>VAT</th>';
						echo '<th>Total (VAT In)</th>';
						echo '<th>Action</th>';
						echo '</tr>';
						     
						for($i=0; $i<count($services); $i++){
							$select = $db->select("SELECT * FROM fibre_service WHERE fs_contract = '$contract'");
							$tot = $db->num_rows();

							//echo $start_dates[$i].'<br/>';

							$k=0;
							
							foreach($select[0] as $row){
								extract($row);
								//greater than
								if(!in_array(strtotime($start_dates[$contract][$i]), $sd) && strtotime($start_dates[$contract][$i]) > $this->startNotification()){
									echo '<tr valign="top" style="background-color:#ecc2c2;">';
								}else{
									echo '<tr valign="top">';
								}
								$k++;
								if($k==1){
									echo '<td rowspan="'.$tot.'">'.($i+1).'.</td>';
									echo '<td rowspan="'.$tot.'">'.$services[$i].'</td>';
									echo '<td rowspan="'.$tot.'">'.$fs_qty.'</td>';
									echo '<td rowspan="'.$tot.'">Months</td>';
								}
								
								
								echo '<td align="right">'.number_format($fs_cores).'</td>';
								echo '<td align="right">'.number_format($fs_rate,2).'</td>';
								echo '<td align="right">'.number_format($fs_rate*$fs_qty*$fs_cores,2).' $</td>';
								$total2 += $fs_rate*$fs_qty;
						      if($ofc_cust_type =='B2F'){
								echo '<td align="right"> -</td>';
								$vat2 +=0;
							   }else{
							   	echo '<td align="right">'.number_format($fs_rate*$fs_cores*$fs_qty*18/100,2).' $</td>';
							   	$vat2 += $fs_rate*$fs_qty*18/100;
							   }
								
								 if($ofc_cust_type =='B2F'){
								   echo '<td align="right">'.number_format($fs_rate*$fs_cores*$fs_qty,2).' $</td>';
								   $total_vat2 += $fs_rate*$fs_qty*$fs_cores+$fs_rate*$fs_cores*$fs_qty;
							    }else{
							    	echo '<td align="right">'.number_format($fs_rate*$fs_cores*$fs_qty*1.18,2).' $</td>';
							    	$total_vat2 += $fs_rate*$fs_qty*$fs_cores+$fs_rate*$fs_cores*$fs_qty*18/100;
							    }
								

								if($k==1){
									echo '<td rowspan="'.$tot.'"> &nbsp; <a href="'.return_url().'optic-fibre-customers/invoice/'.$id.'/'.strtotime($start_dates[$contract][$i]).'/'.$contract.'">Invoice</a></td>';
								}
								
								echo '</tr>';
							}
						}

						echo '<tr>';
						echo '<td colspan="6" align="right" style="font-weight: bold;">Total: </td>';
						echo '<td align="right" style="font-weight: bold;">'.number_format($total2,2).' $</td>';
						echo '<td align="right" style="font-weight: bold;">'.number_format($vat2,2).' $</td>';
						echo '<td align="right" style="font-weight: bold;">'.number_format($total_vat2,2).' $</td>';
						echo '<td>&nbsp;</td>';
						echo '</tr>';
						echo '</table>';
					//}
				}
			}
		}
	}

	public function deleteCustomerService(){
		$id = portion(3);
		$id2 = portion(4);
		$sql = "DELETE from fibre_service WHERE fs_id = '$id2'";
		$db = new Db();
		$db->query($sql);
		$this->deletor("fibre_service", "fs_id",  $id2, 'optic-fibre-customers/optic-fibre-customer-details/'.$id);
	}

	public function invoiceAction(){
		$id = portion(3);
		$contract = portion(5);
		$period = $sdate = portion(4);
		$db = new Db();
		if(isset($_POST['show'])){
			$customer = $_POST['customer'];
			$sel = $db->select("SELECT TOP 1 not_contract_id, not_start_date FROM notification WHERE not_customer_id = '$customer' ORDER BY not_id DESC");
			extract($sel[0][0]);

			Feedback::redirect(return_url().'optic-fibre-customers/invoice/'.$customer.'/'.$not_start_date.'/'.$not_contract_id);
		}

		echo '<form class="search" action="" method="post" style="display:block;">';
		echo '<div class="pull-left"> Select Customer: &nbsp; </div>';
		echo '<div class="pull-left">';
		echo '<select name="customer">';
		$select = $db->select("SELECT * FROM of_customer");
		foreach($select[0] as $row){
			extract($row);
			if($id == $ofc_id)
				echo '<option selected="selected" value="'.$ofc_id.'">'.$ofc_customer_name.'</option>';
			else
				echo '<option value="'.$ofc_id.'">'.$ofc_customer_name.'</option>';
		}
		echo '</select>';
		echo '</div>';
		echo '<div class="pull-left"> &nbsp; &nbsp; ';
		echo '<button name="show" class="btn btn-primary btn-xs"><i class="fa fa-fw fa-eye"></i> Show</button>';
		echo '</div>';
		echo '<div class="clearfix"></div>';
		echo '</form>';

		$select = $db->select("SELECT * FROM of_customer WHERE ofc_id = '$id'");

		if(empty($contract)){
			$select = $db->select("SELECT contract_id as contract FROM contract WHERE contract_ofc_id = '$id'");
			extract($select[0][0]);

		}
		
		if($db->num_rows())
		{
			extract($select[0][0]);	

			$sd = $this->notified($id, $contract);

			$sel = $db->select("SELECT * FROM fibre_service WHERE fs_customer_id = '$id' AND fs_contract = '$contract'");
			extract($sel[0][0]);

			$x = $this->invoiceCreator($period, $fs_qty);

			$end_date = strtotime($x['end_date']);

			if($contract!=""){
				if(!in_array($period, $sd)){
					$db = new Db();
					$exist = $db->select("SELECT * FROM notification WHERE not_start_date = '$period' AND not_contract_id = '$contract' AND not_customer_id='$id'");
					if(!$db->num_rows()){
			         extract($sel[0][0]);
					$insert = $db->insert("notification", [
						"not_start_date"=>$period,
						"not_end_date"=>$end_date,
						"not_customer_id"=>$id,
						"not_date_added"=>time(),
						"not_added_by"=>user_id(),
						"not_contract_id"=>$contract,
					]);
				}
				}
			}
		}
		
		
		
		if(empty($period)){
			echo "<center><br/><br/><h3>No Invoice</h3></center>";
		}else{
		$select = $db->select("SELECT not_id, not_date_added, not_selected1,not_contract_id FROM notification WHERE not_customer_id = '$id'  AND not_start_date = '$period' AND not_contract_id='$contract'");


		extract($select[0][0]);	
		//print_r($select);	
		$ni = $not_id;
		echo '<div class="clearfix"></div>';
		//$b = 2019;
		$db = new Db();
		$select = $db->select("SELECT * FROM notification WHERE not_customer_id = '$id' ORDER BY not_id ASC");
		//echo '<b>Invoice No.: </b>';
		foreach($select[0] as $row){
			extract($row);
			if($not_start_date == $period && $contract == $not_contract_id){
				$gh = "<br/><span style='color:#777;'>".month_name_short(date('m',$not_start_date))." '".date('y',$not_start_date).'</span>';
				echo '<span style="margin-bottom:10px;" class="btn btn-default btn-xs">'.$not_id.$gh.'</span>&nbsp;';
			}else{
				$gh = "<br/><span style='color:#000;'>".month_name_short(date('m',$not_start_date))." '".date('y',$not_start_date).'</span>';
				echo '<a style="margin-bottom:10px;" href="'.return_url().'optic-fibre-customers/invoice/'.$id.'/'.$not_start_date.'/'.$not_contract_id.'" class="btn btn-xs btn-primary">'.$not_id.$gh.'</a>&nbsp;';
			}
		}
		echo '<div class="clearfix" style="margin-bottom:10px;"></div>';
		
		//if($not_app1==1 && $not_app2==1){
		if(1){
			echo '<div class="nav files pull-right">';
			echo '<button class="btn btn-success btn-xs" onclick="printJS({
  printable: \'printMe\', 
  type: \'html\', 
  style: \'table { width: 100%; }\',
  maxWidth: \'100%\',
  targetStyles: [\'box\', \'break\', \'text-decoration\'],
  scanStyles: false,
});"><i class="fa fa-print fa-fw"></i> Print Invoice</button>';
			echo '</div>';
			
			
		}
		echo '<div class="clearfix" style="margin-bottom:10px;"></div>';
		//echo '<br/><br/>';
		?>
		<style>
		table tr th{ background-color:#ccc; color:black; }
		.hide{display:none;}
		.table tr:hover{
			background-color:none;
			background:none !important;
		}
		#y>tbody>tr>td{ padding:5px; background-color:#fbfbfb; border:1px solid #000; font-family: arial; }
		</style>
		<div id="printMe" style="width:900px;margin:auto;">
			<table border="0" cellspacing="0" cellpadding="2" id="y" style="font-family: arial;">
				<tbody>
				<tr valign="bottom">
					<td colspan="11">
					<img src="<?php display_url();?>images/uetcl_header.png" width="100%">
					<center><h2 style="color:blue;">TAX INVOICE</h2></center>
						<!-- <hr style="border:1px solid black;"/> -->
						<?php
						//echo "@@@".date('y-m',$period);
						$db = new Db();
						$select = $db->select("SELECT * FROM of_customer WHERE ofc_id = '$id'");
						extract($select[0][0]);
						?>
						
						<?php
							//echo "==".$ni;
						 $invoice_no = $ofc_short_name.'/'.date('y-m',$period).'/'.str_pad($ni, 3, '0', STR_PAD_LEFT);?>
						
						<p align="right" style="font-weight: bold; color:red;">Tax Invoice No.: <?php echo$invoice_no;

						 ?></p>
						<?php

						
						echo '<table border="0" style="width:100%;">';
						$oi_invoice_date = $this->rgf("other_invoice",$invoice_no,"oi_ref_no","oi_invoice_date");
						
						echo '<tr valign="middle">';
					if($oi_invoice_date){		
		         	    echo '<tr valign="top">';
						echo '<td>Invoice Date: </td>';
						echo '<td>'.FeedBack::date_s($oi_invoice_date).''.'</p></td>';
						echo '</tr>';
					  }
						$postdate = $this->rgf("other_invoice",$invoice_no,"oi_ref_no","oi_date_added");
						
						//echo '<td rowspan="4" align="right">';
					   
							
						$g = new Db();
						$gg = $g->select("SELECT * FROM contract WHERE contract_ofc_id = '$id'");
						if($g->num_rows()){
							extract($gg[0][0]);
							//echo '<div style="color:blue; padding:10px;">';
							//echo '<b>Contract Period</b><br/>';
							//echo Feedback::date_c($contract_start_date);
							//echo ' - ';
							//echo Feedback::date_c($contract_end_date);
							//echo '<br/>';

							//echo Feedback::status($contract_start_date, $contract_end_date, $id);
							//echo '</div>';
						}
						//echo '</td>';
						if($postdate){
						echo '<tr valign="top">';
						echo '<td>Post Date: </td>';
						echo '<td>'.FeedBack::date_s($postdate).''.'</p></td>';
						echo '</tr>';
						echo '</tr>';
					    }
						//echo ' &nbsp;';
						
						
						echo '<tr><td></td><td><br/></td></tr>';

						echo '<tr valign="top">';
						echo '<td>Customer: </td>';
						echo '<td><b>'.strtoupper($ofc_customer_name).'</b>'.'</p></td>';
						echo '</tr>';
						if($ofc_tin_no){
						echo '<tr valign="top">';
						echo '<td>Customer TIN: </td>';
						echo '<td><b>'.strtoupper($ofc_tin_no).'</b>'.'</p></td>';
						echo '</tr>';
					    }
						echo '<tr valign="top">';
						echo '<td>Customer Address: </td>';
						echo '<td>'.'<p style="font-size: 12px;line-height:12px;">';

						if(!empty($ofc_street)){
							echo $ofc_street.'<br/>';
							echo $ofc_postal_code.'<br/>';
							echo $ofc_tel.'<br/>';
							echo $ofc_email.'<br/>';
						}else{
							echo nl2br($ofc_address);
						}

						echo '</p></td>';
						echo '</tr>';

						echo '</table>';
						//echo "@@@@@@".$invoice_no;
						  if($ofc_cust_type =='B2F'){
								$vat = 0;
							}else{
								$vat = 0.18;
							}
                       echo'<table style="" class="" cellspacing="10">';
					     
					     $db = new Db();
					     $efris_info = $db->select("SELECT * FROM other_invoice where oi_ref_no='$invoice_no'");
					     if($db->num_rows()){
					     	extract($efris_info[0][0]);
					     	//print_r($efris_info);

					       $item_id = 3180;

							$buyer = $this->rgf("customer",$customer_id,"customer_id","customer_short_name");
							 $tin = $this->rgf("customer",$customer_id,"customer_id","customer_tin");
							 $qr = '<img style="text-align:center;width:130px;" src="'.return_url().'efris_qr_images/'.$oi_qr.'" alt="'.$oi_fdn.'"/>';
							 $vc = $oi_vc;
							 $fdn = $oi_fdn;

							echo '<tr valign="top" style="border-left:none;">';
							echo '<td colspan="5" style="border-left:none;border-right:none;">';
							echo '<br/><B>URA EFRIS DETAILS:</B><BR/>';
							echo '<table>';
							echo '<tr>';
							echo '<td>Fiscal Document Number:</td>';
							echo '<td>'.$fdn.'</td>';
							echo '</tr>';
							echo '<tr>';
							echo '<td>Verification Code:</td>';
							echo '<td>'.$vc.'</td>';
							echo '</tr>';
							echo '</table><br/>';
							echo '</td>';
							echo '<td colspan="2" class="pull-right" style="border-left:none;border-right:none;"><br/><center>'.$qr.'<br/></center></td>';
							echo '</tr>';
						    }
						  
							$db = new Db();
							$select1 = $db->select("SELECT * FROM fibre_service WHERE fs_customer_id = '$id' AND fs_contract = '$contract'");
							if($db->num_rows()){
								extract($select1[0][0]);
								$service_id = $this->rgf("efris_goods",$this->rgf("services_fibre",$fs_description,"service_id","service_item_id"),"eg_id","eg_id"); 
								//3175
							}
						echo'</table>';
						echo '<br/><style>#table td{padding:2px 10px;}</style>';
						
						if($fs_description == 'Annual Co.Location'){
							echo '<table border="1" id="table" cellspacing="0" style="width:100%;font-size: 12px; font-family: arial;">';
							echo '<tr>';
							echo '<th>Period</th>';
							echo '<th>Description of Service Rendered</td>';
							echo '<th>Rate Per Month</th>';
							//echo '<th>Rate Per Core</th>';
							echo '<th>Quantity<br/>(Months)</th>';
							echo '<th>Amount</th>';
							echo '</tr>';
							$db = new Db();
							$select = $db->select("SELECT * FROM fibre_service WHERE fs_customer_id = '$id' AND fs_contract = '$contract'");
							$period = FeedBack::date_r(portion(4));
							$total = 0;
							$i = 0;
							$all = $db->num_rows();
							foreach($select[0] as $row){
								extract($row);

								$x = $this->invoiceCreator($sdate, $fs_qty);
								$ef_da = "";
								$i++;

								if($i == $all){
									$ef_da = ($id == 1060)? '<div style="margin:4pt;"></div>':"<br/>";
									$ef_da .= "<b>For the Period: ".FeedBack::date_c(strtotime($x['start_date']))." -  ".FeedBack::date_c(strtotime($x['end_date'])).'</b>';
								}
								$rr = array();
								
								if(!empty($fs_description))
								$rr[] = '<b>'.$this->rgf("services_fibre",$fs_description,"service_id","service_name").'</b>';
								
								if(!empty($this->rgf("outlet", $fs_outlet, "outlet_id", "outlet_name")))
								$rr[] = $this->rgf("outlet", $fs_outlet, "outlet_id", "outlet_name");
								
								if(!empty($fs_desc))
								$rr[] = $fs_desc;
								
								if(!empty($ef_da))
								$rr[] = $ef_da;
								$rrr = implode("<br/>", $rr);
								echo '<tr style="';
								if($all == 1) 
									echo 'height: 180px';
								if($all == 2) 
									echo 'height: 80px';
								echo '" valign="top">';
								echo '<td>'.$period.'</td>';
								echo '<td width="30%">'.$this->rgf("services_fibre",$rrr,"service_id","service_name").'</td>';
								//echo '<td width="50px">CORE</td>';
								echo '<td align="right">'.number_format($fs_rate).' $</td>';
								echo '<td align="center">'.$fs_qty.'</td>';
								
								echo '<td align="right">'.number_format($fs_qty*$fs_rate).' $</td>';
								echo '</tr>';
								$total += $fs_qty*$fs_rate;
							}
							
							echo '<tr>';
							echo '<td colspan="4"><b>Total before VAT</b></td>';
							echo '<td align="right"><b>'.number_format($total).' $</b></td>';
							echo '</tr>';
							
							if($ofc_cust_type =='B2F'){
								echo '<tr>';
								echo '<td colspan="4"><b>VAT</b></td>';
								echo '<td align="right"><b>'.number_format($total*$vat).' $</b></td>';
								echo '</tr>';
							}else{
								echo '<tr>';
								echo '<td colspan="4"><b>VAT</b></td>';
								echo '<td align="right"><b>'.number_format($total*$vat).' $</b></td>';
								echo '</tr>';
							}

							echo '<tr>';
							echo '<td colspan="4"><b>Total Amount incl. VAT</b></td>';
							echo '<td align="right"><b>'.number_format($total*$vat).' $</b></td>';
							echo '</tr>';

							echo '</table>';
						}else{
							echo '<table border="1" id="table" cellspacing="0" style="width:100%;font-size: 12px; font-family: arial;">';
							echo '<tr>';
							echo '<th>Period</th>';
							echo '<th>Description of Service Rendered</td>';


							$db = new Db();
							$select = $db->select("SELECT fs_description as fsd FROM fibre_service WHERE fs_customer_id = '$id' AND fs_contract = '$contract' ");

							if($db->num_rows())
								extract($select[0][0]);

							if($fsd == 4){
							 $colspan = 4; 
							}
							elseif($service_id == 3175){
								$colspan = 3; 
							}
							else{
							 $colspan = 5;
							
							}



							if($fsd != '4' && $service_id != '3175'){
								echo '<th>No. of Cores</th>';
						       }

							if($fsd == 4 && $service_id != '3175'){
								echo '<th>Rate</th>';
							}else{
								if($service_id != 3175){
								echo '<th>Rate Per Core</th>';
							   }
						     }
							//echo '<th>Rate Per Core</th>';

							if($fsd == 4){
								echo '<th>Quantity</th>';
							}
							else{
								echo '<th>Quantity<br/>(Months)</th>';
							}

							echo '<th style="text-align:right;">Amount</th>';
							echo '</tr>';
							$db = new Db();
							$select = $db->select("SELECT * FROM fibre_service WHERE fs_customer_id = '$id' AND fs_contract = '$contract' ");
							$period = FeedBack::date_r(portion(4));
							$total = 0;
							$i = 0;
							$all = $db->num_rows();
							foreach($select[0] as $row){
								extract($row);

								$x = $this->invoiceCreator($sdate, $fs_qty);
								$ef_da = "";
								$i++;

								if($i == $all){
									$ef_da = ($id == 1060)? '<div style="margin:4pt;"></div>':"<br/>";
									$ef_da .= "<b>For the Period: ".FeedBack::date_c(strtotime($x['start_date']))." -  ".FeedBack::date_c(strtotime($x['end_date'])).'</b>';
								}
								$rr = array();
								
								if(!empty($fs_description))
								$rr[] = '<b>'.$this->rgf("services_fibre",$fs_description,"service_id","service_name").'</b>';
								
								if(!empty($this->rgf("outlet", $fs_outlet, "outlet_id", "outlet_name")))
								$rr[] = $this->rgf("outlet", $fs_outlet, "outlet_id", "outlet_name");
								
								if(!empty($fs_desc))
								$rr[] = $fs_desc;
								
								if(!empty($ef_da))
								$rr[] = $ef_da;
								$rrr = implode("<br/>", $rr);
								echo '<tr style="';
								if($all == 1) 
									echo 'height: 180px';
								if($all == 2) 
									echo 'height: 80px';
								echo '" valign="top">';
								echo '<td>'.$period.'</td>';
								echo '<td width="30%">'.$rrr.'</td>';
								//echo '<td width="50px">CORE</td>';

							if($fsd != 4)
								if($service_id != 3175)
								echo '<td align="center" width="50px">'.$fs_cores.'</td>';
								if($service_id != 3175)
								echo '<td align="right">'.number_format($fs_rate, 1).' $</td>';
								echo '<td align="center">'.$fs_qty.'</td>';
								if(portion(3) == 1058){
									$total += (int)($fs_qty*$fs_rate*$fs_cores);
								}else{
									$total += $fs_qty*$fs_rate*$fs_cores;								
								}
								echo '<td align="right">'.number_format($fs_qty*$fs_rate*$fs_cores).' $</td>';
								echo '</tr>';
								$total2 = $fs_qty*$fs_rate*$fs_cores;
							}

							echo '<tr>';

							echo '<td colspan="'.$colspan.'"><b>Total before VAT</b></td>';
							echo '<td align="right"><b>'.number_format($total2).' $</b></td>';
							echo '</tr>';

							if($ofc_cust_type =='B2F'){
								echo '<tr>';
								echo '<td colspan="'.$colspan.'"><b>VAT</b></td>';
								echo '<td align="right"><b>-</b></td>';
								echo '</tr>';

								echo '<tr>';
								echo '<td colspan="'.$colspan.'"><b>Total Amount incl. VAT</b></td>';
								echo '<td align="right"><b>'.number_format($total2).' $</b></td>';
								echo '</tr>';
							}else{
								echo '<tr>';
								echo '<td colspan="'.$colspan.'"><b>VAT</b></td>';
								echo '<td align="right"><b>'.number_format($total*$vat).' $</b></td>';
								echo '</tr>';

								echo '<tr>';
								echo '<td colspan="'.$colspan.'"><b>Total Amount incl. VAT</b></td>';
								echo '<td align="right"><b>'.number_format($total*$vat+$total).' $</b></td>';
								echo '</tr>';
							}

							echo '</table>';
						}
						if($ofc_cust_type =='B2F'){
							if($oi_exchange_rate){
							echo '<br/><span style="color:blue;"><b>Exchange Rate: </b>'.number_format($oi_exchange_rate,2).' $</span>';	
						    }
							echo '<br/><span style="color:blue;"><b>Total Amount: </b>'.number_format($total2).' $</span>';

							echo '<br/><span style="color:blue;"><b>Total Amount (in Words): </b>'.FeedBack::number_to_words((ceil)($total2)).' Dollars</span>';

					    }else{
					    	if($oi_exchange_rate){
							echo '<br/><span style="color:blue;"><b>Exchange Rate: </b>'.number_format($oi_exchange_rate,2).' $</span>';	
						    }
							echo '<br/><span style="color:blue;"><b>Total Amount:</b>'.number_format($total*$vat+$total).' $</span>';

							echo '<br/><span style="color:blue;"><b>Total Amount (in Words): </b>'.FeedBack::number_to_words((ceil)($total*$vat+$total)).' Dollars</span>';
					    }
						?>
						<br/><br/>
						
						<span style="color:red; font-weight: bold; font-size: 12px; text-align: center; ">Invoice is due within 30 days of receipt.
                          </span>				

						<br/><br/>
						<table style="width:100%; font-size: 12px !important; font-family: arial;">
							<tr>
								<td colspan="3">
									<?php $c =  $this->opticApprovals(user_id(), $ms, $id, 1, $ni, $sdate); ?>
								</td>
							</tr>
						</table>
						<!-- Prepared by: .........................  -->
						
					</td>				
				</tr>
				
			</tbody></table>
			<?php
			
			//$c =  $this->opticApprovals(user_id(), $ms, $id, 1, $ni, $sdate);
			//echo "@@@@".$c['tc'];
			if(!$fdn){
				if($c['tc'] == 3){
					echo '<span>Invoice Date</span><input type="date" id="date" value="'.date('Y-m-d',$c['invoice_date']).'"/><br><br>';
			echo '<button data-type="approvex" data-count="'.$tc.'" type="button" name="approve" id="pushInvoiceToEFRIS" class="btn btn-xs btn-success"><i class="fa fa-fw fa-plane"></i> Push To Efris</button>';
			echo '<span id="pushInvoiceToEFRISStatus"></span>';	
		    }
		    }
			?>
		</div>
	
	</div>
	<?php

	#################################PUSH TO Efris #######################################
		echo'<form method="post">';
		
		$item_id = 3180;
		$symbol = $this->rgf("currency",$customer_currency, "currency_id", "currency_symbol");
		$item = $this->rgf("services_fibre",$fs_description,"service_id","service_item_id");
		//$this->rgf("efris_goods",$item,"eg_id","eg_name")
		echo '<input type="hidden" id="items" value="'.$item.'"/>';
		
		echo '<input type="hidden" id="a_currency" value="$"/>';
		if($ofc_cust_type =='B2F'){
		 $customer_vat = 0;
	     }else{
	     	$customer_vat = 0.18;
	     }
		$customer_vat = ($customer_vat==0.18)?"RES":"EXP";
		//echo ">>>>>>>>>>>>".$customer_vat;
		echo '<input type="hidden" id="vat" value="'.$customer_vat.'"/>';
		echo '<input type="hidden" id="buyer" value="'.$id.'"/>';
		echo '<input type="hidden" id="qty" value="1"/>';
		//echo '<input type="hidden" id="a_date" value="'.('2024-04-30').'"/>';
		echo '<input type="hidden" id="a_date" value="'.date('Y-m-d', $c['invoice_date']).'"/>';
		echo '<input type="hidden" id="c_date" value="'.date('Y-m-d').'"/>';
		echo '<input type="hidden" id="customer_type" value="'.$ofc_cust_type.'"/>';
//1724878800
		//echo '<input type="hidden" id="date" value="'.$c['invoice_date'].'"/>';
		echo '<input type="hidden" id="ref" value="'.$invoice_no.'"/>';
		echo '<input type="hidden" id="id" value="'.$invoice_no.'"/>';
		if($ofc_cust_type =='B2F'){
		echo '<input type="hidden" id="total_vat" value="0"/>';
		echo '<input type="hidden" id="total_vat_ex" value="'.round($total2).'"/>';
		echo '<input type="hidden" id="unitPrice" value="'.number_format($total2,2).'"/>';
	    }else{
	      echo '<input type="hidden" id="total_vat" value="'.round($total*0.18).'"/>';
		  echo '<input type="hidden" id="total_vat_ex" value="'.round($total).'"/>';
		  echo '<input type="hidden" id="unitPrice" value="'.number_format($total*1.18,2).'"/>';
	    }

		echo '<input type="hidden" id="customer" value="'.$id.'"/>';
	    echo '<input type="hidden" id="exchange" value=""/>';  
//echo'<p id="ccc"></p>';

		// if(!$fdn){
		//$fix_it = "&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;";
		//echo$fix_it.'<button type="button" style="display:none;" id="pushInvoiceToEFRIS" name="send" class="btn btn-primary"><i class="fa fa-fw fa-plane"></i> Push Invoice</button>';
		
	 //    }
		?>
	</form>
	<?php
	//$push = 1; 
	//echo'>>>>>>>>>>'.$_SESSION["push"];?>
<script type="text/javascript">	
	$(document).ready(function(){  
		// id = $(this).val();
		//$('#exchange').val('checking...');
		var form_data = new FormData();
		form_data.append('id', $('#c_date').val());
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
				//alert(data);
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


		var urlPath = $('#urlPath').val();
		//var ready_to_push = '<?php echo$_SESSION["push"];?>';

		$(document).off('click','#pushInvoiceToEFRIS').on('click', '#pushInvoiceToEFRIS', function(e){
	     $('#approvalStatus').html('');
			var items = $('#items').val();

			var a_currency = $('#a_currency').val();
			var a_amount = $('#unitPrice').val();
			var a_date = $('#a_date').val();

			var buyer = $('#buyer').val();
			var id = $('#id').val();
			var currency = $('#currency').val();
			var vat = $('#vat').val();
			var date = $('#date').val();
			var item = $('#item').val();
			var unitPrice = $('#unitPrice').val();

			var type = $(this).attr('data-type');
        	var count = $(this).attr('data-count');
        	var parc_id = $('#parc_id').val();
        	var comment = $('#comment').val();
        	var options = $('#options').val();
        	var gi = $('#gi').val();				        	
        	var month = $('#month').val();				        	
        	var year = $('#year').val();				        	
        	var part = $('#part').val();			        	
        	var customer = $('#customer').val();
        	var total_vat = $('#total_vat').val();			        	
        	var total_vat_ex = $('#total_vat_ex').val();
        	var exchange = $('#exchange').val(); 
        	var customer_type = $('#customer_type').val();

			 if(!confirm('Please confirm the details below\nAmount: '+a_amount+'\nCurrency: '+a_currency+'\nInvoice Date: '+date+'')) return false;

			$(this).attr("disabled", true);
			$('#pushInvoiceToEFRISStatus').html('<img src="'+urlPath+'images/loading.gif" alt="loading..."/>');	

			if($('#ref').val()){

				var form_data = new FormData();				
				form_data.append('items', $('#items').val());
				
				form_data.append('comment', comment);
		        form_data.append('part', part);
		        form_data.append('month', month);
		        form_data.append('year', year);
		        form_data.append('parc_id', parc_id);
		        form_data.append('options', options);
		        form_data.append('gi', gi);
		        form_data.append('customer', customer);
		        form_data.append('count', count);
		        form_data.append('type', type);
		        form_data.append('customer_type', customer_type);
		       // alert(comment);

				form_data.append('qty', $('#qty').val());
				form_data.append('buyer', $('#buyer').val());
				form_data.append('ref', $('#ref').val());
				form_data.append('id', $('#id').val());
				form_data.append('currency', $('#a_currency').val());
				form_data.append('vat', $('#vat').val());
				form_data.append('date', $('#date').val());
				form_data.append('unitPrice', $('#unitPrice').val());
				form_data.append('a_amount', a_amount);
		        form_data.append('total_vat', total_vat);
		        form_data.append('total_vat_ex', total_vat_ex);
		        form_data.append('exchange', exchange);


				//return 0;
				
				$.ajax({
					url: urlPath+"ajax/pushFibreCustomerInvoice.php",
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
				$('#pushInvoiceToEFRISStatus').html("");
			}
		});
	</script>

	<?php $this->opticApprovals(user_id(), $ms, $id, 0, $ni, $sdate); ?>
	<?php
		}
	}
	public function PendingApprovalOpticFibreInvoicesAction(){
		if(($this->notifyAll()&&user_id()==static_generator2()) || $this->toApprove()){
			//echo '<div style="width:100px;margin:10px 0;" onclick="return multipleChecker(); " id="checker">Check All</div>';
		}
		echo '<form action="" method="post">';
		echo '<table border="1" id="table" cellspacing="0" cellpadding="1">';
		echo '<thead>';
		echo '<tr>';
		echo '<th width="30px">No.</th>';
		echo '<th>Date</th>';
		//echo '<th>Invoice No.</th>';
		echo '<th>Customer Name</th>';
		echo '<th>Period</th>';
		echo '<th>Status</th>';
		echo '</tr>';
		echo '</thead>';

		echo '<tbody>';
		$db = new Db();
		
			$user_id = user_id();
		    $designation = $this->rgf("sysuser", $user_id, "user_id", "user_designation");
		    if($designation == '35'){
		    	$select = $db->select("SELECT * FROM notification WHERE (not_selected2 IS NOT NULL AND not_app1 = 1 AND not_app2 IS NULL) OR (not_selected3 IS NOT NULL AND not_app2 = 1 AND not_app3 IS NULL)  ORDER BY not_date_added");
		    }else{
		    	$select = $db->select("SELECT * FROM notification WHERE (not_selected2 = '$designation' AND not_app1 = 1 AND not_app2 IS NULL) OR (not_selected3 = '$designation' AND not_app2 = 1 AND not_app3 IS NULL)  ORDER BY not_date_added");
		    }
			
			//echo "SELECT * FROM notification WHERE (not_selected1 = '$designation' AND not_app1 = 1) OR (not_selected2 = '$designation' AND not_app2 = 1) AND not_app3 != 1    ORDER BY not_date_added DESC";

			$check = array();
			$cus = array();
			$count = 1;
		//if(user_id() == static_generator2()){
			foreach($select[0] as $row){
				extract($row);
				$ed = $fs_effective_date;
				$q = $fs_qty;

				$sd = $this->notified($fs_customer_id, $fs_contract);
				
				// while($ed < (time()+7*24*60*60)){
				// 	$p = $this->invoiceCreator($ed, $q);
				// 	$start_date = strtotime($p['start_date']);
				// 	$ed = strtotime($p['next_start']);

				// 	//date greater
				// 	if(!in_array($start_date, $sd) && $start_date > $this->startNotification())
				// 	{
				// 		if(!in_array($ofc_id.$start_date, $check))
				// 		{
				// 			$edl = $ed - (24*60*60);
				// 			$check[] = $ofc_id.$start_date;
				// 			$c = '<tr>';
				// 			//$c .= '<td>'.($count+1).'.</td>';
				// 			$c .= '<td>'.$n.'</td>';
				// 			$c .= '<td align="center"> - </td>';
				// 			$c .= '<td align="center"> - </td>';
				// 			$c .= '<td><a href="'.return_url().'optic-fibre-customers/invoice/'.$ofc_id.'/'.$start_date.'">'.$this->rgf("of_customer", $ofc_id, "ofc_id", "ofc_customer_name").'</a></td>';
				// 			$c .= '<td>'.Feedback::date_s($start_date).' - '.Feedback::date_s($edl).'</td>';
				// 			$c .= '<td><a href="'.return_url().'optic-fibre-customers/invoice/'.$ofc_id.'/'.$start_date.'" class="btn btn-xs btn-danger">Pending</a></td>';
				// 			$c .= '</tr>';

				// 			echo $c;
				// 			$count++;
				// 			$cus[$ofc_id] = array(
				// 				'start_date'=>$start_date,
				// 				'end_date'=>$edl,
				// 			);
				// 		}
				// 	}
				// }
			
		

		// $count=1;
		// $select = $db->select("SELECT * FROM notification ORDER BY not_id DESC");
		// foreach($select[0] as $row){
		// 	extract($row);
			$l = $this->isApproval(user_id(), $not_id);
			$deg = $this->level($not_customer_id,$not_start_date, $l, $last=0);
			$approve = false;
			if($deg == $this->rgf("sysuser", user_id(), "user_id", "user_designation")){
				if(empty(${"not_app".$l})){
					$approve = true;
				}
			}
			echo '<tr>';
			// if($approve){
			// 	echo '<td><input class="AllCheckBoxes AllCheckBoxesLine'.$i.'  AllCheckBoxesColumn'.$n.'" type="checkbox" style="width:20px" name="options[]" value="'.$not_customer_id.'" title="'.$control.'"  id="la'.$count.'"><label for="la'.$count.'"></label></td>';
			// 	$cus[$not_customer_id] = array(
			// 		'start_date'=>$not_start_date,
			// 		'end_date'=>$not_end_date,
			// 	);
			// }else

			{
				echo '<td>'.($count++).'.</td>';
			}
			echo '<td>'.Feedback::date_fm($not_date_added).'</td>';
			//echo '<td>'.$not_id.'</td>';
			echo '<td>'.$this->rgf("of_customer",$not_customer_id, "ofc_id", "ofc_customer_name").'</td>';
			echo '<td>'.Feedback::date_s($not_start_date).' - '.Feedback::date_s($not_end_date).'  </td>';
			$k = 0;
			for($i=1; $i<=3; $i++){
				if(!empty(${"not_app".$i})){
					$k ++;
				}
			}
			
			if($k==3){
				$status = "Approved";
			}elseif($k >= 1){
				$status = "Pending ".$this->rgf("designation", ${"not_selected".($k+1)}, "designation_id", "designation_name")."'s Approval(".($not_app3)."/3)";
			}else{
				$status = "Pending Submission for Approvals";
			}
			//echo "@@".$k;
			echo '<td><a href="'.return_url().'optic-fibre-customers/invoice/'.$not_customer_id.'/'.$not_start_date.'">'.$status.'</a></td>';
			echo '</tr>';
		}
		//}
		echo '</tbody>';
		echo '</table>';

		if(($this->notifyAll()&&user_id()==static_generator2()) || $this->toApprove()){
			//$this->opticApprovals(user_id(), $ms, $cus);
		}

	}
	public function approveOpticFibreInvoicesAction(){
		if(($this->notifyAll()&&user_id()==static_generator2()) || $this->toApprove()){
			//echo '<div style="width:100px;margin:10px 0;" onclick="return multipleChecker(); " id="checker">Check All</div>';
		}
		echo '<form action="" method="post">';
		echo '<table border="1" id="table" cellspacing="0" cellpadding="1">';
		echo '<thead>';
		echo '<tr>';
		echo '<th width="30px">No.</th>';
		echo '<th>Date</th>';
		//echo '<th>Invoice No.</th>';
		echo '<th>Customer Name</th>';
		echo '<th>Period</th>';
		echo '<th>Status</th>';
		echo '</tr>';
		echo '</thead>';

		echo '<tbody>';
		$db = new Db();
		
			$user_id = user_id();
		    $designation = $this->rgf("sysuser", $user_id, "user_id", "user_designation");
		    if($designation =='35'){
		    	$select = $db->select("SELECT * FROM notification WHERE not_selected3 IS NOT NULL AND not_app3 = 1  ORDER BY not_date_added DESC");

		    }else{
		    	$select = $db->select("SELECT * FROM notification WHERE not_selected3 = '$designation' AND not_app3 = 1  ORDER BY not_date_added DESC");

		    }
			$check = array();
			$cus = array();
			$count = 1;
		//if(user_id() == static_generator2()){
			foreach($select[0] as $row){
				extract($row);
				$ed = $fs_effective_date;
				$q = $fs_qty;

				$sd = $this->notified($fs_customer_id, $fs_contract);
				
				// while($ed < (time()+7*24*60*60)){
				// 	$p = $this->invoiceCreator($ed, $q);
				// 	$start_date = strtotime($p['start_date']);
				// 	$ed = strtotime($p['next_start']);

				// 	//date greater
				// 	if(!in_array($start_date, $sd) && $start_date > $this->startNotification())
				// 	{
				// 		if(!in_array($ofc_id.$start_date, $check))
				// 		{
				// 			$edl = $ed - (24*60*60);
				// 			$check[] = $ofc_id.$start_date;
				// 			$c = '<tr>';
				// 			//$c .= '<td>'.($count+1).'.</td>';
				// 			$c .= '<td>'.$n.'</td>';
				// 			$c .= '<td align="center"> - </td>';
				// 			$c .= '<td align="center"> - </td>';
				// 			$c .= '<td><a href="'.return_url().'optic-fibre-customers/invoice/'.$ofc_id.'/'.$start_date.'">'.$this->rgf("of_customer", $ofc_id, "ofc_id", "ofc_customer_name").'</a></td>';
				// 			$c .= '<td>'.Feedback::date_s($start_date).' - '.Feedback::date_s($edl).'</td>';
				// 			$c .= '<td><a href="'.return_url().'optic-fibre-customers/invoice/'.$ofc_id.'/'.$start_date.'" class="btn btn-xs btn-danger">Pending</a></td>';
				// 			$c .= '</tr>';

				// 			echo $c;
				// 			$count++;
				// 			$cus[$ofc_id] = array(
				// 				'start_date'=>$start_date,
				// 				'end_date'=>$edl,
				// 			);
				// 		}
				// 	}
				// }
			
		

		// $count=1;
		// $select = $db->select("SELECT * FROM notification ORDER BY not_id DESC");
		// foreach($select[0] as $row){
		// 	extract($row);
			$l = $this->isApproval(user_id(), $not_id);
			$deg = $this->level($not_customer_id,$not_start_date, $l, $last=0);
			$approve = false;
			if($deg == $this->rgf("sysuser", user_id(), "user_id", "user_designation")){
				if(empty(${"not_app".$l})){
					$approve = true;
				}
			}
			echo '<tr>';
			// if($approve){
			// 	echo '<td><input class="AllCheckBoxes AllCheckBoxesLine'.$i.'  AllCheckBoxesColumn'.$n.'" type="checkbox" style="width:20px" name="options[]" value="'.$not_customer_id.'" title="'.$control.'"  id="la'.$count.'"><label for="la'.$count.'"></label></td>';
			// 	$cus[$not_customer_id] = array(
			// 		'start_date'=>$not_start_date,
			// 		'end_date'=>$not_end_date,
			// 	);
			// }else

			{
				echo '<td>'.($count++).'.</td>';
			}
			echo '<td>'.Feedback::date_fm($not_date_added).'</td>';
			//echo '<td>'.$not_id.'</td>';
			echo '<td>'.$this->rgf("of_customer",$not_customer_id, "ofc_id", "ofc_customer_name").'</td>';
			echo '<td>'.Feedback::date_s($not_start_date).' - '.Feedback::date_s($not_end_date).'  </td>';
			$k = 0;
			for($i=1; $i<=3; $i++){
				if(!empty(${"not_app".$i})){
					$k ++;
				}
			}
			
			if($k==3){
				$status = "Approved";
			}elseif($k >= 1){
				$status = "Pending ".$this->rgf("designation", ${"not_selected".($k+1)}, "designation_id", "designation_name")."'s Approval(".($not_app3)."/3)";
			}else{
				$status = "Pending Submission for Approvals";
			}
			//echo "@@".$k;
			echo '<td><a href="'.return_url().'optic-fibre-customers/invoice/'.$not_customer_id.'/'.$not_start_date.'">'.$status.'</a></td>';
			echo '</tr>';
		}
		//}
		echo '</tbody>';
		echo '</table>';

		if(($this->notifyAll()&&user_id()==static_generator2()) || $this->toApprove()){
			//$this->opticApprovals(user_id(), $ms, $cus);
		}

	}

	public function isApproval($user_id, $notification_id){
		$db = new Db();
		$user_designation = $this->rgf("sysuser", $user_id, "user_id", "user_designation"); 
		$this->rgf("designation", $role_id, "designation_id", "designation_name");
		
		$select = $db->select("SELECT not_selected2, not_selected3 FROM notification WHERE not_id = '$notification_id'");
		
		if($db->num_rows()==0){
			return 0;			
		}else{			
			extract($select[0][0]);
			if($not_selected2==$user_designation){
				return 2;
			}elseif($not_selected3==$user_designation){
				return 3;
			}
			return 0;			
		}
		
	}

	public function level($customer_id,$start_date, $count=0, $last=0){
		
		//tfr_app
		$u = new Db();
		$lev = array();
		for($i=0; $i<$count-1; $i++){
			$lev[] = "not_app".($i+1)." = 1";
		}
		if($last==0){ 
		//$last = "NULL";
			$lev[]= "not_app{$count} IS NULL";
		}else{
			$lev[]= "not_app{$count} = $last";
		}
		$leve = implode(" AND ", $lev);
		$sql = "SELECT not_selected$count FROM notification WHERE $leve AND not_customer_id = '$customer_id' AND not_start_date = '$start_date'";
		$status = $u->select($sql);
		
		if($u->num_rows()==1){
			extract($status[0][0]);
			return ${"not_selected".$count};
		}else{
			return false;
		}
	}

	public function toApprove(){
		$db = new Db();
		// $select = $db->select("SELECT * FROM notification ORDER BY not_id DESC");
		// $approve=0;
		// foreach($select[0] as $row){
		// 	extract($row);
		// 	$l = $this->isApproval(user_id(), $not_id);
		// 	$deg = $this->level($not_customer_id,$not_start_date, $l, $last=0);
		// 	if($deg == $this->rgf("sysuser", user_id(), "user_id", "user_designation")){
		// 		if(1){
		// 			if(empty(${"not_app".$l})){
		// 				$approve++;
		// 			}
		// 		}else{		
		// 			// $ll = $l - 1;
		// 			// if(empty(${"not_app".$l}) && !empty(${"not_app".$ll})){
		// 			// 	$approve++;
		// 			// }
		// 		}
		// 	}
		// }
		   $user_id = user_id();
		    $designation = $this->rgf("sysuser", $user_id, "user_id", "user_designation");
		    if($designation == '35'){
		    	$select = $db->select("SELECT * FROM notification WHERE (not_selected2 IS NOT NULL AND not_app1 = 1 AND not_app2 IS NULL) OR (not_selected3 IS NOT NULL AND not_app2 = 1 AND not_app3 IS NULL)  ORDER BY not_date_added");
		    }else{
		    	$select = $db->select("SELECT * FROM notification WHERE (not_selected2 = '$designation' AND not_app1 = 1 AND not_app2 IS NULL) OR (not_selected3 = '$designation' AND not_app2 = 1 AND not_app3 IS NULL)  ORDER BY not_date_added");
		    }
			$approve = $db->num_rows();
		return $approve;
	}

	public function opticApprovals($user_id, $ms="", $cus, $invoice=0, $gi=0, $period=0){
		$cid = $cus;
		//echo $this->rgf("sysuser", $user_id, "user_id", "user_designation");
		$db = new Db();
		$select = $db->select("SELECT app_role_id FROM approval_order ORDER BY app_id ASC");
		
		echo '<div style="font-size: 12px;">';
		$tc=1;
		echo '<h2 style="border-bottom:1px solid #000; margin-top:15px; font-size: 16px;">Approvals:</h2>';
		echo '<ol>';

		/////////////////////////////////////////////////////////////
		$select12 = $db->select("SELECT * FROM fibre_service WHERE fs_customer_id = '$cus'");
		
		if($db->num_rows()>=1){
			$isThere = 1;
			extract($select12[0][0]);
			$effective_date = $fs_effective_date;
			$rate = $fs_rate;
			$qty = $fs_qty;
		}

		$ed = $fs_effective_date;
				$q = $fs_qty;

				$sd = $this->notified($fs_customer_id, $contract);
				//print_r($sd);
				while($ed < (time()+7*24*60*60)){
					$p = $this->invoiceCreator($ed, $q);
					$start_date = strtotime($p['start_date']);
					$ed = strtotime($p['next_start']);

					//date greater
					if(!in_array($start_date, $sd) && $start_date > $this->startNotification())
					{
						if(!in_array($ofc_id.$start_date, $check))
						{
							$edl = $ed - (24*60*60);
							$check[] = $ofc_id.$start_date;
							
							$cus[$ofc_id] = array(
								'start_date'=>$start_date,
								'end_date'=>$edl,
							);
						}
					}
				}

				//echo $period;


		$year = date('Y', $period);
		$month = date('m', $period);

		/////////////////////////////////////////////////////////


		$k = 0;
		foreach($select[0] as $row){
			extract($row);
			$k++;
			echo '<div style="width:29%;padding:0 2%; float:left;">';
		
			if($tc==1)
				echo '<li><b>'.strtoupper('Prepared By:</b></li>');
		
			else
				echo '<li><b>'.strtoupper('Approved By:</b></li>');

			$d = new Db();

			$ttt = array($cid); 
			//print_r($ttt);
			foreach($ttt as $cu){
				$custo = $cu;
				if($tc == 1){
					$sql = "SELECT * FROM notification WHERE not_customer_id = '$custo' AND not_app$tc = 1 AND not_id = '$gi'";
					$sel = $d->select($sql);
				}else{
					$tcn = $tc-1;
					$sql = "SELECT * FROM notification WHERE not_customer_id = '$custo' AND not_app$tc = 1 AND not_app$tcn = 1 AND not_id = '$gi'";
					$sel = $d->select($sql);
				}
			}
	
			if($d->num_rows()){
				$dd = new Db();

				extract($sel[0][0]);
				
				if($invoice){

					//echo '<br/><br/>................................<br/>';
					$sele = $dd->select("SELECT arc_added_by FROM all_readings_comments WHERE arc_year = '$year' AND arc_month = '$month' AND arc_part = '$tc' AND arc_customer='$cus' ");

					if($dd->num_rows()){
						extract($sele[0][0]);
					}

					//echo '<b style="font-size:8pt">'.ucwords(strtolower(''.$this->rgf("designation", $this->rgf("sysuser", $not_added_by, "user_id", "user_designation"), 'designation_id', 'designation_name').'</b>'));	
					
					if($tc==1)
					echo '<b>'.strtoupper(''.$this->rgf("designation", 52, 'designation_id', 'designation_name').'</b><br/>');	
					else
					echo '<b>'.strtoupper(''.$this->rgf("designation", ${"not_selected".$tc}, 'designation_id', 'designation_name').'</b><br/>');	
										
					$tc_num = $tc;
				}else{
				
					$sele = $dd->select("SELECT arc_added_by FROM all_readings_comments WHERE arc_year = '$year' AND arc_month = '$month' AND arc_part = '$tc' AND arc_customer = '$cid'");

					if($dd->num_rows()){
						extract($sele[0][0]);
					}

					echo '<b>'.strtoupper(''.$this->rgf("designation", $this->rgf("sysuser", $not_added_by, "user_id", "user_designation"), 'designation_id', 'designation_name').'</b><br/>');

					//extract($sel[0][0]);
					
					//echo ''.$this->full_name($dl_added_by).'	';
					echo ''.$this->full_name(${'dl_selected'.$tc}).'	';
					$month = (int)$month;
					$sql = "SELECT * FROM all_readings_comments WHERE arc_year = '$year' AND arc_month = '$month' AND arc_part = '$tc' AND arc_type = 'FIBRE' AND arc_customer = '$cid'";
					$sel = $d->select($sql);
					if($d->num_rows()){
						$sel[0][0];
						if(!empty($arc_description))
							echo '<br/><b>Comment: </b><br/>';
					}
					foreach($sel[0] as $row){
						extract($row);
						echo nl2br($arc_description);
							if($tc==1)
						echo '<br/><b>Invoice Date: </b>'.FeedBack::date_s($arc_invoice_date).'<br/>';
						//echo '<br/><b>Date: </b>'.FeedBack::date_s(time()).'<br/>';
						
					}
				}
			}else{


				if($tc == 1){
					$tot = 1;
				}else{
					$tcn = $tc-1;
				
					$tcn = $tc-1;
					$user_designation = $this->rgf("sysuser", user_id(), "user_id", "user_designation");
					$sql = "SELECT * FROM notification WHERE not_selected$tc = $user_designation AND not_app$tc IS NULL AND not_app$tcn IS NOT NULL AND not_id = '$not_id'";
					$sel = $d->select($sql);
					$tot = $d->num_rows();
				}

				if(1){
					extract($sel[0][0]);

					//if($invoice == 0 && (($tc == 1 && user_id() == static_generator2())|| ${"not_selected".$tc} == $this->rgf("sysuser", user_id(), "user_id", "user_designation")&&$this->rgf("notification", $not_id, "not_id", "not_app".($tc-1))==1)){
					if($invoice == 0 && (($tc == 1 )|| ${"not_selected".$tc} == $this->rgf("sysuser", user_id(), "user_id", "user_designation")&&$this->rgf("notification", $not_id, "not_id", "not_app".($tc-1))==1)){	

					echo '<form action="" method="post" style="background-color: #fbe2e2;padding:10px; border:1px solid #000; ">';

						//echo '<div style="background-color: #fbe2e2;padding:10px; border:1px solid #000; ">';

						echo '<div>';
						
						echo '<input type="hidden" value="'.$cid.'" id="options"/>';



						if(isset($_POST['approve']) && $_POST['part']== $tc){
							echo '';

							$ind = array();
							$comment = $_POST['comment'];
							$options = $_POST['options'];
							$part = $_POST['part'];
							$parc_id = $_POST['parc_id'];

							
							if($tc == 1){

								//$this->assignInvoiceNumber($year, $month);
								
								$approveOne = $_POST["approveOne"];
								$approveTwo = $_POST["approveTwo"];							
								
							}
								
							$ind["not_app$tc"] = 1;
							
							$com = array();
							$com["arc_year"] = $year;
							$com["arc_month"] = $month;
							$com["arc_description"] = $comment;
							$com["arc_date_added"] = time();
							$com["arc_added_by"] = user_id();
							$com["arc_part"] = $part;
							$com["arc_type"] = "FIBRE";

							


							$insert = $d->insert("all_readings_comments", $com);
							echo $d->error();
							$link = ''.return_url().portion(1)."/".portion(2)."/".portion(3)."/".portion(4).'">'.$this->rgf("of_customer", $cu, "ofc_id", "ofc_customer_name").''; //return_url().'optic-fibre-customers/approve-optic-fibre-invoices';
							
							if($tc==1){

								$approveOne = $_POST["approveOne"];
								$approveTwo = $_POST["approveTwo"];
								$msg = "";
								foreach($options as $cu){
									//$ind['not_start_date'] = $cus[$cu]['start_date'];
									//$ind['not_end_date'] = $cus[$cu]['end_date'];
									$ind['not_date_added'] = time();
									$ind['not_added_by'] = user_id();
									$ind['not_selected1'] = $this->rgf("sysuser", user_id(), "user_id", "user_designation");
									$ind['not_selected2'] = $approveOne;
									$ind['not_selected3'] = $approveTwo;
									//$ind['not_customer_id'] = $cu;
									$insert = $d->update("notification", $ind, ["not_id"=>$gi]);
									echo $d->error();

									$period = Feedback::date_s($start_date);
									$period .= " - ".Feedback::date_s($edl);

									$msg .= '<li><a href="'.return_url().portion(1)."/".portion(2)."/".portion(3)."/".portion(4).'">'.$this->rgf("of_customer", $cu, "ofc_id", "ofc_customer_name").'</a> &nbsp; ('.$period.')</li>';
								}
								
								$next = $this->next($tc, $cid, $start_date);
								$to = ($next['email'])?$next['email']:"josemusiitwa@gmail.com";
								$message = "Hello ".$next['fname'].",";	

								if(count($options) > 1){
									$message .= "<br/><br/>The following invoices are ready for approval:<br/>";
								}elseif(count($options) == 1){
									$message .= "<br/><br/>The following invoice is ready for approval:<br/>";
								}
								$message .= '<ol>';
								$message .= $msg;
								$message .= '</ol>';
								$message .= "\r\nYou can use this link: <a href='$link'>$link</a>";	
								$message .= $ms;

							    $subject = "Optic Fibre Invoice Approval";
							  
							    Feedback::sendmail($to,$subject,$message,$name);
							}else{

								
								$next = $this->next($tc, $cid, $start_date);
								//print_r($next);
								$to = ($next['email'])?$next['email']:"josemusiitwa@gmail.com";
								$message = "Hello ".$next['fname'].",";

								if($tc == 2){
									$msg = "";
									foreach($options as $cu){
										$z = $cu;
										
										$ind['not_app'.$tc] = 1;

										$not_id;
										$insert = $d->update("notification", $ind, ["not_id"=>$gi]);
										$period = Feedback::date_s($start_date);
										$period .= " - ".Feedback::date_s($edl);
										$msg .= '<li><a href="'.return_url().portion(1)."/".portion(2)."/".portion(3)."/".portion(4).'">'.$this->rgf("of_customer", $cu, "ofc_id", "ofc_customer_name").'</a> &nbsp; ('.$period.')</li>';
									}

									if(count($options) > 1){
										$message .= "<br/><br/>The following invoices are ready for approval:<br/>";
									}elseif(count($options) == 1){
										$message .= "<br/><br/>The following invoice is ready for approval:<br/>";
									}
									$message .= '<ol>';
									$message .= $msg;
									$message .= '</ol>';
										
									$message .= $ms;
								    
								}elseif($tc == 3){
									$msg = "";
									foreach($options as $cu){
										$z = $cu;
																			
										$ind['not_app'.$tc] = 1;

										$not_id;
										$insert = $d->update("notification", $ind, ["not_id"=>$gi]);
										$period = Feedback::date_s($start_date);
										$period .= " - ".Feedback::date_s($edl);
										$msg .= '<li><a href="'.return_url().portion(1)."/".portion(2)."/".portion(3)."/".portion(4).'">'.$this->rgf("of_customer", $cu, "ofc_id", "ofc_customer_name").'</a> &nbsp; ('.$period.')</li>';
									}

									if(count($options) > 1){
										$message .= "<br/><br/>The following invoices are successfully approved:<br/>";
									}elseif(count($options) == 1){
										$message .= "<br/><br/>The following invoice is successfully approved:<br/>";
									}
									$message .= '<ol>';
									$message .= $msg;
									$message .= '</ol>';
										
									$message .= $ms;					
								}
									
								
								$message .= "<br/>You can use this link: <a href='$link'>$link</a>";	
								$subject = "Optic Fibre Invoice Approval";
							    Feedback::sendmail($to,$subject,$message,$name);
								
							}

							FeedBack::redirect(return_url().portion(1)."/".portion(2)."/".portion(3)."/".portion(4));
						}

						echo 'Comment:<br/><textarea id="comment" name="comment" style="width: 100%;"></textarea>';
						
						echo '<input type="hidden" id="part" value="'.$tc.'"/>';

						if($tc==1){	
							echo 'Invoice Date:<br/><input type="date" id="selected_invoice_date" style="width:100%" value=""/>';	

							$app = array("One", "Two");
							foreach($app as $ap){
								echo '<br/>Select Approve '.$ap.':';
								echo '<select style="width:100%" required id="Approve'.$ap.'">';
								echo '<option value="">Select</option>';

								$t = new Db();
								$tt = $t->select("SELECT * FROM designation");
								foreach($tt[0] as $row){
									extract($row);
									echo '<option value="'.$designation_id.'">'.$designation_name.'</option>';
								}
								echo '</select>';

								echo '<br/>';
							}
						
							echo '<br/>';
							echo '<button data-type="approve" class="approveBtn btn btn-xs btn-success" data-count="'.$tc.'" type="button" name="approve"><i class="fa fa-fw fa-check"></i> Forward</button>';
							   echo '<input type="hidden" name="parc_id" id="parc_id" value="'.$dl_id.'"/>';

						}else{
							if($tc == 3){
								//echo '<button data-type="approve" data-count="'.$tc.'" type="button" name="approve" id="pushInvoiceToEFRIS" class="btn btn-xs btn-success"><i class="fa fa-fw fa-check"></i> Approve</button>';	
								echo '<button data-type="approve" data-count="'.$tc.'" type="button" name="approve" class="approveBtn btn btn-xs btn-success"><i class="fa fa-fw fa-check"></i> Approve</button>';	
							}else{
								echo '<button data-type="approve" data-count="'.$tc.'" type="button" name="approve" class="approveBtn btn btn-xs btn-success"><i class="fa fa-fw fa-check"></i> Approve</button>';	
							}

							echo ' &nbsp;&nbsp;<button data-type="reject" data-count="'.$tc.'" type="button" name="approve" class="approveBtn btn btn-xs btn-danger"><i class="fa fa-fw fa-check"></i> Reject</button>';		
							echo '<input type="hidden" name="parc_id" id="parc_id" value="'.$dl_id.'"/>';	

						}
						echo '<div id="approvalStatus"></div>';


							echo '<input type="hidden" name="" id="year" value="'.$year.'"/>';
							echo '<input type="hidden" name="" id="month" value="'.$month.'"/>';
							echo '<input type="hidden" name="" id="customer" value="'.$cu.'"/>';
							echo '<input type="hidden" name="" id="part" value="'.$tc.'"/>';
							echo '<input type="hidden" name="" id="gi" value="'.$gi.'"/>';

						echo '</div>';

						echo '</form>';
						?>
						<script type="text/javascript">							
				        $(document).off('click','#pushInvoiceToEFRIS2').on('click', '#pushInvoiceToEFRIS2', function(e){
				        	alert("reached");
				        });
				        $(document).off('click','.approveBtn').on('click', '.approveBtn', function(e){
				        	$('#approvalStatus').html('');
				        	var urlPath = $('#urlPath').val();
				        	var type = $(this).attr('data-type');
				        	var count = $(this).attr('data-count');
				        	var parc_id = $('#parc_id').val();
				        	var comment = $('#comment').val();
				        	var selected_invoice_date = $('#selected_invoice_date').val();
				        	var options = $('#options').val();
				        	var gi = $('#gi').val();
				        	//alert(parc_id);
				        	
					        //alert(parc_id);				        	
				        	var month = $('#month').val();				        	
				        	var year = $('#year').val();				        	
				        	var part = $('#part').val();			        	
				        	var customer = $('#customer').val();

				        	if(part == 1){
					        	var ApproveOne = $('#ApproveOne').val();
					        	var ApproveTwo = $('#ApproveTwo').val();

					        }

				        	$('.approveBtn').attr('disabled',true);

				        	if(!comment){
				        		alert("Enter Comment");
				        		$('.approveBtn').attr('disabled',false);
				        	}else if(part == 1 && (!ApproveOne||!ApproveTwo)){
				        		alert("Select Approval One & Two ");
				        		$('.approveBtn').attr('disabled',false);
				        	} else if(!selected_invoice_date && part == 1){
					        	alert("Please Enter Invoice Date");
					        	$('.approveBtn').attr('disabled',false);
					        }
				        	else{
				        		$('#approvalStatus').html('Sending. Please wait.');
						        var form_data = new FormData();
						        form_data.append('selected_invoice_date', selected_invoice_date);
						        form_data.append('comment', comment);
						        form_data.append('part', part);
						        form_data.append('month', month);
						        form_data.append('year', year);
						        form_data.append('parc_id', parc_id);
						        form_data.append('options', options);
						        form_data.append('gi', gi);
						        if(part == 1){
							        form_data.append('ApproveOne', ApproveOne);
							        form_data.append('ApproveTwo', ApproveTwo);
							    }
							    // alert(ApproveOne);
							    // alert(ApproveTwo);
						        form_data.append('customer', customer);
						        form_data.append('count', count);
						        form_data.append('type', type);
						        $.ajax({
						            url: urlPath+"ajax/approveFibreSales.php",
						            type: "POST",
						            data: form_data,
						            contentType: false,
						            cache: false,
						            processData:false,
						            success: function(data){
						            	//alert(data);
				        				$('#approvalStatus').html('Sent. Reloading Page.');
						            	//alert(data);
				        				//$('#netAdvanceBtnStatus'+id).html("Saved. Reloading Page");
						            	location.reload();
						            }
						        });
				        	}

				        });	
						</script>
						<?php
						//echo "<br/><br/>........................................<br/>";
						if($tc == 1){
							echo '<b>'.strtoupper(''.$this->rgf("designation", $this->rgf("sysuser", $user_id, "user_id", "user_designation"), 'designation_id', 'designation_name').'</b><br/>');
						}else{
							echo '<b>'.strtoupper(''.$this->rgf("designation", ${"not_selected".$tc}, 'designation_id', 'designation_name').'</b><br/>');
						}

	
					}else{

						if($tc == 1){
							echo '<b>'.strtoupper(''.$this->rgf("designation", $this->rgf("sysuser", $user_id, "user_id", "user_designation"), 'designation_id', 'designation_name').'</b><br/>');
						}else{
							echo '<b>'.strtoupper(''.$this->rgf("designation", ${"not_selected".$tc}, 'designation_id', 'designation_name').'</b><br/>');
						}
						 echo 'Not Yet';
					}

				}else{
					if($tc == 1){
						echo '<b>'.strtoupper(''.$this->rgf("designation", $this->rgf("sysuser", $user_id, "user_id", "user_designation"), 'designation_id', 'designation_name').'</b><br/>');
					}else{
						echo '<b>'.strtoupper(''.$this->rgf("designation", ${"not_selected".$tc}, 'designation_id', 'designation_name').'</b><br/>');
					}
					 echo 'Not Yet';
				}

			}

			

			echo '<div style="padding-left:10px;">';



			//echo '<b>Approved By: </b>Musiitwa Joseph<br/>';
			//echo '<b>Date: </b>'.FeedBack::date_fm(time()).'<br/>';
			echo '</div>';
			echo '</div>';

			$tc++;

		}
		echo '</ol>';
		echo '</div>';
		echo '</div>';
		    $db = new Db();
		    $sql = "SELECT * FROM all_readings_comments WHERE arc_year = '$year' AND arc_month = '$month' AND arc_part = '1' AND arc_type = 'FIBRE' AND arc_customer = '$cid'";
					$sel = $db->select($sql);
					if($db->num_rows()){
						extract($sel[0][0]);
					}	


		return $tc_num = array('tc' =>$tc_num,'invoice_date'=>$arc_invoice_date);
			
	}

	public function next($level, $customer_id, $start_date){
		if($level == 3){
			$level = 1;
		}else{
			$level = $level+1;
		}
		$db = new Db();
		$select = $db->select("SELECT not_selected".($level)." as did FROM notification WHERE not_customer_id = '$customer_id' AND not_start_date = '$start_date' ");
		extract($select[0][0]);

		$user_id = $this->rgf("sysuser", $did, "user_designation", "user_id");

		$fname = $this->rgf("sysuser", $user_id, "user_id", "user_surname");
		$sname = $this->rgf("sysuser", $user_id, "user_id", "user_othername");
		$email = $this->rgf("sysuser", $user_id, "user_id", "user_email");

		return array('fname'=>$fname, 'sname'=>$sname, 'email'=>$email);

	}

	public function expiredContracts(){
		$total_expired = array();
		$db = new Db();
		$select = $db->select("SELECT * FROM of_customer ORDER BY ofc_customer_name ASC");
		foreach($select[0] as $row){
			extract($row);			
			
			$b = new Db();
			$bb = $b->select("SELECT * FROM contract WHERE contract_ofc_id = '$ofc_id'");
			if($b->num_rows()){
				extract($bb[0][0]);

				if(Feedback::status($contract_start_date, $contract_end_date, $ofc_id)){
					$total_expired[] = $ofc_id;
				}
			}
		}

		return $total_expired;
	}

	public function addOpticFibreServiceAction(){
		$id = portion(3);
		$db = new Db();
		$select = $db->select("SELECT * FROM fibre_service WHERE fs_customer_id = '$id'");
		$isThere = 0;
		if($db->num_rows()>=1){
			$isThere = 1;
			extract($select[0][0]);
			$effective_date = $fs_effective_date;
			$rate = $fs_rate;
			$qty = $fs_qty;
		}

		if(isset($_POST['submit'])){
			$qty = $_POST['qty'];
			$effective_date = $_POST['effective_date'];
			$rate = $_POST['rate'];
			$description = trim($_POST['description']);
			$outlet = trim($_POST['outlet']);
			$desc = trim($_POST['desc']);


			$time = time();
			$user = user_id();
			$db = new Db();
			
			$errors = array();

			if(empty($qty)){
				$errors[]="Select Quantity";
			}
			if(empty($effective_date)){
				$errors[]="Select Effective Date";
			}
			if(empty($rate)){
				$errors[]="Enter Rate";
			}
			if(empty($description)){
				$errors[]="Enter Description";
			}
			if(empty($desc)){
				$errors[]="Enter Description";
			}
			if(empty($outlet)){
				$errors[]="Enter Outlet";
			}
			//if($this->isThere("fibre_service", ["fs_description"=>$description])){
			if(0){
				$errors[] = "Service Description already exists";
			}
			$effective_date1 = strtotime($effective_date);
			if(empty($errors)){
				$db = new Db();
				$x = $db->insert("fibre_service",[
				"fs_customer_id"=>$id, 
				"fs_date_added"=>time(), 
				"fs_added_by"=>user_id(), 
				"fs_rate"=>str_replace(',','',$rate), 
				"fs_qty"=>$qty, 
				"fs_description"=>$description, 
				"fs_desc"=>$desc, 
				"fs_outlet"=>$outlet, 
				"fs_effective_date"=>$effective_date1]);
								
				if(!$db->error()){
					FeedBack::success();
					FeedBack::refresh(3, return_url().'optic-fibre-customers/optic-fibre-customer-details/'.$id);
				}else{
					FeedBack::error();
				}
			}else{
				FeedBack::errors($errors);
			}
		}
		?>
		<div class="col-md-12">
			<div class="col-lg-12">
			<br/>
			<div class="panel panel-default">
				<div class="panel-heading">
					Add Optic Fibre Service Form: <b> <?php echo $this->rgf("of_customer", $id, "ofc_id", "ofc_customer_name"); ?></b>
				</div>
				<div class="panel-body">
					<div id="must">All field with asterisk(*) are mandatory.</div>
					
					<div class="row">
						<div class="col-lg-12">
							<form role="form" action="" method="post">
								<div class="row">
									<div class="col-md-4">
										<div class="form-group">
											<label>Select Quantity<span class="must">*</span></label>
											<div class="form-line">
												<?php
												if($isThere == 0){
												 echo '<select name="qty" class="form-control">';
												 	for($i=1; $i<=24; $i++){
												 		$s = ($i>=2)?'s':'';
														if($qty == $i){
															echo '<option selected="select" value="'.$i.'"> '.$i.' Month'.$s.'</option>';
														}else{
															echo '<option value="'.$i.'"> '.$i.' Month'.$s.'</option>';
														}
													}
												echo '</select>';
												}else{
													echo '<select name="qty" disabled class="form-control">';
														for($i=1; $i<24; $i++){
															$s = ($i>=2)?'s':'';
															if($qty == $i){
																echo '<option value="'.$i.'"> '.$i.' Month'.$s.'</option>';
															}
														}
													echo '</select>';

													echo '<input type="hidden" value="'.$qty.'" name="qty"/>';
												}
												?>
											</div>
										</div>
									</div>
									<div class="col-md-4">
										<div class="form-group">
											<label>Effective Date<span class="must">*</span></label>
											<div class="form-line">
												<?php
												if($isThere == 0){
													$ef=date('Y-m-d', $effective_date);
													echo '<input class="form-control" type="date" name="effective_date" placeholder="" value="'.$effective_date.'">';
												}else{
													$ef=date('Y-m-d', $effective_date);
													echo '<input class="form-control" type="date" name="effective_date" disabled placeholder="" value="'.$ef.'">';

													echo '<input type="hidden" value="'.$ef.'" name="effective_date"/>';
												}
												 ?>
											</div>
										</div>
									</div>
									<div class="col-md-4">
										<div class="form-group">
											<label>Rate (USD)<span class="must">*</span></label>
											<div class="form-line">
												<input class="form-control number" type="text" name="rate" placeholder="" value="<?php echo @$rate; ?>">
											</div>
										</div>
									</div>
									<div class="clearfix"></div>
									<div class="col-lg-6">
										<div class="form-group">
											<label>Select Service<span class="must">*</span></label>
											<div class="form-line">
												<select name="description" class="form-control">
													<?php
														$db = new Db();
														$select = $db->select("SELECT * FROM services_fibre");
														foreach($select[0] as $row){
															extract($row);
															echo '<option value="'.$service_id.'">'.$service_name.'</option>';
														}
													?>
												</select>
											</div>
										</div>
									</div>
									<div class="col-lg-6">
										<div class="form-group">
											<label>Select Outlet<span class="must">*</span></label>
											<div class="form-line">
												<select name="outlet" class="form-control">
													<?php
														$db = new Db();
														$select = $db->select("SELECT * FROM outlet");
														foreach($select[0] as $row){
															extract($row);
															echo '<option value="'.$outlet_id.'">'.$outlet_name.'</option>';
														}
													?>
												</select>
											</div>
										</div>
									</div>
									<div class="col-lg-12">
										<div class="form-group">
											<label>Enter Description<span class="must">*</span></label>
											<div class="form-line">
												<textarea class="form-control" name="desc"><?php echo $desc; ?></textarea>
												
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

	public function servicesAction(){
		if(isset($_POST['save'])){
			$service = $_POST['service'];
			$item = $_POST['item'];
			$errors = array();
			if(empty($service)){	
				$errors[] = "Enter Service";
			}
			if(empty($item)){	
				$errors[] = "Select Item";
			}

			if(empty($errors)){
				$db = new Db();
				$insert = $db->insert("services_fibre", [
					"service_name"=>$service,
					"service_date_added"=>time(),
					"service_item_id"=>$item,
					"service_added_by"=>user_id(),
				]);
				if($db->error()){
					Feedback::error($db->error());
				}else{
				Feedback::refresh();
				Feedback::success();
				}
			}else{
				Feedback::errors($errors);
			}

		}
		?>
		<div class="col-md-4">
			<form action="" method="post">
				<br/>
				Add New Service
				<textarea class="form-control" name="service"><?php echo $service; ?></textarea>
				 Item
				<select name="item" class="form-control">
				<option value="">****select*****</option>
				<?php
					$db = new Db();
					$sql = "SELECT * FROM efris_goods WHERE eg_mapped IS NOT NULL";
					$select = $db->select($sql);
					foreach ($select[0] as $key) {
							extract($key);
						if($eg_id == $service_item_id)	
					      echo'<option selected value="'.$eg_id.'">'.$eg_name.'</option>';
					    else
					     echo'<option value="'.$eg_id.'">'.$eg_name.'</option>';
					 }	  			
				?>
				

				
			</select>
				<br/>
				<button class="btn btn-primary" name="save">Save</button>
			</form>
		</div>		
		<div class="col-md-8">
			<br/>
			<table id="table" border=1>
				<tr>
					<th>No</th>
					<th>Service</th>
					<th>Item</th>
					<th>Action</th>
				</tr>
				<?php 
				$i=1;
				$db = new Db();
				$select = $db->select("SELECT * FROM services_fibre");
				//echo $db->num_rows();
				foreach($select[0] as $row){
					extract($row);
					echo '<tr>';
					echo '<td width="1px">'.($i++).'</td>';
					echo '<td>'.($service_name).'</td>';
					echo '<td>'.$this->rgf("efris_goods",$service_item_id,"eg_id","eg_name").'</td>';
					echo '<td width="100px"><a href="'.return_url().'optic-fibre-customers/delete-service/'.$service_id.'" class="btn  btn-xs btn-danger">Delete</a> &nbsp; <a href="#" class="btn btn-success btn-xs " id="add-hint'.$i.'">Edit</a></td>';
					echo '</tr>';

					$co = $i;
					?>

					<div id="myModal<?php echo $co; ?>" class="net-advance-form modal">
					 	<div class="modal-content">
					 		<form action="" method="post">
						 		<?php
						 		if(isset($_POST['saveChanges'.$co])){
						 			$service_name = $_POST['service_name'];
						 			$item = $_POST['item'];
						 			$db->update("services_fibre", [
						 				"service_name"=>$service_name,
						 				"service_item_id"=>$item,
						 			], ["service_id"=>$service_id]);

						 			Feedback::redirect(return_url().'optic-fibre-customers/services');
						 		}
						 		?>
						    	<span id="close<?php echo $co; ?>" class="close" style="color:red;">&times;</span>
						    	<h4>Edit Service</h4>
						    	<textarea required class="form-control" style="height:100px;" name="service_name"><?php echo $service_name; ?></textarea>
						    	 Edit Item
								<select name="item" class="form-control">
									<option value="">****select*****</option>
									<?php
										$db = new Db();
										$sql = "SELECT * FROM efris_goods WHERE eg_mapped IS NOT NULL";
										$select = $db->select($sql);
										foreach ($select[0] as $key) {
												extract($key);
											if($eg_id == $service_item_id)	
										      echo'<option selected value="'.$eg_id.'">'.$eg_name.'</option>';
										    else
										     echo'<option value="'.$eg_id.'">'.$eg_name.'</option>';
										 }	  			
									?>
									

									
								</select>
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
		</div>

		<?php
	}

	public function outLetAction(){
		if(isset($_POST['save'])){
			$outlet = $_POST['outlet'];
			$errors = array();
			if(empty($outlet)){	
				$errors[] = "Enter Outlet";
			}

			if(empty($errors)){
				$db = new Db();
				$insert = $db->insert("outlet", [
					"outlet_name"=>$outlet,
					"outlet_date_added"=>time(),
					"outlet_added_by"=>user_id(),
				]);
				if($db->error()){
					Feedback::error($db->error());
				}else{
				Feedback::refresh();
				Feedback::success();
				}
			}else{
				Feedback::errors($errors);
			}

		}
		?>
		<div class="col-md-4">
			<form action="" method="post">
				<br/>
				Add New Outlet
				<textarea class="form-control" name="outlet"><?php echo $outlet; ?></textarea>
				
				<br/>
				<button class="btn btn-primary" name="save">Save</button>
			</form>
		</div>		
		<div class="col-md-8">
			<br/>
			<table id="table" border=1>
				<tr>
					<th>No</th>
					<th>Outlet</th>
					<th>Action</th>
				</tr>
				<?php 
				$i=1;
				$db = new Db();
				$select = $db->select("SELECT * FROM outlet");
				//echo $db->num_rows();
				foreach($select[0] as $row){
					extract($row);
					echo '<tr>';
					echo '<td width="1px">'.($i++).'</td>';
					echo '<td>'.($outlet_name).'</td>';
					echo '<td width="100px"><a href="'.return_url().'optic-fibre-customers/delete-outlet/'.$outlet_id.'" class="btn  btn-xs btn-danger">Delete</a> &nbsp; <a href="#" class="btn btn-success btn-xs " id="add-hint'.$i.'">Edit</a></td>';
					echo '</tr>';

					$co = $i;
					?>

					<div id="myModal<?php echo $co; ?>" class="net-advance-form modal">
					 	<div class="modal-content">
					 		<form action="" method="post">
						 		<?php
						 		if(isset($_POST['saveChanges'.$co])){
						 			$outlet = $_POST['outlet_name'];

						 			$db->update("outlet", [
						 				"outlet_name"=>$outlet,
						 			], ["outlet_id"=>$outlet_id]);

						 			Feedback::redirect(return_url().'optic-fibre-customers/outlet');
						 		}
						 		?>
						    	<span id="close<?php echo $co; ?>" class="close" style="color:red;">&times;</span>
						    	<h4>Edit Outlet</h4>
						    	<textarea required class="form-control" style="height:100px;" name="outlet_name"><?php echo $outlet_name; ?></textarea>
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
		</div>

		<?php
	}

	public function deleteOutletAction(){		
		$id = portion(3);
		$this->deletor("outlet", "outlet_id",  $id, 'optic-fibre-customers/outlet');
	}


	public function deleteServiceAction(){		
		$id = portion(3);
		$this->deletor("services_fibre", "service_id",  $id, 'optic-fibre-customers/services');
	}


}
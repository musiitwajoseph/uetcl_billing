<?php
Class OpticFibreCustomers extends BeforeAndAfter{
	public $page = "Customers";
	
	public function __construct(){
		$access = new AccessRights();
		//$access->pageAccess(user_id(), $this->page, 'V');
	}
	
	public function getLinks(){
		$page = "Customers";
		
		if(portion(2)=="invoice"||portion(2)=="edit-customer-details"){
			$t = array(
				"link_name"=>"All Invoices", 
				"link_address"=>"optic-fibre-customers/optic-fibre-customer-details/".portion(3),
				"link_icon"=>"fa-list",
				"link_page"=>$page,
				"link_right"=>"V",
			);
		}else if(portion(2)=="add-optic-fibre-service"){
			$t = array(
				"link_name"=>"Customer Details", 
				"link_address"=>"optic-fibre-customers/optic-fibre-customer-details/".portion(3),
				"link_icon"=>"fa-eye",
				"link_page"=>$page,
				"link_right"=>"V",
			);
		}else{
			$t = array(
				"link_name"=>"", 
				"link_address"=>"",
				"link_icon"=>"",
				"link_page"=>$page,
				"link_right"=>"V",
			);
		}

		$links = array(
			array(
				"link_name"=>"Add Optic Fibre Customer", 
				"link_address"=>"optic-fibre-customers/add-customer",
				"link_icon"=>"fa-plus",
				"link_page"=>$page,
				"link_right"=>"A",
			),
			array(
				"link_name"=>"View Optic Fibre Customers", 
				"link_address"=>"optic-fibre-customers/all-customers",
				"link_icon"=>"fa-eye",
				"link_page"=>$page,
				"link_right"=>"V",
			), 

			$t
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
						"ofc_address"=>$address, 
						"ofc_added_by"=>user_id(), 
						"ofc_date_added"=>time(), 
						"ofc_street"=>$street, 
						"ofc_postal_code"=>$postal_code, 
						"ofc_tel"=>$telephone, 
						"ofc_email"=>$email,
					]);

				$select = $db->select("SELECT TOP 1 ofc_id FROM of_customer ORDER BY ofc_id DESC");
				extract($select[0][0]);

				$insert = $db->insert("contract", ["contract_start_date"=>$tstart_date, "contract_end_date"=>$tend_date, "contract_ofc_id"=>$ofc_id, "contract_date_added"=>time(), "contract_added_by"=>user_id()]);
								
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
					Add Optic Fibre Customer Form
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
												<input class="form-control" type="text" name="customer_name" placeholder="" value="<?php echo @$customer_name; ?>" autocomplete="off">
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
													<div class="col-md-6">
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


	public function editCustomerDetailsAction(){
		
		$id = portion(3);

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
			$postal_code = $_POST['postal_code'];
			$street = $_POST['street'];
			$email = $_POST['email'];
			$telephone = $_POST['telephone'];
			$address = trim($_POST['address']);
			
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
			
			if(empty($customer_name)){
				$errors[]="Enter Customer Name";
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

			if($this->isThereEdit("of_customer", ["ofc_customer_name"=>$customer_name, "ofc_id"=>$id])){
				$errors[]="Customer Name($customer_name) already exists";
			}
			
			if(empty($errors)){
				$db = new Db();
				$x = $db->update("of_customer",[
					"ofc_customer_name"=>$customer_name, 
					"ofc_address"=>$address, 
					"ofc_added_by"=>user_id(), 
					"ofc_date_added"=>time(), 
					"ofc_street"=>$street, 
					"ofc_postal_code"=>$postal_code, 
					"ofc_email"=>$email, 
					"ofc_tel"=>$telephone, 
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
									<div class="col-md-12">
										<div class="form-group">
											<label>Customer Name<span class="must">*</span></label>
											<div class="form-line">
												<input class="form-control" type="text" name="customer_name" placeholder="" value="<?php echo @$ofc_customer_name; ?>" autocomplete="off">
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
													<div class="col-md-6">
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
				echo '<button class="pull-right btn btn-danger btn-lg">Total Raised Invoice: '.$this->notifyAll().'</button>';
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
				echo '<th>Contract Period</th>';
				echo '<th>Services</th>';
				echo '<th width="100px">Raised Invoice</th>';
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
					if($t){
						if($this->notify($ofc_id))
							echo '<tr valign="top" style="background-color:#ecc2c2;">';
						else
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
							echo ' &nbsp; '.$this->action('view','optic-fibre-customers/optic-fibre-customer-details/'.$ofc_id, 'View Details');
						}
						if($access->sectionAccess(user_id(), $this->page, 'A')){
							echo ' &nbsp; '.$this->action('edit','optic-fibre-customers/edit-customer-details/'.$ofc_id, 'Edit Details');
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


	public function AllOpticFibreInvoicesAction(){

	$access = new AccessRights();
	?>
		<div class="col-md-12">
			<?php 
			
			if($this->notifyAll()){
				echo '<button class="pull-right btn btn-danger btn-lg">Total Raised Invoice: '.$this->notifyAll().'</button>';
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
				echo '<th>Contract Period</th>';
				echo '<th>Services</th>';
				echo '<th width="100px">Raised Invoice</th>';
				if($access->sectionAccess(user_id(), $this->page, 'E') || $access->sectionAccess(user_id(), $this->page, 'D')){
					echo '<th width="">Action</th>';
				}
				echo '</tr>';
				echo '</thead>';
				
					
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
							echo ' &nbsp; '.$this->action('view','optic-fibre-customers/optic-fibre-customer-details/'.$ofc_id, 'View Details');
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
				echo '<button class="pull-right btn btn-danger btn-lg">Total Raised Invoice: '.$this->notifyAll().'</button>';
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

	public function opticFibreCustomerDetailsAction(){


		$id = portion(3);

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

		echo '<tr>';
		echo '<td><b>Contract Period:</b></td>';
		echo '<td>';
		$b = new Db();
		$bb = $b->select("SELECT * FROM contract WHERE contract_ofc_id = '$ofc_id'");
		if($b->num_rows()){
			extract($bb[0][0]);
			echo Feedback::date_s($contract_start_date);
			echo ' - ';
			echo Feedback::date_s($contract_end_date);
			echo '<br/>';
			echo Feedback::status2($contract_start_date, $contract_end_date, $ofc_id);
		}
		echo '</td>';
		echo '</tr>';
		
		echo '</table>';

		//if($this->notify($ofc_id, 0) <= 1)
		{
			echo '<a class="right" href="'.return_url().'optic-fibre-customers/add-optic-fibre-service/'.$id.'"><button class="btn btn-success">Add Optic Fibre Service</button></a>';			
		}

			echo '<a class="left" href="'.return_url().'optic-fibre-customers/edit-customer-details/'.$id.'"><button class="btn btn-primary">Edit Customer Details</button></a>';
		
		echo '<br/><br/>';
		echo '<table id="table" border="1">';

		echo '<tr>';
		echo '<th width="30px">No</th>';
		echo '<th>Effective Date</th>';
		echo '<th>Description</th>';
		echo '<th>Qty</th>';
		echo '<th>Rate</th>';
		echo '<th width="100px">Action</th>';
		echo '</tr>';
		$no = 1;
		$db = new Db();
		$select = $db->select("SELECT * FROM fibre_service WHERE fs_customer_id = '$id'");
		foreach($select[0] as $row){
			extract($row);
			echo '<tr valign="top">';
			echo '<td>'.($no++).'</td>';
			echo '<td>'.FeedBack::date_s($fs_effective_date).'</td>';
			echo '<td>'.nl2br($fs_description).'</td>';
			echo '<td>'.($fs_qty).' months</td>';
			echo '<td>'.number_format($fs_rate).'</td>';
			echo '<td><a href="'.return_url().'optic-fibre-customers/delete-customer-service/'.$id.'/'.$fs_id.'" class="btn btn-danger btn-xs"><i class="fa fa-fw fa-times"></i> Delete</a></td>';
			echo '</tr>';
		}

		echo '</table>';
		$all_dates = array();
		$start_dates = array();
		if($db->num_rows())
		{
			while($fs_effective_date < time()){
				$x = $this->invoiceCreator($fs_effective_date, $fs_qty);
				$fs_effective_date = strtotime($x['next_start']);
				$all_dates[] = $x['start_date'].' - '.$x['end_date'];
				$start_dates[] = $x['start_date'];
			}
		}

		$sd = $this->notified($id);

		//echo count($all_dates);
		echo '<br/>';

		echo '<table border="1" id="table">';
		echo '<tr>';
		echo '<th width="30px">No.</th>';
		echo '<th>Effective Date</th>';
		echo '<th>Qty</th>';
		echo '<th>Units</th>';
		echo '<th>Rate</th>';
		echo '<th>Total (VAT Ex)</th>';
		echo '<th>VAT</th>';
		echo '<th>Total (VAT In)</th>';
		echo '<th>Action</th>';
		echo '</tr>';
		$total = 0;
		$vat = $total_vat = 0;
		for($i=0; $i<count($all_dates); $i++){
			$select = $db->select("SELECT * FROM fibre_service WHERE fs_customer_id = '$id'");
			$tot = $db->num_rows();

			//echo $start_dates[$i].'<br/>';

			$k=0;
			foreach($select[0] as $row){
				extract($row);
				//greater than
				if(!in_array(strtotime($start_dates[$i]), $sd) && strtotime($start_dates[$i]) > strtotime('2020-01-01')){
					echo '<tr valign="top" style="background-color:#ecc2c2;">';
				}else{
					echo '<tr valign="top">';
				}
				$k++;
				if($k==1){
					echo '<td rowspan="'.$tot.'">'.($i+1).'.</td>';
					echo '<td rowspan="'.$tot.'">'.$all_dates[$i].'</td>';
					echo '<td rowspan="'.$tot.'">'.$fs_qty.'</td>';
					echo '<td rowspan="'.$tot.'">Months</td>';
				}
				
				
				echo '<td align="right">'.number_format($fs_rate).'</td>';
				echo '<td align="right">'.number_format($fs_rate*$fs_qty).' $</td>';
				$total += $fs_rate*$fs_qty;
				echo '<td align="right">'.number_format($fs_rate*$fs_qty*18/100).' $</td>';
				$vat += $fs_rate*$fs_qty*18/100;
				echo '<td align="right">'.number_format($fs_rate*$fs_qty*1.18).' $</td>';
				$total_vat += $fs_rate*$fs_qty+$fs_rate*$fs_qty*18/100;

				if($k==1){
					echo '<td rowspan="'.$tot.'"> &nbsp; <a href="'.return_url().'optic-fibre-customers/invoice/'.$id.'/'.strtotime($start_dates[$i]).'">Invoice</a></td>';
				}
				
				echo '</tr>';
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
		$period = $sdate = portion(4);
		$db = new Db();
		if(isset($_POST['show'])){
			$customer = $_POST['customer'];
			Feedback::redirect(return_url().'optic-fibre-customers/invoice/'.$customer);
		}

		echo '<form class="search" action="" method="post">';
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
		
		if($db->num_rows())
		{
			extract($select[0][0]);	

			$sd = $this->notified($id);

			

			if(!in_array($period, $sd)){
				$db = new Db();
				$insert = $db->insert("notification", [
					"not_start_date"=>$period,
					"not_customer_id"=>$id,
					"not_date_added"=>time(),
					"not_added_by"=>user_id(),
				]);
			}
		}
		
		$select = $db->select("SELECT not_id, not_date_added FROM notification WHERE not_customer_id = '$id' AND not_start_date = '$period'");
		extract($select[0][0]);		
		$ni = $not_id;
		echo '<div class="clearfix"></div>';
		//$b = 2019;
		$db = new Db();
		$select = $db->select("SELECT * FROM notification WHERE not_customer_id = '$id' ORDER BY not_id ASC");
		echo '<b>Invoice No.: </b>';
		foreach($select[0] as $row){
			extract($row);
			if($not_start_date == $period){
				echo '<span class="btn btn-default btn-xs">'.$not_id.'</span>&nbsp;';
			}else{
				echo '<a href="'.return_url().'optic-fibre-customers/invoice/'.$id.'/'.$not_start_date.'" class="btn btn-xs btn-primary">'.$not_id.'</a>&nbsp;';
			}
		}
		echo '<div class="clearfix" style="margin-bottom:10px;"></div>';

		echo '<div class="nav files pull-right">';
		
		echo '<a class="btn btn-success btn-xs" href="" onclick="return print(\'printMe\');"><i class="fa fa-print fa-fw"></i> Print Invoice</a>	
		</div>';
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
						
						<p align="right" style="font-weight: bold; color:red;">Tax Invoice No.: <?php echo str_pad($ni, 5, '0', STR_PAD_LEFT).' / '.date('Y'); ?></p>
						<?php

						$db = new Db();
						$select = $db->select("SELECT * FROM of_customer WHERE ofc_id = '$id'");
						extract($select[0][0]);

						echo '<table border="0" style="width:100%;">';
						echo '<tr valign="middle">';
						echo '<td width="150px">Date: </td>';
						echo '<td>'.FeedBack::date_s($not_date_added).'</td>';
						echo '<td rowspan="4" align="right">';
							
						$g = new Db();
						$gg = $g->select("SELECT * FROM contract WHERE contract_ofc_id = '$id'");
						if($g->num_rows()){
							extract($gg[0][0]);
							echo '<div style="color:blue; padding:10px;">';
							echo '<b>Contract Period</b><br/>';
							echo Feedback::date_c($contract_start_date);
							echo ' - ';
							echo Feedback::date_c($contract_end_date);
							echo '<br/>';

							//echo Feedback::status($contract_start_date, $contract_end_date, $id);
							echo '</div>';
						}
						echo ' &nbsp;';
						echo '</td>';
						echo '</tr>';
						echo '<tr><td></td><td><br/></td></tr>';

						echo '<tr valign="top">';
						echo '<td>Customer: </td>';
						echo '<td><b>'.strtoupper($ofc_customer_name).'</b>'.'</p></td>';
						echo '</tr>';

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

						echo '<br/><style>#table td{padding:2px 10px;}</style>';
						
						echo '<table border="1" id="table" cellspacing="0" style="width:100%;font-size: 12px; font-family: arial;">';
						echo '<tr>';
						echo '<th>Period</th>';
						echo '<th>Description of Service Rendered</td>';
						echo '<th>Quantity</th>';
						echo '<th>Rate</th>';
						echo '<th>Amount</th>';
						echo '</tr>';
						$db = new Db();
						$select = $db->select("SELECT * FROM fibre_service WHERE fs_customer_id = '$id' ");
						$period = FeedBack::date_r($not_date_added);
						$total = 0;
						$i = 0;
						$all = $db->num_rows();
						foreach($select[0] as $row){
							extract($row);

							$x = $this->invoiceCreator($sdate, $fs_qty);
							$ef_da = "";
							$i++;

							if($i == $all){
								$ef_da = ($id == 1060)? '<div style="margin:4pt;"></div>':"<br/><br/>";
								$ef_da .= "<b>Effective Date: ".FeedBack::date_r(strtotime($x['start_date']))."<br/>Expiry Date: ".FeedBack::date_r(strtotime($x['end_date'])).'</b>';
							}
							echo '<tr style="';
							if($all == 1) 
								echo 'height: 180px';
							if($all == 2) 
								echo 'height: 90px';
							echo '" valign="top">';
							echo '<td>'.$period.'</td>';
							echo '<td width="30%"><b>'.$fs_description.'</b><br/>'.$this->rgf("outlet", $fs_outlet, "outlet_id", "outlet_name").'<br/>'.$fs_desc.'<br/>'.$ef_da.'</td>';
							echo '<td>'.$fs_qty.' months</td>';
							echo '<td>'.number_format($fs_rate).' $</td>';
							$total += $fs_qty*$fs_rate;
							echo '<td align="right">'.number_format($fs_qty*$fs_rate).' $</td>';
							echo '</tr>';
						}

						echo '<tr>';
						echo '<td colspan="4"><b>Total before VAT</b></td>';
						echo '<td align="right"><b>'.number_format($total).' $</b></td>';
						echo '</tr>';

						echo '<tr>';
						echo '<td colspan="4"><b>VAT</b></td>';
						echo '<td align="right"><b>'.number_format($total*0.18).' $</b></td>';
						echo '</tr>';

						echo '<tr>';
						echo '<td colspan="4"><b>Total Amount incl. VAT</b></td>';
						echo '<td align="right"><b>'.number_format($total*1.18).' $</b></td>';
						echo '</tr>';

						echo '</table>';

						echo '<br/><span style="color:blue;"><b>Total Amount: </b>'.number_format($total*1.18).' $</span>';

						echo '<br/><span style="color:blue;"><b>Total Amount (in Words): </b>'.FeedBack::number_to_words((int)($total*1.18)).' Dollars</span>';
						?>
						<br/><br/>
						
						<span style="color:red; font-weight: bold; font-size: 12px; text-align: center; ">Invoice is due within 30 days of receipt.
</span>				

						<br/><br/>
						<table style="width:100%; font-size: 12px !important; font-family: arial;">
							<tr>
								<td colspan="3">
									<p style="border-bottom:1px solid black;font-weight: bold; padding:0 auto; margin:0 auto;">APPROVALS</p>
								</td>
							</tr>
							<tr>
							<?php 
							$db = new Db();
							$select = $db->select("SELECT app_role_id FROM approval_order ORDER BY app_id ASC");
							
							echo '<div style="font-size: 12px;">';
							$tc=1;
							foreach($select[0] as $row){
								extract($row);
								echo '<td style="width:30%;padding:0 2%">';

								if($tc==1)
									echo '<b>'.($tc++).strtoupper('. Prepared By:</b><br/>');
								else
									echo '<b>'.($tc++).strtoupper('. '.$this->rgf("designation", $app_role_id, 'designation_id', 'designation_name').'</b><br/>');

								echo '<div style="padding-left:10px;">';
								echo '<b>Approved By: </b>Musiitwa Joseph<br/>';
								echo '<b>Date: </b>'.FeedBack::date_fm(time()).'<br/>';
								echo '</div>';
								echo '</td>';
							}
							echo '</div>';
							?>
							</tr>
						</table>
						<!-- Prepared by: .........................  -->
						<?php //for($i=0; $i<20; $i++) echo ' &nbsp ';  ?>
						<!-- Approved By: ......................... -->
					</td>				
				</tr>
				
			</tbody></table>
		</div>
	
	</div>
	<?php
	}

	public function approveOpticFibreInvoicesAction(){
		if(($this->notifyAll()&&user_id()==static_generator2()) || $this->toApprove()){
			echo '<div style="width:100px;margin:10px 0;" onclick="return multipleChecker(); " id="checker">Check All</div>';
		}
		echo '<form action="" method="post">';
		echo '<table border="1" id="table" cellspacing="0" cellpadding="1">';
		echo '<thead>';
		echo '<tr>';
		echo '<th width="30px">No.</th>';
		echo '<th>Date</th>';
		echo '<th>Invoice No.</th>';
		echo '<th>Customer Name</th>';
		echo '<th>Period</th>';
		echo '<th>Status</th>';
		echo '</tr>';
		echo '</thead>';

		echo '<tbody>';
		$db = new Db();
		
			
			$select = $db->select("SELECT * FROM fibre_service, of_customer WHERE ofc_id = fs_customer_id");
			$check = array();
			$cus = array();
			$count = 1;
		if(user_id() == static_generator2()){
			foreach($select[0] as $row){
				extract($row);
				$ed = $fs_effective_date;
				$q = $fs_qty;

				$sd = $this->notified($fs_customer_id);
				
				while($ed < time()){
					$p = $this->invoiceCreator($ed, $q);
					$start_date = strtotime($p['start_date']);
					$ed = strtotime($p['next_start']);

					//date greater
					if(!in_array($start_date, $sd) && $start_date > strtotime('2020-01-01'))
					{
						if(!in_array($ofc_id.$start_date, $check))
						{
							$edl = $ed - (24*60*60);
							$check[] = $ofc_id.$start_date;
							$c = '<tr>';
							//$c .= '<td>'.($count+1).'.</td>';
							$c .= '<td><input class="AllCheckBoxes AllCheckBoxesLine'.$i.'  AllCheckBoxesColumn'.$n.'" type="checkbox" style="width:20px" name="options[]" value="'.$ofc_id.'" title="'.$control.'"  id="la'.$count.'"><label for="la'.$count.'"></label></td>';
							$c .= '<td align="center"> - </td>';
							$c .= '<td align="center"> - </td>';
							$c .= '<td><a href="'.return_url().'optic-fibre-customers/invoice/'.$ofc_id.'/'.$start_date.'">'.$this->rgf("of_customer", $ofc_id, "ofc_id", "ofc_customer_name").'</a></td>';
							$c .= '<td>'.Feedback::date_s($start_date).' - '.Feedback::date_s($edl).'</td>';
							$c .= '<td><a href="" class="btn btn-xs btn-danger">Pending</a></td>';
							$c .= '</tr>';

							echo $c;
							$count++;
							$cus[$ofc_id] = array(
								'start_date'=>$start_date,
								'end_date'=>$edl,
							);
						}
					}
				}
			}
		}

		$count=1;
		$select = $db->select("SELECT * FROM notification ORDER BY not_id DESC");
		foreach($select[0] as $row){
			extract($row);
			$l = $this->isApproval(user_id(), $not_id);
			$deg = $this->level($not_customer_id,$not_start_date, $l, $last=0);
			$approve = false;
			if($deg == $this->rgf("sysuser", user_id(), "user_id", "user_designation")){
				if(empty(${"not_app".$l})){
					$approve = true;
				}
			}
			echo '<tr>';
			if($approve){
				echo '<td><input class="AllCheckBoxes AllCheckBoxesLine'.$i.'  AllCheckBoxesColumn'.$n.'" type="checkbox" style="width:20px" name="options[]" value="'.$not_customer_id.'" title="'.$control.'"  id="la'.$count.'"><label for="la'.$count.'"></label></td>';
				$cus[$not_customer_id] = array(
					'start_date'=>$not_start_date,
					'end_date'=>$not_end_date,
				);
			}else{
				echo '<td>'.($count++).'.</td>';
			}
			echo '<td>'.Feedback::date_fm($not_date_added).'</td>';
			echo '<td>'.$not_id.'</td>';
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
				$status = "Pending ".$this->rgf("designation", ${"not_selected".($k+1)}, "designation_id", "designation_name")."'s Approval(".$k."/3)";
			}else{
				$status = "Pending Submission for Approvals";
			}

			echo '<td>'.$status.'</td>';
			echo '</tr>';
		}

		echo '</tbody>';
		echo '</table>';

		if(($this->notifyAll()&&user_id()==static_generator2()) || $this->toApprove()){
			$this->opticApprovals(user_id(), $ms, $cus);
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
		$select = $db->select("SELECT * FROM notification ORDER BY not_id DESC");
		$approve=0;
		foreach($select[0] as $row){
			extract($row);
			$l = $this->isApproval(user_id(), $not_id);
			$deg = $this->level($not_customer_id,$not_start_date, $l, $last=0);
			if($deg == $this->rgf("sysuser", user_id(), "user_id", "user_designation")){
				if(empty(${"not_app".$l})){
					$approve++;
				}
			}
		}
		return $approve;
	}

	public function opticApprovals($user_id, $ms="", $cus=array()){
		//echo $this->rgf("sysuser", $user_id, "user_id", "user_designation");
		$db = new Db();
		$select = $db->select("SELECT app_role_id FROM approval_order ORDER BY app_id ASC");
		
		echo '<div style="font-size: 12px;">';
		$tc=1;
		echo '<h2 style="border-bottom:1px solid #000; margin-top:15px; font-size: 16px;">Approvals:</h2>';
		echo '<ol>';
		foreach($select[0] as $row){
			extract($row);
			echo '<div style="width:29%;padding:0 2%; float:left;">';

			if($tc==1)
				echo '<li><b>'.strtoupper('Prepared By:</b></li>');
			else
				echo '<li><b>'.strtoupper('Approved By:</b></li>');

			$d = new Db();

			foreach($cus as $cu){
				$custo = $cu[0];
				if($tc == 1){
					$sel = $d->select("SELECT * FROM notification WHERE not_start_date = '' AND not_customer_id = '$custo' AND not_app$tc = 1");
				}else{
					$tcn = $tc-1;
					$sel = $d->select("SELECT * FROM notification WHERE not_start_date = '' AND not_customer_id = '$custo' AND not_app$tc = 1 AND not_app$tcn = 1");
				}
			}

			if($d->num_rows()){
				$dd = new Db();
				
				if($invoice){

					echo '<br/><br/>................................<br/>';

					$sele = $dd->select("SELECT arc_added_by FROM all_readings_comments WHERE arc_year = '$year' AND arc_month = '$month' AND arc_part = '$tc' ");

					if($dd->num_rows()){
						extract($sele[0][0]);
					}

					echo '<b style="font-size:8pt">'.ucwords(strtolower(''.$this->rgf("designation", $this->rgf("sysuser", $arc_added_by, "user_id", "user_designation"), 'designation_id', 'designation_name').'</b>'));					

				}else{
				
					$sele = $dd->select("SELECT arc_added_by FROM all_readings_comments WHERE arc_year = '$year' AND arc_month = '$month' AND arc_part = '$tc' ");

					if($dd->num_rows()){
						extract($sele[0][0]);
					}

					echo '<b>'.strtoupper(''.$this->rgf("designation", $this->rgf("sysuser", $arc_added_by, "user_id", "user_designation"), 'designation_id', 'designation_name').'</b><br/>');

					extract($sel[0][0]);
					
					echo ''.$this->full_name($dl_added_by).'	';
					
					$sel = $d->select("SELECT * FROM all_readings_comments WHERE arc_year = '$year' AND arc_month = '$month' AND arc_part = '$tc' ");
					if($d->num_rows()){
						echo '<br/><b>Comment: </b><br/>';
					}
					foreach($sel[0] as $row){
						extract($row);
						echo nl2br($arc_description);
						echo '<br/><b>Date: </b>'.FeedBack::date_s(time()).'<br/>';
					}
				}
			}else{
				if($tc == 1){
					$tot = 1;
				}else{
					$tcn = $tc-1;
				
					$tcn = $tc-1;
					$user_designation = $this->rgf("sysuser", $user_id, "user_id", "user_designation");
					$sql = "SELECT * FROM notification WHERE not_selected$tc = $user_designation AND not_app$tc IS NULL";
					$sel = $d->select($sql);
					$tot = $d->num_rows();
				}

				if($tot){
					extract($sel[0][0]);
					if($invoice == 0 && (($tc == 1 && user_id() == static_generator2())|| ${"not_selected".$tc} == $this->rgf("sysuser", user_id(), "user_id", "user_designation"))){					
				
						echo '<div style="background-color: #fbe2e2;padding:10px; border:1px solid #000; ">';
						

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
							
							// $com = array();
							// $com["arc_year"] = $year;
							// $com["arc_month"] = $month;
							// $com["arc_description"] = $comment;
							// $com["arc_date_added"] = time();
							// $com["arc_added_by"] = user_id();
							// $com["arc_part"] = $part;


							// $insert = $d->insert("all_readings_comments", $com);
							echo $d->error();
							$link = return_url().'optic-fibre-customers/approve-optic-fibre-invoices';
							
							if($tc==1){

								$approveOne = $_POST["approveOne"];
								$approveTwo = $_POST["approveTwo"];
								$msg = "";
								foreach($options as $cu){
									$ind['not_start_date'] = $cus[$cu]['start_date'];
									$ind['not_end_date'] = $cus[$cu]['end_date'];
									$ind['not_date_added'] = time();
									$ind['not_added_by'] = user_id();
									$ind['not_selected1'] = $this->rgf("sysuser", user_id(), "user_id", "user_designation");
									$ind['not_selected2'] = $approveOne;
									$ind['not_selected3'] = $approveTwo;
									$ind['not_customer_id'] = $cu;
									$insert = $d->insert("notification", $ind);
									echo $d->error();

									$period = Feedback::date_s($cus[$cu]['start_date']);
									$period .= " - ".Feedback::date_s($cus[$cu]['end_date']);

									$msg .= '<li><a href="'.return_url().'optic-fibre-customers/invoice/'.$cu.'/'.$cus[$cu]['start_date'].'">'.$this->rgf("of_customer", $cu, "ofc_id", "ofc_customer_name").'</a> &nbsp; ('.$period.')</li>';
								}
								
								$next = $this->next($tc, $cu, $cus[$cu]['start_date']);
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
								$next = $this->next($tc, $cu, $cus[$cu]['start_date']);
								print_r($next);
								$to = ($next['email'])?$next['email']:"josemusiitwa@gmail.com";
								$message = "Hello ".$next['fname'].",";

								if($tc == 2){
									$msg = "";
									foreach($options as $cu){
										$z = $cu;
										$x = $cus[$cu]['start_date'];
										$y = $cus[$cu]['end_date'];
										$z = "SELECT not_id FROM notification WHERE not_customer_id = '$z' AND not_start_date = '$x' AND not_end_date = '$y'";
										$select = $d->select($z);
										extract($select[0][0]);
										$ind['not_app'.$tc] = 1;

										$not_id;
										$insert = $d->update("notification", $ind, ["not_id"=>$not_id]);
										$period = Feedback::date_s($x);
										$period .= " - ".Feedback::date_s($y);
										$msg .= '<li><a href="'.return_url().'optic-fibre-customers/invoice/'.$cu.'/'.$cus[$cu]['start_date'].'">'.$this->rgf("of_customer", $cu, "ofc_id", "ofc_customer_name").'</a> &nbsp; ('.$period.')</li>';
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
										$x = $cus[$cu]['start_date'];
										$y = $cus[$cu]['end_date'];
										$z = "SELECT not_id FROM notification WHERE not_customer_id = '$z' AND not_start_date = '$x' AND not_end_date = '$y'";
										$select = $d->select($z);
										extract($select[0][0]);
										$ind['not_app'.$tc] = 1;

										$not_id;
										$insert = $d->update("notification", $ind, ["not_id"=>$not_id]);
										$period = Feedback::date_s($x);
										$period .= " - ".Feedback::date_s($y);
										$msg .= '<li><a href="'.return_url().'optic-fibre-customers/invoice/'.$cu.'/'.$cus[$cu]['start_date'].'">'.$this->rgf("of_customer", $cu, "ofc_id", "ofc_customer_name").'</a> &nbsp; ('.$period.')</li>';
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

							FeedBack::redirect(return_url().portion(1)."/".portion(2));
						}

						echo 'Comment:<br/><textarea name="comment" style="width: 100%;"></textarea>';
						echo '<input type="hidden" name="part" value="'.$tc.'"/>';

						if($tc==1){	



							$app = array("One", "Two");
							foreach($app as $ap){
								echo '<br/>Select Approve '.$ap.':';
								echo '<select required name="approve'.$ap.'">';
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
							echo '<button name="approve"><i class="fa fa-fw fa-check"></i> Forward</button>';
						}else{
							echo '<button name="approve"><i class="fa fa-fw fa-check"></i> Approve</button>';		
							echo '<input type="hidden" name="parc_id" value="'.$dl_id.'"/>';
						}

						echo '</div>';

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
															echo '<option value="'.$service_name.'">'.$service_name.'</option>';
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
			$errors = array();
			if(empty($service)){	
				$errors[] = "Enter Service";
			}

			if(empty($errors)){
				$db = new Db();
				$insert = $db->insert("services_fibre", [
					"service_name"=>$service,
					"service_date_added"=>time(),
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

						 			$db->update("services_fibre", [
						 				"service_name"=>$service_name,
						 			], ["service_id"=>$service_id]);

						 			Feedback::redirect(return_url().'optic-fibre-customers/services');
						 		}
						 		?>
						    	<span id="close<?php echo $co; ?>" class="close" style="color:red;">&times;</span>
						    	<h4>Edit Service</h4>
						    	<textarea required class="form-control" style="height:100px;" name="service_name"><?php echo $service_name; ?></textarea>
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
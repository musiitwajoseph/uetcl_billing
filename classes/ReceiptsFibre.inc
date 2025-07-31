<?php
Class ReceiptsFibre extends BeforeAndAfter{
	public $page = "Receipts";
	
	public function __construct(){
		$access = new AccessRights();
		$access->pageAccess(user_id(), $this->page, 'V');
	}
	
	public function getLinks(){
		$page = "Receipts";
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
			)
		);
		
		return $links;
	}

	public function receiptNo(){
		$db = new Db();
		$select = $db->select("SELECT TOP 1 rec_receipt_no FROM receipts2 ORDER BY rec_id DESC");
		extract($select[0][0]);
		$r = str_replace('REC','', $rec_receipt_no);
		$r += 1;
		return 'REC'.str_pad($r, 5, "0", STR_PAD_LEFT);
	}
	
	public function addReceiptAction(){

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

			if($this->isThere("receipts2", ["rec_receipt_no"=>$receipt_no])){
				$errors[]="Receipt No.($receipt_no) already exists";
			}
			
			if(empty($errors)){
				$db = new Db();
				$am = str_replace(',', '', $amount);
				$cus =  $this->rgf("customer", $customer_name, "customer_id", "customer_short_name");
				$x = $db->insert("receipts",["rec_date_added"=>$time, "rec_added_by"=>user_id(),"rec_category"=>$category, "rec_receipt_no"=>$receipt_no.'-'.$cus, "rec_amount"=>$am, "rec_customer_id"=>$customer_name, "rec_description"=>$description, "rec_date"=>strtotime($date)]);
								
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
													<option value="2">Optice Fibre</option>
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
	
	public function editReceiptAction(){
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
	

	public function viewReceiptsAction(){

		echo '<a href="'.return_url().'receipts/add-receipt"><i class="fa fa-fw fa-plus"></i>Add Receipt</a><div style="margin-bottom:20px;"></div>';
		
		if(isset($_POST['show'])){
			$filter_year = $_POST['filter_year'];
			$filter_month = $_POST['filter_month'];	
			$customer = $_POST['customer'];	

			Feedback::redirect(return_url().'receipts/view-receipts/'.$customer.'/'.$filter_year.'/'.$filter_month);		
		}else{
			if(portion(3)){
				$customer = portion(3);
			}else{
				$customer = "all";
			}

			if(portion(4)){
				$filter_year = portion(4);
			}else{
				$filter_year = date('Y');
			}

			if(portion(5)){
				$filter_month = portion(5);
			}else{
				$filter_month = date('m')-1;
			}
		}




		$customers = array();
        $db = new Db();
        $select = $db->select("SELECT * FROM customer ORDER BY customer_short_name ASC");
        foreach($select[0] as $row){
            extract($row);
            $customers[$customer_id] = $customer_short_name;
        }
		?>
		<form action="" method="post" class="search">
			<div class="pull-left" style="margin:10px;"> <b>Select: </b><br/>
			</div>
			<div class="pull-left" style="margin:10px;"> <b>Year </b><br/>  
				<select name="filter_year" style="width:80px; margin: auto;">
					<option>Select</option>
					<?php 
					if(empty($filter_year)){
						$filter_year = date('Y');
					}
					for($i=2019; $i<=date('Y'); $i++){
						if($i == $filter_year)
							echo '<option value="'.$i.'" selected="selected">'.($i).'</option>';
						elseif($i == $year_uploaded)
							echo '<option value="'.$i.'" selected="selected">'.($i).'</option>';
						else
							echo '<option value="'.$i.'">'.($i).'</option>';
					}
					?>
				</select>
			</div>
			<div class="pull-left" style="margin:10px;"> <b>Month </b><br/>  
				<select name="filter_month" style="width:80px; margin: auto;">
					<option value="all">All</option>
					<?php 
					if(empty($filter_month)){
						$filter_month = date('m')-1;
					}
					for($i=1; $i<=12; $i++){
						if($i == $filter_month)
							echo '<option value="'.$i.'" selected="selected">'.month_name($i).'</option>';
						else
							echo '<option value="'.$i.'">'.month_name($i).'</option>';
					}
					?>
				</select>
			</div>
			<div class="pull-left" style="margin:10px;"> <b>Customer </b><br/>  
				<select name="customer" style=" margin: auto;">
					<option value="all">All</option>
					<?php
					$i=0;
					foreach($customers as $id => $name){
						if($id == $customer)
							echo '<option value="'.$id.'" selected="selected">'.($name).'</option>';
						else
							echo '<option value="'.$id.'">'.($name).'</option>';
					}
					?>
				</select>
			</div>
			<div class="pull-left"  style="margin:10px;">
				&nbsp; <br/>
				<button name="show">Show</button>
			</div>
			<div class="clearfix"></div>
		</form>

		<?php
		if($filter_month == "all"){
			$month = " ";
		}else{
			$month = " ".month_name($filter_month)." ";
		}
		echo '<br/>';
		if($customer == "all"){
			$label = "ALL CUSTOMER RECEIPTS FOR ".$filter_year.$month;

			$from = strtotime(($filter_year.'-01-01 12:00:00am'));
			$to = strtotime($filter_year.'-12-'.date('t', $from).' 11:59:59 pm');

		}else{
			$label = $this->rgf("customer", $customer, "customer_id", "customer_full_name")." ACCOUNT STATEMENT FOR ".$filter_year.$month;			
		
			$from = strtotime(($filter_year.'-'.$filter_month.'-01 12:00:00am'));
			$to = strtotime($filter_year.'-'.$filter_month.'-'.date('t', $from).' 11:59:59 pm');
		}


		// echo date('Y-m-d h:i:s a', $from);
		// echo ' = ';
		// echo date('Y-m-d h:i:s a', $to);

		echo strtoupper("<h4>$label</h4>");
		echo '<table border="1" cellspacing="0" cellpadding="2" id="table">';

		echo '<tr>';
		echo '<th>No.</th>';
		echo '<th>Receipt Date</th>';
		echo '<th>Receipt No.</th>';
		echo '<th>Customer</th>';
		echo '<th>Category</th>';
		echo '<th>Amount</th>';
		echo '<th>Action</th>';

		$db = new Db();
		$sql = "SELECT * FROM receipts WHERE rec_date >= '$from' AND rec_date <= '$to' ORDER BY rec_receipt_no ASC";
		$select = $db->select($sql);
		$no = 1;
		$total_amount = 0;
		foreach($select[0] as $row){
			extract($row);
			echo '<tr>';
			echo '<td width="30px">'.($no++).'.</td>';
			echo '<td>'.FeedBack::date_s($rec_date_added).'</td>';
			echo '<td>'.($rec_receipt_no).'</td>';
			echo '<td>'.$this->rgf("customer", $rec_customer_id, "customer_id", "customer_short_name").'</td>';
			echo '<td>Energy Sales</td>';
			echo '<td class="text-right">'.number_format($rec_amount, 1).'</td>';
			echo '<td class="text-right"><a href="'.return_url().'receipts/delete-receipt/'.$rec_id.'" class="btn btn-danger btn-xs">Delete</a> <a href="'.return_url().'receipts/edit-receipt/'.$rec_id.'" class="btn btn-success btn-xs">Edit</a></td>';
			echo '</tr>';
			$total_amount += $rec_amount;
		}

		echo '<tr>';
		echo '<th colspan="5">Total</th>';
		echo '<th class="text-right" style="text-align:right">'.number_format($total_amount).'</th>';		
		echo '<th>&nbsp;</th>';
		echo '</tr>';

		echo '</table>';
	}

	public function deleteReceiptAction(){
		$id = portion(3);
		$this->deletor("receipts", "rec_id",  $id, 'receipts/view-receipts');
	}

}
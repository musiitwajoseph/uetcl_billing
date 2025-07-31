<?php
Class Services extends BeforeAndAfter{
	public $page = "OFFICE";

	public $set_class = "";
	public $set_method = "";
	public $set_customer = "";
	public $set_year = "";
	public $set_month = "";
	public $set_note = "";

	public function parameters($href){
		$portion = explode('/',str_replace(return_url(), '', $href));

		$this->set_class = $portion[0];
		$this->set_method = $portion[1];
		$this->set_customer = $portion[2];
		$this->set_year = $portion[3];
		$this->set_month = $portion[4];
		$this->set_note = $portion[5];
	}

	public function deleteBranchAction(){		
		$id = portion(3);
		$this->deletor("branch", "branch_id",  $id, 'branches/all-branches');
	}

	public function deleteSectionAction(){
		$id = portion(3);
		$this->deletor("section", "section_id",  $id, 'department/all-sections');
	}
	public function getLinks(){
		$links = array(
			array(
				"link_name"=>"Add Department", 
				"link_address"=>"department/add-department",
				"link_icon"=>"fa-plus",
				"link_page"=>$page,
				"link_right"=>"A",
			),
			array(
				"link_name"=>"View Departments", 
				"link_address"=>"department/all-departments",
				"link_icon"=>"fa-eye",
				"link_page"=>$page,
				"link_right"=>"V",
			)
		);
		
		return $links;
	}

	public function firstCustomer(){
		$db = new Db();
		$select = $db->select("SELECT TOP 1 customer_id FROM customer ORDER BY customer_short_name ASC");
		extract($select[0][0]);
		$date = strtotime('-1 month', time());
		return array($customer_id, date('Y', $date), date('m', $date));
	}
		
	public function AddCustomerServiceAction(){
		
		?>
		<div id="side">
		<div class="col-md-4">
			<form id="form" action="" method="post" style="background-color: #f9f9f9; border:1px solid black; margin-top:20px; padding:10px; ">
				<div id="form_title" style="font-weight:bold">Service Form</div><br/>
				<div style="padding:10px;">
					<?php 
					if(isset($_POST['save'])){
						$service_name = $_POST['service_name'];
						$service_type = $_POST['service_type'];
						$error = array();
						
						if(empty($service_type)){
							$error[] = "Service Type should not be empty.";
						}
						if(empty($service_name)){
							$error[] = "Service Name should not be empty.";
						}
												
						
						$time = time();
						$user_id = user_id();
						if(empty($error)){
							$db = new Db();
							$insert = $db->insert("service", 
							[
								"service_type_id"=>$service_type,
								"service_name"=>$service_name,
								"service_added_by"=>user_id(),
								"service_date_added"=>time()
							]
							);
							
							if(empty($db->error())){
								Feedback::success();
								Feedback::refresh();
							}else{
								FeedBack::errors($db->error());
							}
						}else{
							FeedBack::errors($error);
						}
						echo mysqli_error($con);
					}
					?>
				
					<p>Service Type</p>
					
					<select name="service_type" class="form-control">
						<option value="">Select</option>
						<option value="1">Energy</option>
						<option value="2">Optic Fibre</option>
					</select>
					<br/>
					<p>Service Name</p>
					<input type="text" class="form-control" name="service_name" value="<?php echo @$service_name; ?>"/><br/><br/>
					
					<button class="btn btn-success btn-block" name="save"><i class="fa fa-save fa-fx"></i> Save</button>
				</div>
			</form>
		</div>
		<div class="col-lg-8"><br/>
			<table border="1" id="table">
				<tr>
					<th width="20px">No.</th>
					<th>Service Type</th>
					<th>Service Name</th>
					<th width="100px">Action</th>
				</tr>
				<?php 
				$no = 0;
				$db = new Db();
				$sel = $db->select("SELECT * FROM service ORDER BY service_type_id ASC, service_name ASC");
				
				foreach($sel[0] as $row){
					extract($row);
					echo '<tr>';
					echo '<td>'.(++$no).'</td>';
					echo '<td>'.$this->serviceType($service_type_id).'</td>';
					echo '<td>'.$service_name.'</td>';
					echo '<td></td>';
					echo '</tr>';
				}
				?>
				<tr>
					<th colspan="9">&nbsp;</th>
				</tr>
			</table>
		</div>
	</div>
		<?php
	}
	public function editCustomerServiceAction(){
		
	?>
	<div id="side">
		<div class="col-md-4">
			<form id="form" action="" method="post" style="background-color: #f9f9f9; border:1px solid black; margin-top:20px; padding:10px; ">
				<div id="form_title">Service Form</div>
				<div style="padding:10px;">
					<?php 
					$id = portion(3);
					$select = mysqli_query($con, "SELECT * FROM service WHERE se_id = '$id'");
					extract(mysqli_fetch_assoc($select));
					if(isset($_POST['save'])){
						$service_name = $_POST['service_name'];
						$service_number = $_POST['service_number'];
						$service_rate = $_POST['service_rate'];
						$error = array();
						
						if(empty($service_name)){
							$error[] = "Service Name should not be empty.";
						}
						
						if(empty($service_number)){
							$error[] = "Service Number should not be empty.";
						}
						
						if(empty($service_rate)){
							$error[] = "Service rate should not be empty.";
						}
						
						$time = time();
						$user_id = user_id();
						if(empty($error)){
							$insert = mysqli_query($con, "UPDATE `service` SET `service_number` = '$service_number', `service_name` = '$service_name', `service_rate` = '$service_rate' WHERE se_id = $id;");
							if($insert){
								echo '<div id="note1">Successfully Saved</div>';
								refresh(3);
							}
						}else{
							error($error);
							echo '<br/>';
						}
						echo mysqli_error($con);
					}
					?>
				
					<p>Service Number</p>
					<input type="text" name="service_number" value="<?php echo @$service_number; ?>"/>
					<br/><br/>
					<p>Service Name</p>
					<input type="text" name="service_name" value="<?php echo @$service_name; ?>"/><br/><br/>
					<p>Rate</p>
					<input type="text" name="service_rate" value="<?php echo @$service_rate; ?>"/>
					<br/><br/><br/>
					<input type="submit" value="Save" name="save" style="width:100%"/>
				</div>
			</form>
		</div>
		<div class="col-lg-8">
			<br/>
			<table border="1" id="table">
				<tr>
					<th>No.</th>
					<th>Service No.</th>
					<th>Service Name</th>
					<th>Service Rate</th>
					<th width="100px">Action</th>
				</tr>
				<?php 
				$no = 0;
				$sel = mysqli_query($con, "select * from service order by service_number asc, service_name asc");
				while($row = mysqli_fetch_assoc($sel)){
					extract($row);
					echo '<tr>';
					echo '<td>'.(++$no).'</td>';
					echo '<td>'.$service_number.'</td>';
					echo '<td>'.$service_name.'</td>';
					echo '<td align="right" style="padding-right:10px;">'.number_format($service_rate).'</td>';
					echo '<td><a href="'.return_url().'services/edit-customer-service/'.$se_id.'" class="btn btn-primary btn-xs"><i class="fa fa-fx fa-edit"></i> Edit</a>';
					echo '</tr>';
				}
				?>
				<tr>
					<th colspan="9">&nbsp;</th>
				</tr>
			</table>
		</div>
	</div>
	<?php
	}

	public function serailNumber($type){
		//$select =

		$db = new Db();
		$select = $db->select("SELECT * FROM metering_point, r_reading WHERE mp_customer_id = '$customer_id' AND rea_cus_id = '$customer_id' AND mp_id = rea_mp_id");
		foreach($select[0] as $row){
			extract($row);
			if(date('Y', $rea_date)==portion(4) && date('m', $rea_date)==portion(5)){
				$invoice_no = str_pad($rea_id, 5, '0', STR_PAD_LEFT);;	
			}
		}

		return $invoice_no;
						
	}
	
	public function uploadedSummary(){
	
		
	?>
	<div id="side">		
		<form class="" action="" method="post" enctype="multipart/form-data">
			<?php
			if(isset($_POST['import'])){
				
			}
			
			if(isset($_POST['filter'])){
				$year_uploaded = $_POST['filter_year'];
				$month_uploaded = $_POST['filter_month'];

				FeedBack::redirect(return_url().portion(1).'/'.portion(2).'/'.$year_uploaded.'/'.$month_uploaded);
			}else{

				 $month_uploaded = (portion(4))? portion(4) : date('m');
				 $year_uploaded = (portion(3))? portion(3) : date('Y');
			}
			
			?>
			
		</form>
		<br/>
		<form class="search" action="" method="post">
			<fieldset >
				<div class="pull-left" style="width:200px"> 
				<p style="margin-top:5px;font-weight:bold;font-size:18px;">Summary Filter >></p>				
				</div>
				<div class="pull-left"> Year: 
					<select name="filter_year" style="width:80px; margin: auto 10px;">
						<option>Select</option>
						<?php 
						for($i=2019; $i<=date('Y'); $i++){
							if($i == date('Y') && empty($year_uploaded))
								echo '<option value="'.$i.'" selected="selected">'.($i).'</option>';
							elseif($i == $year_uploaded)
								echo '<option value="'.$i.'" selected="selected">'.($i).'</option>';
							else
								echo '<option value="'.$i.'">'.($i).'</option>';
						}
						?>
					</select>
				</div>
				<div class="pull-left">
					Month: 
					<select name="filter_month" style="width:120px; margin: auto 10px;">
						<option>Select</option>
						<?php 
						for($i=1; $i<=12; $i++){
							if($i == date('m') && empty($month_uploaded))
								echo '<option value="'.$i.'" selected="selected">'.month_name($i).'</option>';
							elseif($i == $month_uploaded)
								echo '<option value="'.$i.'" selected="selected">'.month_name($i).'</option>';
							else
								echo '<option value="'.$i.'">'.month_name($i).'</option>';
						}
						?>
					</select>
				</div>	
				<div  class="pull-left">
					<input type="submit" value="Show" name="filter" style="width:100%; margin: auto 10px;"/>
				</div>
				<div class="clear"></div>
			</fieldset>
		</form>
		<br/>
		<?php 
		$tab .=  '<h3>Uploaded summary for Year: '.@$year_uploaded.' and Month: '.month_name(@$month_uploaded).'</h3>';
	$tab .=  '<table border="1" id="table" cellspacing="0">';
	$tab .=  '<tr valign="bottom">';

	$tab .=  '<th style="width:30px;">No.</th>';

	$tab .=  '<th width="">Customer<br/>Name</th>';

	$tab .=  '<th width="30px">Metering Points</th>';

	$tab .=  '<th width="30px">Total Uploaded</th>';

	$tab .=  '<th width="30px">Total Remaining</th>';

	$tab .=  '<th>List of <br/>remaining</th>';

$tab .=  '<th>Total<br/>Gross</th>';
$tab .=  '<th>Total<br/>Payable</th>';
$tab .=  '<th>Total<br/>Collectable</th>';

$tab .=  '<th>Total VAT<br/>Exclusive</th>';
$tab .=  '<th>Total VAT</th>';
$tab .=  '<th>Total Amount<br/>VAT Inclusive</th>';
$tab .=  '</tr>';
			
			$no = 0;
			$db = new Db();
			$select = $db->select("SELECT * FROM customer ORDER BY customer_short_name ASC");
			$tot_tis = 0;
			$tot_tis2 = 0;
			foreach($select[0] as $row){
				extract($row);

				$tab .=  '<tr>';
				$tab .=  '<td>'.(++$no).'.</td>';
				$tab .=  '<td><a class="eagle-load" href="'.return_url().'services/view-invoice/'.$customer_id.'/'.$year_uploaded.'/'.$month_uploaded.'" >'.$customer_short_name.'</a></td>';
				
				$tab .=  '<td>'; 

				$sel = $db->select("SELECT * FROM metering_point WHERE mp_customer_id = $customer_id;");
				$total_metering_points = $db->num_rows($sel);
				$tab .=  $total_metering_points;

				$tab .=  '</td>';
				
				$tab .=  '<td>';
				$db = new Db();
				$sel = $db->select("SELECT rea_mp_id, rea_date as rdd FROM r_reading WHERE rea_cus_id = $customer_id;");
				$uploads = array();
				$tot = 0;
				foreach($sel[0] as $ro){
					extract($ro);
					if(date('Y', $rdd)==$year_uploaded && date('m', $rdd)==$month_uploaded)
					{
						$uploads[] = $rea_mp_id;
						$tot++;					
					}
				}
				$total_uploaded = $tot;
				$tab .=  $total_uploaded;
				$tab .=  '</td>';
				
				$tab .=  '<td>'; 
				$tab .=  $all_remaining = $total_metering_points - $total_uploaded;
				$tab .=  '</td>';
				
				$tab .=  '<td>';
				if($total_uploaded == 0){
					$tab .=  'All';
				}else{
					$db = new Db();
					$s = $db->select("SELECT * FROM metering_point WHERE mp_customer_id = $customer_id");
					$num = number_format($db->num_rows($s));
					$count = 0;
					$al = array();
					foreach($s[0] as $row){
						extract($row);
						if(!in_array($mp_id, $uploads)){
							$al[] = '<b>'.ucwords($mp_location).'</b>';					
						}
					}
					$tab .=  implode(' , ', $al);
					//metering number
					//echo count($al);
				}
				$tab .=  '</td>';
				$tis = $this->invoiceTotalSummary($customer_id, $year_uploaded, $month_uploaded);
				
				if($customer_currency == 1){
					$tot_tis = $tot_tis + $tis[3];
				}else{
					$tot_tis2 = $tot_tis2 + $tis[3];
				}

				$tab .=  '<td style="text-align:right">'.number_format($tis[0]).'</td>';
				$tab .=  '<td style="text-align:right">'.number_format($tis[4]).'</td>';
				$tab .=  '<td style="text-align:right">'.number_format($tis[5]).'</td>';
				$tab .=  '<td style="text-align:right">'.number_format($tis[1]).'</td>';
				$tab .=  '<td style="text-align:right">'.number_format($tis[2]).'</td>';
				$tab .=  '<td style="text-align:right">'.number_format($tis[3]).'</td>';		
				
				$tab .=  '</tr>';
			}

			$tab .=  '<tr>';
			$tab .=  '<td>&nbsp;</td>';
			$tab .=  '<td colspan="10" align="right"><b>Total ('.$this->rgf("currency", 1, "currency_id", "currency_symbol").') :</b></td>';
			$tab .=  '<td style="text-align:right;"><b>'.number_format($tot_tis).'</b></td>';
			$tab .=  '</tr>';

			$tab .=  '<tr>';
			$tab .=  '<td>&nbsp;</td>';
			$tab .=  '<td colspan="10" align="right"><b>Total ('.$this->rgf("currency", 2, "currency_id", "currency_symbol").') :</b></td>';
			$tab .=  '<td style="text-align:right;"><b>'.number_format($tot_tis2).'</b></td>';
			$tab .=  '</tr>';
			$tab .=  '</table>';
			
			echo $tab;
			?>
			
		
	</div>
	<?php
	}

	public function anyCustomerReading($year_uploaded, $month_uploaded){
		$db = new Db();
		$select = $db->select("SELECT customer_id FROM customer");
		foreach($select[0] as $row){
			extract($row);
			if($this->readingExist($customer_id, $year_uploaded, $month_uploaded)){
				return true;
			}
		}

		return false;
	}

	public function unloaded_readings($cus_id, $date="", $metering_point=0){
    	$total = 6;
	    $db = new Db();
	    
	    if(empty($date)){
	        //sdafaf
	        $sel = $db->select("SELECT TOP 1 rea_id, rea_date FROM r_reading WHERE rea_cus_id = $cus_id AND rea_mp_id = '$metering_point' ORDER BY rea_date DESC");
	        extract($sel[0][0]);
	    }else{
	        $year = date('Y', @$date);
	        $month = date('m', @$date);

	        $sel = $db->select("SELECT rea_id as x, rea_date as y FROM r_reading WHERE rea_cus_id = $cus_id  AND rea_mp_id = '$metering_point'");
	        //year(from_unixtime(rea_date)) = '$year' AND month(from_unixtime(rea_date)) = $month AND
	        foreach($sel[0] as $row){
	            extract($row);
	            if($year == date('Y', $y) && $month == date('m', $y) ){
	                $rea_id = $x;
	                $rea_date = $y;
	                //echo 'reached';
	            }
	        }
	    }

	    $sel = $db->select("SELECT TOP 1 rate_import_wh_1 as rat1, rate_import_wh_2 as rat2, rate_import_wh_3 as rat3,rate_export_wh_4 as rat4,rate_export_wh_5 as rat5,rate_export_wh_6 as rat6, rate_cus_id FROM r_rate WHERE rate_reading_id = '$rea_id' AND rate_cus_id = '$cus_id'");
	    extract($sel[0][0]);
	        
	   	if(empty($rat1) && $rat1 != 0){
	   		$total--;
	   	}    
	   	if(empty($rat2) && $rat2 != 0){
	   		$total--;
	   	}    
	   	if(empty($rat3) && $rat3 != 0){
	   		$total--;
	   	}    
	   	if(empty($rat4) && $rat4 != 0){
	   		$total--;
	   	}    
	   	if(empty($rat5) && $rat5 != 0){
	   		$total--;
	   	}    
	   	if(empty($rat6) && $rat6 != 0){
	   		$total--;
	   	}	    
	    
	    return $total;
	    
	}

	public function rateSumCN($customer_id, $year, $month){
		$count = 1;
		$db = new Db();
		$select = $db->select("SELECT * FROM metering_point, r_reading_cn, r_rate_cn WHERE mp_customer_id = '$customer_id' AND n_rea_cus_id = '$customer_id' AND mp_id = n_rea_mp_id AND n_rate_reading_id = n_rea_id AND (n_rate_import_wh_1 > 0 OR n_rate_import_wh_3 > 0 OR n_rate_export_wh_4 > 0 OR n_rate_export_wh_5 > 0 OR n_rate_export_wh_6 > 0 )");

		$total_mp = 0;
		foreach($select[0] as $row){
			extract($row);
			if(date('Y', $n_rea_date)==$year && date('m', $n_rea_date)==$month){
				$total_mp++;
			}
		}
		//$units = 0.001;

		$tc1 = $tc2 = $tc3 = $tc4 = $tc5 = $tc6 = $tc7 = $tc8 = 0;

		$total_mp;


		$db = new Db();
		$select = $db->select("SELECT * FROM metering_point, r_reading_cn, r_rate_cn WHERE mp_customer_id = '$customer_id' AND n_rea_cus_id = '$customer_id' AND mp_id = n_rea_mp_id AND n_rate_reading_id = n_rea_id AND (n_rate_import_wh_1 > 0 OR n_rate_import_wh_3 > 0 OR n_rate_export_wh_4 > 0 OR n_rate_export_wh_5 > 0 OR n_rate_export_wh_6 > 0 ) ORDER BY n_rea_mp_id DESC");

		$tc1 = $tc2 = $tc3 = $tc4= $tc5= $tc6= $tc7= $tc8 = 0;

		$tca1 = array();
		$tca2 = array();
		$tca3 = array();
		$tca4 = array();

		// echo $db->num_rows();
		// echo '<pre>';
		// print_r($select[0]);
		// echo '</pre>';
		$mp_ids = array();

		foreach($select[0] as $row){
			extract($row);

			if(date('Y', $n_rea_date)==$year && date('m', $n_rea_date)==$month){
				//selecting rates
				
				$db = new Db();
				$sel = $db->select("SELECT * FROM r_rate_cn WHERE n_rate_reading_id = '$n_rea_id'");
				@extract($sel[0][0]);
				
				//calling the function
					$mp_ids[] = $mp_id;

					$cal = rate_calculate_cn($customer_id, ($n_rea_date), $mp_id);
				//echo '<pre>';
				//print_r($cal);
				//echo '</pre>';
				//echo "$count != $total_mp";
				//if($count != $total_mp)

				{
					if($customer_id == 1017){ //BECS

						//echo $cal[0];
						$g1 = ($cal[0]);// - $cal[4]);
						$g2 = ($cal[1]);// - $cal[5]);
						$g3 = ($cal[2]);// - $cal[6]);

						$g4 = $g1 + $g2 + $g3;

						$tc1 += $g1;
						$tc2 += $g2;
						$tc3 += $g3;
						$tc4 += $g4;		
				
					}elseif($customer_id == 2020 && $mp_id == 5324){ //UEDCL => ecopower
						
						$cal_2 = rate_calculate_cn($customer_id, ($n_rea_date), 5278); //muhanga rugyeyo
						$cal_3 = rate_calculate_cn($customer_id, ($n_rea_date), 5274); //nyakagyeme

						$g1 = ($cal[0] - $cal[4] - $cal_2[4] - $cal_3[4]);
						$g2 = ($cal[1] - $cal[5] - $cal_2[5] - $cal_3[5]);
						$g3 = ($cal[2] - $cal[6] - $cal_2[6] - $cal_3[6]);

						$g4 = $g1 + $g2 + $g3;

						$tc1 += $g1;
						$tc2 += $g2;
						$tc3 += $g3;
						$tc4 += $g4;

					}elseif($customer_id == 2020 && $mp_id == 5297){ //UEDCL => nkuusi
        
				        $cal_2 = rate_calculate_cn($customer_id, ($n_rea_date), 5284); //munteme
				        $cal_3 = rate_calculate_cn($customer_id, ($n_rea_date), 5275); //rugombe
				        $cal_4 = rate_calculate_cn($customer_id, ($n_rea_date), 5276); //kakumiro

				        $g1 = ($cal[0] - $cal[4] - $cal_2[4] - $cal_3[4] - $cal_4[4]);
				        $g2 = ($cal[1] - $cal[5] - $cal_2[5] - $cal_3[5] - $cal_4[5]);
				        $g3 = ($cal[2] - $cal[6] - $cal_2[6] - $cal_3[6] - $cal_4[6]);

				        $g4 = $g1 + $g2 + $g3;

				        $tc1 += $g1;
				        $tc2 += $g2;
				        $tc3 += $g3;
				        $tc4 += $g4;

				    }elseif($customer_id == 2020 && $mp_id == 5298){ //UEDCL => waki hydro max
						
						$cal_2 = rate_calculate_cn($customer_id, ($n_rea_date), 5291); //burindi exports

						$g1 = ($cal[4] - $cal[0] - $cal_2[4] );
						$g2 = ($cal[5] - $cal[1] - $cal_2[5] );
						$g3 = ($cal[6] - $cal[2] - $cal_2[6] );

						$g4 = $g1 + $g2 + $g3;

						$tc1 += $g1;
						$tc2 += $g2;
						$tc3 += $g3;
						$tc4 += $g4;

					}elseif($customer_id == 2020 && $mp_id == 5299 ){ //UEDCL => kahungye kamwenge
						
						$cal_2 = rate_calculate(2020, ($rea_date), 5329); //UMEME Kamwenge

						$g1 = ($cal[4] - $cal_2[0] );
						$g2 = ($cal[5] - $cal_2[1] );
						$g3 = ($cal[6] - $cal_2[2] );

						$g4 = $g1 + $g2 + $g3;

						$tc1 += $g1;
						$tc2 += $g2;
						$tc3 += $g3;
						$tc4 += $g4;

					}elseif($customer_id == 2020 && $mp_id == 5326){ //UEDCL => kamwenge
						
						$tc1 += $cal[0];
						$tc2 += $cal[1];
						$tc3 += $cal[2];
						$tc4 += $cal[3];

					}elseif($customer_id == 2020 && $mp_id == '5327'){ //UEDCL => siti 1 5327

				        $cal_2 = rate_calculate_cn(2020, ($n_rea_date), 5267); //kweene exports   

				        $g1 = ($cal[0] - $cal[4]);
				        $g2 = ($cal[1] - $cal[5]);
				        $g3 = ($cal[2] - $cal[6]);

				        $g4 = $g1 + $g2 + $g3;
				        $tc1 += $g1;
				        $tc2 += $g2;
				        $tc3 += $g3;
				        $tc4 += $g4;

				    }elseif($customer_id == 2020 && $mp_id == '5328'){ //UEDCL => siti 2 5328
				        $cal_2 = rate_calculate_cn(2020, ($n_rea_date), 5267); //kweene exports   

				        $g1 = ($cal[0] - $cal[4] - $cal_2[4]);
				        $g2 = ($cal[1] - $cal[5] - $cal_2[5]);
				        $g3 = ($cal[2] - $cal[6] - $cal_2[6]);

				        $g4 = $g1 + $g2 + $g3;
				        $tc1 = $g1;
				        $tc2 = $g2;
				        $tc3 = $g3;
				        $tc4 = $g4;

				    }elseif($customer_id == 2022 && $mp_id == 5315){ //KRECs => kiseruka

						$g1 = ($cal[4]-$cal[0]);
						$g2 = ($cal[5]-$cal[1]);
						$g3 = ($cal[6]-$cal[2]);

						$g4 = $g1 + $g2 + $g3;

						$tc1 += $g1;
						$tc2 += $g2;
						$tc3 += $g3;
						$tc4 += $g4;

					}elseif($customer_id == 2021 && $mp_id == '9383'){ //KIL => Kyambura shp ziba

				        $g1 = ($cal[0] - $cal[4]);
				        $g2 = ($cal[1] - $cal[5]);
				        $g3 = ($cal[2] - $cal[6]);

				        $g4 = $g1 + $g2 + $g3;
				        $tc1 += $g1;
				        $tc2 += $g2;
				        $tc3 += $g3;
				        $tc4 += $g4;

				    }elseif($customer_id == 2021 && $mp_id == '5302'){ //KIL => Kyambura shp ziba

				        $g1 = ($cal[0] - $cal[4]);
				        $g2 = ($cal[1] - $cal[5]);
				        $g3 = ($cal[2] - $cal[6]);

				        $g4 = $g1 + $g2 + $g3;
				        $tc1 += $g1;
				        $tc2 += $g2;
				        $tc3 += $g3;
				        $tc4 += $g4;

				    }elseif($customer_id == 2021 && $mp_id == 5310){ //KIL
												
						$cal_2 = rate_calculate_cn($customer_id, ($n_rea_date), 5303); //Kikorongo
								$cal_3 = rate_calculate_cn(2017, ($n_rea_date), 4009); //SNEL -> Mpondwe
								
								$g0 = ($cal[4] - $cal[0] - $cal_2[4] - $cal_3[0]);
								$g1 = ($cal[5] - $cal[1] - $cal_2[5] - $cal_3[1]);
								$g2 = ($cal[6] - $cal[2] - $cal_2[6] - $cal_3[2]);
								$g3 = $g1 + $g2 + $g0;

						

						$g4 = ($g0*$shoulder + $g1*$peak + $g2*$off_peak)/1000;

						/////////////////////////////////////////

						$tc1 += $g0;
						$tc2 += $g1;
						$tc3 += $g2;
						$tc4 += $g3;
						//echo $mp_location.'>>>>'.$g0.'<br/>';+

					}else{
						$tc1 += $cal[0];
						$tc2 += $cal[1];
						$tc3 += $cal[2];
						$tc4 += $cal[3];
						//echo $mp_location.'>>>>'.$cal[0].'<br/>';
					}

					$tc5 += $cal[4];
					$tc6 += $cal[5];
					$tc7 += $cal[6];
					$tc8 += $cal[7];

				}
			}
		}

		//echo $tc4.'<=<br/>';

		$sum = array($tc1, $tc2, $tc3, $tc4, $tc5, $tc6, $tc7, $tc8, $mp_ids);

		return $sum;

	}

	public function rateSum2($customer_id, $year, $month, $mp_array, $no_wheeling=0){
		$count = 1;
		$db = new Db();
		$select = $db->select("SELECT * FROM metering_point, r_reading WHERE mp_customer_id = '$customer_id' AND rea_cus_id = '$customer_id' AND mp_id = rea_mp_id");

		$total_mp = 0;
		foreach($select[0] as $row){
			extract($row);
			if(date('Y', $rea_date)==$year && date('m', $rea_date)==$month){
				$total_mp++;
			}
		}
		//$units = 0.001;

		$tc1 = $tc2 = $tc3 = $tc4 = 0;

		$total_mp;


		$db = new Db();
		$select = $db->select("SELECT * FROM metering_point, r_reading WHERE mp_customer_id = '$customer_id' AND rea_cus_id = '$customer_id' AND mp_id = rea_mp_id ORDER BY rea_mp_id DESC");

		$tc1 = $tc2 = $tc3 = $tc4= $tc5= $tc6= $tc7= $tc8 = 0;

		$tca1 = array();
		$tca2 = array();
		$tca3 = array();
		$tca4 = array();

		foreach($select[0] as $row){
			extract($row);
			if(in_array($mp_id, $mp_array)){
				if(date('Y', $rea_date)==$year && date('m', $rea_date)==$month){
					//selecting rates
					
					$db = new Db();
					$sel = $db->select("SELECT * FROM r_rate WHERE rate_reading_id = '$rea_id'");
					@extract($sel[0][0]);
					
					//calling the function

					
					////////sdfsdf
					$cal = rate_calculate($customer_id, ($rea_date), $mp_id);
					
					$v = $this->get_advance($customer_id, $mp_id, $month, $year, $cal[23]);

					$g0 = ($v['AR1']);
					$g1 = ($v['AR2']);
					$g2 = ($v['AR3']);
					
					$v = $this->get_advance($customer_id, $mp_id, $month, $year, $cal[23]);
					if($customer_id==1017){
					$g0 = ($cal[0]);
					$g1 = ($cal[1]);
					$g2 = ($cal[2]);
					}else{
					$g0 = ($v['AR1']);
					$g1 = ($v['AR2']);
					$g2 = ($v['AR3']);
					}


					//echo '<pre>';
					//print_r($cal);
					//echo '</pre>';
					//echo "$count != $total_mp";
					//if($count != $total_mp)

					{
					///999999
						if($customer_id == 101712121212){ //BECS
							if($no_wheeling){
								$tc1 += $cal[8];// * $mp_wheeling_charge/100;
								$tc2 += $cal[9];// * $mp_wheeling_charge/100;
								$tc3 += $cal[10];//* $mp_wheeling_charge/100;
								$tc4 += $cal[11];// * $mp_wheeling_charge/100;
							}else{
								$tc1 += $cal[8] * $mp_wheeling_charge/100;
								$tc2 += $cal[9] * $mp_wheeling_charge/100;
								$tc3 += $cal[10] * $mp_wheeling_charge/100;
								$tc4 += $cal[11] * $mp_wheeling_charge/100;
							}						
						}else{
							if($no_wheeling){
								$tc1 += $g0;// * $mp_wheeling_charge/100;
								$tc2 += $g1;// * $mp_wheeling_charge/100;
								$tc3 += $g2;// * $mp_wheeling_charge/100;
								$tc4 += ($g0+$g1+$g2);// * $mp_wheeling_charge/100;
							}else{
								$tc1 += ($g0) * $mp_wheeling_charge/100;
								$tc2 += ($g1) * $mp_wheeling_charge/100;
								$tc3 += ($g2) * $mp_wheeling_charge/100;
								$tc4 += ($g0+$g1+$g2) * $mp_wheeling_charge/100;
							}
							
						}

						$tc5 += $cal[4];
						$tc6 += $cal[5];
						$tc7 += $cal[6];
						$tc8 += $cal[7];

						$tc9 += $cal[0];
						$tc10 += $cal[1];
						$tc11 += $cal[2];
						$tc12 += $cal[3];

					}
				}
			}
		}

		$sum = array($tc1, $tc2, $tc3, $tc4, $tc5, $tc6, $tc7, $tc8, $tc9, $tc10, $tc11, $tc12);

		return $sum;

	}
	public function deleteMonthReadingsAction(){
		$id = portion(3);
		$year = portion(4);
		$month = portion(5);
		$units = $this->customerAndSchedule($id, $year, $month);
		$this->energyLinks($id, $year, $month);
		echo '<div class="clearfix"></div>';
		if(isset($_POST['save'])){
			$filter_year = $_POST['filter_year'];
			$filter_month = $_POST['filter_month'];
			$customers = $_POST['customers'];
			$time_readings = strtotime(date($filter_year.'-'.$filter_month.'-15'));
			$errors = array();


			if(empty($customers)){
				$errors[] = "Check atleast one Customer";
			}

			foreach($customers as $customer){
				$db = new Db();

				if(!$this->isLocked($customer, $filter_year, $filter_month)){
					$select = $db->select("SELECT rea_id FROM r_reading WHERE rea_date = '$time_readings' AND rea_cus_id = '$customer'");
					
					if(!$db->num_rows()){
						$errors[] = "Readings donot exit for  &nbsp;  &nbsp;  &nbsp; <b>Customer: </b>".$this->rgf("customer", $customer, "customer_id", "customer_short_name")." &nbsp;  &nbsp;  &nbsp; <b>Year: </b>".$filter_year."  &nbsp;  &nbsp;  &nbsp;  <b>Month: </b>".$filter_month;
					}
				}else{
					$errors[] = "Readings are locked for  &nbsp;  &nbsp;  &nbsp; <b>Customer: </b>".$this->rgf("customer", $customer, "customer_id", "customer_short_name")." &nbsp;  &nbsp;  &nbsp; <b>Year: </b>".$filter_year."  &nbsp;  &nbsp;  &nbsp;  <b>Month: </b>".$filter_month;
				}

			}

			if(empty($errors)){
				foreach($customers as $customer){
					if(1){
						$n = new Db();
						$sel = $n->select("SELECT mp_location as pnt FROM metering_point WHERE mp_customer_id = '$customer'");
						foreach($sel[0] as $row){
							extract($row);

							$db = new Db();
							$select = $db->select("SELECT mp_id,mp_advance FROM metering_point WHERE mp_customer_id = '$customer' AND mp_location = '$pnt'");
							extract($select[0][0]);

							$select = $db->select("SELECT * FROM r_reading WHERE rea_date = '$time_readings' AND rea_cus_id = '$customer' AND rea_mp_id = '$mp_id'");

							if($db->num_rows()){
								extract($select[0][0]);
								$rrr = $rea_id;

								$db = new Db();
								$sql = "DELETE FROM r_rate WHERE rate_cus_id = '$customer' AND rate_reading_id = '$rrr'";
								$delete = $db->query($sql);

								$db = new Db();
								$sql = "DELETE FROM r_reading WHERE rea_id = '$rrr'";
								$delete = $db->query($sql);
								
							}
						}
					}
				}

				Feedback::success("Successfully Deleted");
				Feedback::refresh(3, return_url()."services/readings/".$id.'/'.$filter_year.'/'.$filter_month);
			}
			
			
			if(!empty($errors)){
				Feedback::errors($errors);
			}
		}

		?>
		<div class="clearfix"></div>
		<hr/>
		<form class="search" action="" method="post">
			<h4 style="color:red;"><i class="fa fa-fw fa-times"></i> Delete Month Readings</h4>
			<hr/><br/>
			<div class="" style="margin:10px;"> <b>Select Year: </b>&nbsp;  
				<select name="filter_year" style="width:80px; margin: auto 10px;">
					<option>Select</option>
					<?php 
					if(empty($filter_year)){
						$filter_year = $year;
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
			<div class="" style="margin:10px;">
				<b>Select Month:</b> 
				<select name="filter_month" style="width:120px; margin: auto 10px;">
					<option>Select</option>
					<?php 
					if(empty($filter_month)){
						$filter_month = $month;
					}
					for($i=1; $i<=12; $i++){
						if($i == $filter_month)
							echo '<option value="'.$i.'" selected="selected">'.month_name($i).'</option>';
						elseif($i == $month_uploaded)
							echo '<option value="'.$i.'" selected="selected">'.month_name($i).'</option>';
						else
							echo '<option value="'.$i.'">'.month_name($i).'</option>';
					}
					?>
				</select>
			</div>
			<div class="clear"><br/></div>
			<div class="" style="margin:10px;">
				<b>Customers:</b> 
				<div class="clearfix"></div>
				<?php
				$db = new Db();
				$select = $db->select("SELECT * FROM customer ORDER BY customer_short_name ASC");
				foreach($select[0] as $row){
					extract($row);
					echo '<div class="col-md-3">';
					if($customer_id == $id || in_array($customer_id, $customers))
						echo '<input class="chk-col-red" type="checkbox" name="customers[]" checked="checked" value="'.$customer_id.'" id="cb'.$customer_id.'"/>';
					else
						echo '<input class="chk-col-red" type="checkbox" name="customers[]" value="'.$customer_id.'" id="cb'.$customer_id.'"/>';

					echo '<label for="cb'.$customer_id.'">'.$customer_short_name.'</label>';
					echo '</div>';
				}
				//echo '<pre>';
				//print_r($select[0]);
				//echo '</pre>';
				?>
			</div>			
			<div class="clearfix"><br/></div>
			<div class="clearfix"><br/></div>
			<div  class="">
				<button type="submit" class="btn-block btn btn-danger" name="save" style=""><i class="fa fa-fw fa-trash"></i> Remove</button>
			</div>
			<div class="clear"></div>
		</form>

		<?php
	}

	public function deleteCreditNoteMonthReadingsAction(){
		$id = portion(3);
		$year = portion(4);
		$month = portion(5);
		$units = $this->customerAndSchedule($id, $year, $month);
		$this->energyLinks($id, $year, $month);

		$not = ($this->isDNorCN($id, $year, $month) > 0)? "Debit Note":"Credit Note" ;

		echo '<div class="clearfix"></div>';
		if(isset($_POST['save'])){
			$filter_year = $_POST['filter_year'];
			$filter_month = $_POST['filter_month'];
			$customers = $_POST['customers'];
			$time_readings = strtotime(date($filter_year.'-'.$filter_month.'-15'));
			$errors = array();


			if(empty($customers)){
				$errors[] = "Check atleast one Customer";
			}

			foreach($customers as $customer){
				if(!$this->isLocked($customer, $filter_year, $filter_month)){
					$db = new Db();
					$select = $db->select("SELECT n_rea_id FROM r_reading_cn WHERE n_rea_date = '$time_readings' AND n_rea_cus_id = '$customer'");
					
					if(!$db->num_rows()){
						$errors[] = $not." Readings donot exit for  &nbsp;  &nbsp;  &nbsp; <b>Customer: </b>".$this->rgf("customer", $customer, "customer_id", "customer_short_name")." &nbsp;  &nbsp;  &nbsp; <b>Year: </b>".$filter_year."  &nbsp;  &nbsp;  &nbsp;  <b>Month: </b>".$filter_month;
					}
				}else{
					$errors[] = $not." Readings are locked  &nbsp;  &nbsp;  &nbsp; <b>Customer: </b>".$this->rgf("customer", $customer, "customer_id", "customer_short_name")." &nbsp;  &nbsp;  &nbsp; <b>Year: </b>".$filter_year."  &nbsp;  &nbsp;  &nbsp;  <b>Month: </b>".$filter_month;
				}

			}

			if(empty($errors)){
				foreach($customers as $customer){
					if(1){
						$n = new Db();
						$sel = $n->select("SELECT mp_location as pnt FROM metering_point WHERE mp_customer_id = '$customer'");
						foreach($sel[0] as $row){
							extract($row);

							$db = new Db();
							$select = $db->select("SELECT mp_id,mp_advance FROM metering_point WHERE mp_customer_id = '$customer' AND mp_location = '$pnt'");
							extract($select[0][0]);

							$select = $db->select("SELECT * FROM r_reading_cn WHERE n_rea_date = '$time_readings' AND n_rea_cus_id = '$customer' AND n_rea_mp_id = '$mp_id'");

							if($db->num_rows()){
								extract($select[0][0]);
								$rrr = $n_rea_id;

								$db = new Db();
								$sql = "DELETE FROM r_rate_cn WHERE n_rate_cus_id = '$customer' AND n_rate_reading_id = '$rrr'";
								$delete = $db->query($sql);

								$db = new Db();
								$sql = "DELETE FROM r_reading_cn WHERE n_rea_id = '$rrr'";
								$delete = $db->query($sql);
								
							}

							$db = new Db();
							$select = $db->select("SELECT * FROM r_reading_cn_previous WHERE n_rea_date = '$time_readings' AND n_rea_cus_id = '$customer' AND n_rea_mp_id = '$mp_id'");

							if($db->num_rows()){
								extract($select[0][0]);
								$rrr = $n_rea_id;

								$db = new Db();
								$sql = "DELETE FROM r_rate_cn_previous WHERE n_rate_cus_id = '$customer' AND n_rate_reading_id = '$rrr'";
								$delete = $db->query($sql);

								$db = new Db();
								$sql = "DELETE FROM r_reading_cn_previous WHERE n_rea_id = '$rrr'";
								$delete = $db->query($sql);
								
							}
						}
					}
				}

				Feedback::success("Successfully Deleted");
				Feedback::refresh();
			}
			
			
			if(!empty($errors)){
				Feedback::errors($errors);
			}
		}
		

		?>
		<div class="clearfix"></div>
		<hr/>
		<form class="search" action="" method="post">
			<h4 style="color:red;"><i class="fa fa-fw fa-times"></i> Delete <?php echo $not; ?> Month Readings</h4>
			<hr/><br/>
			<div class="" style="margin:10px;"> <b>Select Year: </b>&nbsp;  
				<select name="filter_year" style="width:80px; margin: auto 10px;">
					<option>Select</option>
					<?php 
					if(empty($filter_year)){
						$filter_year = $year;
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
			<div class="" style="margin:10px;">
				<b>Select Month:</b> 
				<select name="filter_month" style="width:120px; margin: auto 10px;">
					<option>Select</option>
					<?php 
					if(empty($filter_month)){
						$filter_month = $month;
					}
					for($i=1; $i<=12; $i++){
						if($i == $filter_month)
							echo '<option value="'.$i.'" selected="selected">'.month_name($i).'</option>';
						elseif($i == $month_uploaded)
							echo '<option value="'.$i.'" selected="selected">'.month_name($i).'</option>';
						else
							echo '<option value="'.$i.'">'.month_name($i).'</option>';
					}
					?>
				</select>
			</div>
			
			<div class="clear"><br/></div>
			<div class="" style="margin:10px;">
				<b>Customers:</b> 
				<div class="clearfix"></div>
				<?php
				$db = new Db();
				$select = $db->select("SELECT * FROM customer ORDER BY customer_short_name ASC");
				foreach($select[0] as $row){
					extract($row);
					echo '<div class="col-md-3">';
					if($customer_id == $id || in_array($customer_id, $customers))
						echo '<input class="chk-col-red" type="checkbox" name="customers[]" checked="checked" value="'.$customer_id.'" id="cb'.$customer_id.'"/>';
					else
						echo '<input class="chk-col-red" type="checkbox" name="customers[]" value="'.$customer_id.'" id="cb'.$customer_id.'"/>';

					echo '<label for="cb'.$customer_id.'">'.$customer_short_name.'</label>';
					echo '</div>';
				}
				//echo '<pre>';
				//print_r($select[0]);
				//echo '</pre>';
				?>
			</div>			
			<div class="clearfix"><br/></div>
			<div class="clearfix"><br/></div>
			<div  class="">
				<button type="submit" class="btn-block btn btn-danger" name="save" style=""><i class="fa fa-fw fa-trash"></i> Remove</button>
			</div>
			<div class="clear"></div>
		</form>

		<?php
	}

	public function newMonthReadingsAction(){
		$id = portion(3);
		$year = portion(4);
		$month = portion(5);
		$units = $this->customerAndSchedule($id, $year, $month);
		$this->energyLinks($id, $year, $month);
#######################################################################################

//$to_date = strtotime($currentDate1->format('Y-m-d'));


//echo "Last month was: " . $lastMonth2;
		$to_date = strtotime(date($year.'-'.$month.'-31'));
		$from_date = strtotime(date($year.'-'.$month.'-1'));
		//echo "@@@".$from_date;
		echo '<div class="clearfix"></div>';
		if(isset($_POST['save'])){
			$filter_year = $_POST['filter_year'];
			$filter_month = $_POST['filter_month'];
			$customers = $_POST['customers']; 
			$entity = $_POST['entity'];
			$time_readings = strtotime(date($filter_year.'-'.$filter_month.'-15'));
			$errors = array();

			if(empty($customers)){
				$errors[] = "Check atleast one Customer";
			}
			foreach($customers as $customer){
				$db = new Db();
				echo $sql = "SELECT rea_id FROM r_reading WHERE rea_date = '$time_readings' AND rea_cus_id = '$customer'";
				$select = $db->select($sql);
				//echo"SELECT rea_id FROM r_reading WHERE rea_date = '$time_readings' AND rea_cus_id = '$customer'";				
				if($db->num_rows()){
					$errors[] = "ssReadings already exit for  &nbsp;  &nbsp;  &nbsp; <b>Customer: </b>".$this->rgf("customer", $customer, "customer_id", "customer_short_name")." &nbsp;  &nbsp;  &nbsp; <b>Year: </b>".$filter_year."  &nbsp;  &nbsp;  &nbsp;  <b>Month: </b>".$filter_month;
				}

			}
  if($entity=='previous_month'){
			

			if(empty($errors)){
				foreach($customers as $customer){
					if(1){
						$n = new Db();
						$sel = $n->select("SELECT * FROM r_reading WHERE rea_date >= '$from_date' AND rea_date <= '$to_date' AND rea_cus_id = '$customer'");

						//$sel = $n->select("SELECT mp_location as pnt FROM metering_point WHERE mp_customer_id = '$customer'");
						foreach($sel[0] as $row){
							extract($row);
							$db = new Db();
							// $select = $db->select("SELECT mp_id,mp_advance FROM metering_point WHERE mp_customer_id = '$customer' AND mp_location = '$pnt'");
							// extract($select[0][0]);

							$insert = $db->insert("r_reading",[
								"rea_number"=>$file_name,
								"rea_date"=>$time_readings,
								"rea_cus_id"=>$customer,
								"rea_added_by"=>user_id(),
								"rea_date_added"=>time(),
								"rea_mp_id"=>$rea_mp_id,
							]);					
							
							$db = new Db();
							$select = $db->select("SELECT TOP 1 rea_id FROM r_reading ORDER BY rea_id DESC");
							extract($select[0][0]);
							$rrr = $rea_id;
							
							$sql = "SELECT TOP 1 rate_sfi, rate_sfe, rate_lfi, rate_lfe rate_narration, rate_label, rate_wc_sfe, rate_wc_sfi, rate_wc_lfe, rate_wc_lfi, rate_payable_exports, rate_payable_imports, rate_lfe, rate_lfi, rate_tlf, rate_tlfs FROM r_rate, r_reading WHERE rate_cus_id = '$customer' AND rea_id = rate_reading_id AND rea_cus_id = '$customer' AND rea_mp_id = '$rea_mp_id' ORDER BY rate_id DESC";
							$select = $db->select($sql);
							extract($select[0][0]);
							// echo '<pre>';
							// print_r($select);
							// echo '</pre>';
						
							
							{ //imports as advance
								$select = $db->insert("r_rate",[
									"rate_import_wh_1"=>0,
									"rate_import_wh_2"=>0,
									"rate_import_wh_3"=>0,
									"rate_export_wh_4"=>0,
									"rate_export_wh_5"=>0,
									"rate_export_wh_6"=>0,
									"rate_reading_id"=>$rrr,
									"rate_added_by"=>user_id(),
									"rate_date_added"=>time(),
									"rate_cus_id"=>$customer,
									"rate_sfi"=>$rate_sfi,
									"rate_sfe"=>$rate_sfe,
									"rate_lfe"=>$rate_lfe,
									"rate_lfi"=>$rate_lfi,
									"rate_narration"=>$rate_narration,
									"rate_label"=>$rate_label,
									"rate_wc_sfe"=>$rate_wc_sfe,
									"rate_wc_sfi"=>$rate_wc_sfi,
									"rate_wc_lfe"=>$rate_wc_lfe,
									"rate_wc_lfi"=>$rate_wc_lfi,
									"rate_payable_imports"=>$rate_payable_imports,
									"rate_payable_exports"=>$rate_payable_exports,
									"rate_tlf"=>$rate_tlf,
									"rate_tlfs"=>$rate_tlfs,
								]);

							}						

						}
					}

				}

				Feedback::success("Successfully Created");
				Feedback::refresh(3, return_url().'services/readings/'.$id.'/'.$filter_year.'/'.$filter_month);
			}
		}elseif($entity=='list'){
		if(empty($errors)){
				foreach($customers as $customer){
					if(1){
						$n = new Db();
						$sel = $n->select("SELECT mp_location as pnt FROM metering_point WHERE mp_customer_id = '$customer'");
						foreach($sel[0] as $row){
							extract($row);
							$db = new Db();
							$select = $db->select("SELECT mp_id,mp_advance FROM metering_point WHERE mp_customer_id = '$customer' AND mp_location = '$pnt'");
							extract($select[0][0]);

							$insert = $db->insert("r_reading",[
								"rea_number"=>$file_name,
								"rea_date"=>$time_readings,
								"rea_cus_id"=>$customer,
								"rea_added_by"=>user_id(),
								"rea_date_added"=>time(),
								"rea_mp_id"=>$mp_id,
							]);					
							
							$db = new Db();
							$select = $db->select("SELECT TOP 1 rea_id FROM r_reading ORDER BY rea_id DESC");
							extract($select[0][0]);
							$rrr = $rea_id;
							$sql = "SELECT TOP 1 rate_sfi,rate_tlfs,rate_tlf, rate_sfe, rate_lfi, rate_lfe rate_narration, rate_label, rate_wc_sfe, rate_wc_sfi, rate_wc_lfe, rate_wc_lfi, rate_payable_exports, rate_payable_imports, rate_lfe, rate_lfi FROM r_rate, r_reading WHERE rate_cus_id = '$customer' AND rea_id = rate_reading_id AND rea_cus_id = '$customer' AND rea_mp_id = '$mp_id' ORDER BY rate_id DESC";
							$select = $db->select($sql);
							extract($select[0][0]);
							// echo '<pre>';
							// print_r($select);
							// echo '</pre>';
						
							
							{ //imports as advance
								$select = $db->insert("r_rate",[
									"rate_import_wh_1"=>0,
									"rate_import_wh_2"=>0,
									"rate_import_wh_3"=>0,
									"rate_export_wh_4"=>0,
									"rate_export_wh_5"=>0,
									"rate_export_wh_6"=>0,
									"rate_reading_id"=>$rrr,
									"rate_added_by"=>user_id(),
									"rate_date_added"=>time(),
									"rate_cus_id"=>$customer,
									"rate_sfi"=>$rate_sfi,
									"rate_sfe"=>$rate_sfe,
									"rate_lfe"=>$rate_lfe,
									"rate_lfi"=>$rate_lfi,
									"rate_narration"=>$rate_narration,
									"rate_label"=>$rate_label,
									"rate_wc_sfe"=>$rate_wc_sfe,
									"rate_wc_sfi"=>$rate_wc_sfi,
									"rate_wc_lfe"=>$rate_wc_lfe,
									"rate_wc_lfi"=>$rate_wc_lfi,
									"rate_payable_imports"=>$rate_payable_imports,
									"rate_payable_exports"=>$rate_payable_exports,
									"rate_tlf"=>$rate_tlf,
									"rate_tlfs"=>$rate_tlfs,
								]);

							}						

						}
					}

				}

				Feedback::success("Successfully Created");
				Feedback::refresh(3, return_url().'services/readings/'.$id.'/'.$filter_year.'/'.$filter_month);
			}	
		}
			
			echo '<div class="clearfix"></div>';
			if(!empty($errors)){
				Feedback::errors($errors);
			}
		}

		?>
		<div class="clearfix"></div>
		<hr/>
		<form class="search" action="" method="post">
			<h4 style="color:green"><i class="fa fa-fw fa-plus"></i> Add New Month Readings</h4>
			<hr/><br/>
			<div class="" style="margin:10px;"> <b>Select Year: </b>&nbsp;  
				<select name="filter_year" style="width:80px; margin: auto 10px;">
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
			<div class="" style="margin:10px;">
				<b>Select Month:</b> 
				<select name="filter_month" style="width:120px; margin: auto 10px;">
					<option>Select</option>
					<?php 
					if(empty($filter_month)){
						$filter_month = date('m');
					}
					for($i=1; $i<=12; $i++){
						if($i == $filter_month)
							echo '<option value="'.$i.'" selected="selected">'.month_name($i).'</option>';
						elseif($i == $month_uploaded)
							echo '<option value="'.$i.'" selected="selected">'.month_name($i).'</option>';
						else
							echo '<option value="'.$i.'">'.month_name($i).'</option>';
					}
					?>
				</select>
			</div>
			<div class="" style="margin:10px;">
				<b>Metering Point Entity:</b> 
				<select name="entity" style="width:210px; margin: auto 10px;">
					<option value="previous_month">Pick from the previous month</option>
					<option value="list">Pick from the list</option>
				</select>
			</div>
			<div class="clear"><br/></div>
			<div class="" style="margin:10px;">
				<b>Customers:</b> 
				<div class="clearfix"></div>
				<?php
				$db = new Db();
				$select = $db->select("SELECT * FROM customer ORDER BY customer_short_name ASC");
				foreach($select[0] as $row){
					extract($row);
					echo '<div class="col-md-3">';
					if($customer_id == $id || in_array($customer_id, $customers))
						echo '<input type="checkbox" name="customers[]" checked="checked" value="'.$customer_id.'" id="cb'.$customer_id.'"/>';
					else
						echo '<input type="checkbox" name="customers[]" value="'.$customer_id.'" id="cb'.$customer_id.'"/>';

					echo '<label for="cb'.$customer_id.'">'.$customer_short_name.'</label>';
					echo '</div>';
				}
				//echo '<pre>';
				//print_r($select[0]);
				//echo '</pre>';
				?>
			</div>			
			<div class="clearfix"><br/></div>
			<div class="clearfix"><br/></div>
			<div  class="">
				<button type="submit" class="btn-block btn btn-success" name="save" style=""><i class="fa fa-fw fa-save"></i> Save</button>
			</div>
			<div class="clear"></div>
		</form>

		<?php
	}

		
	public function creditNoteAction(){
		$id = ($this->set_customer)?$this->set_customer: portion(3);
		$year = ($this->set_year)?$this->set_year: portion(4);
		$month = ($this->set_month)?$this->set_month: portion(5);

		$units = $this->customerAndSchedule($id, $year, $month);

		$this->energyLinks($id, $year, $month);
		echo '<div class="clearfix"></div>';
		if(isset($_POST['save'])){
			$filter_year = $_POST['filter_year'];
			$filter_month = $_POST['filter_month'];
			$customers = $_POST['customers'];
			$time_readings = strtotime(date($filter_year.'-'.$filter_month.'-15'));
			$errors = array();

			if(empty($customers)){
				$errors[] = "Check atleast one Customer";
			}

			foreach($customers as $customer){
				$db = new Db();

				if(!$this->isLocked($customer, $filter_year, $filter_month)){
					$select = $db->select("SELECT n_rea_id FROM r_reading_cn WHERE n_rea_date = '$time_readings' AND n_rea_cus_id = '$customer'");
					
					if($db->num_rows()){
						$errors[] = "Credit Note Readings already exit for  &nbsp;  &nbsp;  &nbsp; <b>Customer: </b>".$this->rgf("customer", $customer, "customer_id", "customer_short_name")." &nbsp;  &nbsp;  &nbsp; <b>Year: </b>".$filter_year."  &nbsp;  &nbsp;  &nbsp;  <b>Month: </b>".$filter_month;
					}
				}else{
					$errors[] = "Readings are locked for  &nbsp;  &nbsp;  &nbsp; <b>Customer: </b>".$this->rgf("customer", $customer, "customer_id", "customer_short_name")." &nbsp;  &nbsp;  &nbsp; <b>Year: </b>".$filter_year."  &nbsp;  &nbsp;  &nbsp;  <b>Month: </b>".$filter_month;
				}
			}

			if(empty($errors)){
				foreach($customers as $customer){
					if(1){
						$n = new Db();
						$sel = $n->select("SELECT mp_location as pnt FROM metering_point WHERE mp_customer_id = '$customer'");
						foreach($sel[0] as $row){
							extract($row);
							$db = new Db();
							$select = $db->select("SELECT mp_id,mp_advance FROM metering_point WHERE mp_customer_id = '$customer' AND mp_location = '$pnt'");
							extract($select[0][0]);

							$insert = $db->insert("r_reading_cn",[
								"n_rea_number"=>$file_name,
								"n_rea_date"=>$time_readings,
								"n_rea_cus_id"=>$customer,
								"n_rea_added_by"=>user_id(),
								"n_rea_date_added"=>time(),
								"n_rea_mp_id"=>$mp_id,
							]);			



							//GETT READINGS AND FORMULAS ====================

							$db = new Db();
							$select = $db->select("SELECT TOP 1 rea_id FROM r_reading ORDER BY rea_id DESC");
							extract($select[0][0]);
							$rrr = $rea_id;
							$sql = "SELECT TOP 1 rate_import_wh_1, rate_import_wh_2, rate_import_wh_3, rate_export_wh_4, rate_export_wh_5, rate_export_wh_6, rate_sfi, rate_sfe, rate_lfi, rate_lfe rate_narration, rate_label, rate_wc_sfe, rate_wc_sfi, rate_wc_lfe, rate_wc_lfi, rate_payable_exports, rate_payable_imports, rate_lfe, rate_lfi FROM r_rate, r_reading WHERE rate_cus_id = '$customer' AND rea_id = rate_reading_id AND rea_cus_id = '$customer' AND rea_mp_id = '$mp_id' AND rea_date < '$time_readings' ORDER BY rate_id DESC";
							$select = $db->select($sql);
							extract($select[0][0]);
							//===============================================
									
							
							$db = new Db();
							$select = $db->select("SELECT TOP 1 n_rea_id FROM r_reading_cn ORDER BY n_rea_id DESC");
							extract($select[0][0]);
							$rrr = $n_rea_id;
							
							{ //imports as advance
								$select = $db->insert("r_rate_cn",[
									"n_rate_import_wh_1"=>0,
									"n_rate_import_wh_2"=>0,
									"n_rate_import_wh_3"=>0,
									"n_rate_export_wh_4"=>0,
									"n_rate_export_wh_5"=>0,
									"n_rate_export_wh_6"=>0,
									"n_rate_reading_id"=>$rrr,
									"n_rate_added_by"=>user_id(),
									"n_rate_date_added"=>time(),
									"n_rate_cus_id"=>$customer,
									
									"n_rate_sfi"=>$rate_sfi,
									"n_rate_sfe"=>$rate_sfe,
									"n_rate_lfe"=>$rate_lfe,
									"n_rate_lfi"=>$rate_lfi,
									"n_rate_narration"=>$rate_narration,
									"n_rate_label"=>$rate_label,
									"n_rate_wc_sfe"=>$rate_wc_sfe,
									"n_rate_wc_sfi"=>$rate_wc_sfi,
									"n_rate_wc_lfe"=>$rate_wc_lfe,
									"n_rate_wc_lfi"=>$rate_wc_lfi,
									"n_rate_payable_imports"=>$rate_payable_imports,
									"n_rate_payable_exports"=>$rate_payable_exports,
									"n_rate_tlf"=>$rate_tlf,
									"n_rate_tlfs"=>$rate_tlfs,
								]);
							}						

							$insert = $db->insert("r_reading_cn_previous",[
								"n_rea_number"=>$file_name,
								"n_rea_date"=>$time_readings,
								"n_rea_cus_id"=>$customer,
								"n_rea_added_by"=>user_id(),
								"n_rea_date_added"=>time(),
								"n_rea_mp_id"=>$mp_id,
							]);					
							
							$db = new Db();
							$select = $db->select("SELECT TOP 1 n_rea_id FROM r_reading_cn_previous ORDER BY n_rea_id DESC");
							extract($select[0][0]);
							$rrr = $n_rea_id;
							{ //imports as advance
								$select = $db->insert("r_rate_cn_previous",[
									"n_rate_import_wh_1"=>$rate_import_wh_1,
									"n_rate_import_wh_2"=>$rate_import_wh_2,
									"n_rate_import_wh_3"=>$rate_import_wh_3,
									"n_rate_export_wh_4"=>$rate_export_wh_4,
									"n_rate_export_wh_5"=>$rate_export_wh_5,
									"n_rate_export_wh_6"=>$rate_export_wh_6,
									"n_rate_reading_id"=>$rrr,
									"n_rate_added_by"=>user_id(),
									"n_rate_date_added"=>time(),
									"n_rate_cus_id"=>$customer
								]);
							}						

						}
					}

				}

				Feedback::success("Successfully Created, redirecting please wait");
				Feedback::refresh(1, return_url().'services/readings/'.$customer.'/'.$filter_year.'/'.$filter_month.'/credit-note');
			}
			
			echo '<div class="clearfix"></div>';
			if(!empty($errors)){
				Feedback::errors($errors);
			}
		}

		?>
		<div class="clearfix"></div>
		<hr/>
		<form class="search" action="" method="post">
			<h4 style="color:green"><i class="fa fa-fw fa-plus"></i> Add Debit / Credit Note</h4>
			<hr/><br/>
			<div class="" style="margin:10px;"> <b>Select Year: </b>&nbsp;  
				<select name="filter_year" style="width:80px; margin: auto 10px;">
					<option>Select</option>
					<?php 
					if(empty($filter_year)){
						$filter_year = $year;
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
			<div class="" style="margin:10px;">
				<b>Select Month:</b> 
				<select name="filter_month" style="width:120px; margin: auto 10px;">
					<option>Select</option>
					<?php 
					if(empty($filter_month)){
						$filter_month = $month;
					}
					for($i=1; $i<=12; $i++){
						if($i == $filter_month)
							echo '<option value="'.$i.'" selected="selected">'.month_name($i).'</option>';
						elseif($i == $month_uploaded)
							echo '<option value="'.$i.'" selected="selected">'.month_name($i).'</option>';
						else
							echo '<option value="'.$i.'">'.month_name($i).'</option>';
					}
					?>
				</select>
			</div>
			<div class="clear"><br/></div>
			<div class="" style="margin:10px;">
				<b>Customers:</b> 
				<div class="clearfix"></div>
				<?php
				$db = new Db();
				$select = $db->select("SELECT * FROM customer ORDER BY customer_short_name ASC");
				foreach($select[0] as $row){
					extract($row);
					echo '<div class="col-md-3">';
					if($customer_id == $id || in_array($customer_id, $customers))
						echo '<input type="checkbox" name="customers[]" checked="checked" value="'.$customer_id.'" id="cb'.$customer_id.'"/>';
					else
						echo '<input type="checkbox" name="customers[]" value="'.$customer_id.'" id="cb'.$customer_id.'"/>';

					echo '<label for="cb'.$customer_id.'">'.$customer_short_name.'</label>';
					echo '</div>';
				}
				//echo '<pre>';
				//print_r($select[0]);
				//echo '</pre>';
				?>
			</div>			
			<div class="clearfix"><br/></div>
			<div class="clearfix"><br/></div>
			<div  class="">
				<button type="submit" class="btn-block btn btn-success" name="save" style=""><i class="fa fa-fw fa-save"></i> Save</button>
			</div>
			<div class="clear"></div>
		</form>

		<?php
	}


	public function salesTrendAction(){
		$id = portion(3);
		$year = portion(4);
		$month = portion(5);
		$units = $this->customerAndSchedule($id, $year, $month);
		$this->energyLinks($id, $year, $month);
		echo '<div class="clearfix"></div>';
		if(isset($_POST['save'])){
			$filter_year = $_POST['filter_year'];
			$filter_month = $_POST['filter_month'];
			$customers = $_POST['customers'];
			$time_readings = strtotime(date($filter_year.'-'.$filter_month.'-15'));
			$errors = array();

			if(empty($customers)){
				$errors[] = "Check atleast one Customer";
			}

			foreach($customers as $customer){
				$db = new Db();
				ECHO $SQL = "SELECT rea_id FROM r_reading WHERE rea_date = '$time_readings' AND rea_cus_id = '$customer'";
				$select = $db->select($SQL);
				
				if($db->num_rows()){
					$errors[] = "SSReadings already exit for  &nbsp;  &nbsp;  &nbsp; <b>Customer: </b>".$this->rgf("customer", $customer, "customer_id", "customer_short_name")." &nbsp;  &nbsp;  &nbsp; <b>Year: </b>".$filter_year."  &nbsp;  &nbsp;  &nbsp;  <b>Month: </b>".$filter_month;
				}

			}

			if(empty($errors)){
				foreach($customers as $customer){
					if(1){
						$n = new Db();
						$sel = $n->select("SELECT mp_location as pnt FROM metering_point WHERE mp_customer_id = '$customer'");
						foreach($sel[0] as $row){
							extract($row);
							$db = new Db();
							$select = $db->select("SELECT mp_id,mp_advance FROM metering_point WHERE mp_customer_id = '$customer' AND mp_location = '$pnt'");
							extract($select[0][0]);

							$insert = $db->insert("r_reading",[
								"rea_number"=>$file_name,
								"rea_date"=>$time_readings,
								"rea_cus_id"=>$customer,
								"rea_added_by"=>user_id(),
								"rea_date_added"=>time(),
								"rea_mp_id"=>$mp_id,
							]);					
							
							$db = new Db();
							$select = $db->select("SELECT TOP 1 rea_id FROM r_reading ORDER BY rea_id DESC");
							extract($select[0][0]);
							$rrr = $rea_id;
							
							{ //imports as advance
								$select = $db->insert("r_rate",[
									"rate_import_wh_1"=>0,
									"rate_import_wh_2"=>0,
									"rate_import_wh_3"=>0,
									"rate_export_wh_4"=>0,
									"rate_export_wh_5"=>0,
									"rate_export_wh_6"=>0,
									"rate_reading_id"=>$rrr,
									"rate_added_by"=>user_id(),
									"rate_date_added"=>time(),
									"rate_cus_id"=>$customer
								]);
							}						

						}
					}

				}

				Feedback::success("Successfully Created");
				Feedback::refresh();
			}
			
			echo '<div class="clearfix"></div>';
			if(!empty($errors)){
				Feedback::errors($errors);
			}
		}


		$customer_id = portion(3);
		$year = portion(4);
		echo '<div class="clearfix"></div>';
		echo '<hr/>';

		echo '<div class="col-md-12" style="display:none;">';
		echo '<h4>'.$year.' '.$this->rgf("customer", $customer_id, "customer_id", "customer_short_name").' Advance Breakdown</h4>';
		echo '<table id="table" border="1" style="width:auto;">';

		echo '<tr>';
		echo '<th style="width:100px;">Month</th>';

		$db = new Db();
		$mp_ids = array();
		$select = $db->select("SELECT * FROM metering_point WHERE mp_customer_id = '$customer_id'");
		foreach($select[0] as $row){
			extract($row);
			echo '<th style="max-width:80px; overflow:hidden;">'.ucwords(strtolower($mp_location)).'</th>';
			$mp_ids[] = $mp_id;
		}

		echo '<th style="width:100px;">Total</th>';
		echo '</tr>';

		$sum_per_mp = array();
		$sum_per_month = array();
		for($i=1; $i<=12; $i++){
			if($i == $month)
				echo '<tr style="font-weight:750;">';
			else
				echo '<tr>';

			echo '<td>'.month_name($i).'</td>';
			$monthly_sum = 0;
			foreach($mp_ids as $mpid){

				$rea_date = $time_readings = strtotime(date($year.'-'.$i.'-15'));
				if($customer_id == 1017){
					$cal = rate_calculate($customer_id, ($rea_date), $mpid);
				}elseif($customer_id == 2021){
					$cal = rate_calculate($customer_id, ($rea_date), $mpid);
				}else{
					$cal = rate_calculate2($customer_id, ($rea_date), $mpid);
				}
				$value = ($cal[3]*0.000001);

				$sum_per_mp[$mpid] += $value;
				$sum_per_month[$i] += $value;
				$monthly_sum += $value;

				$value = number_format($value, 2);
				if($value == "0.00"){
					$value = "<center>-</center>";
				}else{
					$value = '<span>'.$value.'</span>';
				}				

				echo '<td align="right">'.$value.'</td>';
			}

			if($monthly_sum == "0.00"){
				$value = "<center>-</center>";
			}else{
				$value = number_format($monthly_sum, 2);
			}
			echo '<td align="right">'.$value.'</td>';	//total		
			echo '</tr>';
		}

		echo '<tr>';
		echo '<th>Total</th>';
		foreach($mp_ids as $mpid){
			$value = $sum_per_mp[$mpid];
			if($value == "0.00"){
				$value = "<center>-</center>";
			}else{
				$value = number_format($value, 2);
			}
			echo '<th style="text-align:right">'.$value.'</th>';
		}		
		echo '<th style="text-align:right">'.number_format(array_sum($sum_per_mp), 2).'</th>';
		echo '</tr>';

		$sum_percentages=0;
		echo '<tr>';
		echo '<th>% Contribution</th>';
		foreach($mp_ids as $mpid){
			$value = $sum_per_mp[$mpid];
			{
				$value = number_format($value/array_sum($sum_per_mp)*100, 1);
				$sum_percentages += $value;
			}
			echo '<th style="text-align:right">'.$value.'%</th>';
		}				
		echo '<th style="text-align:right;">'.number_format($sum_percentages).'%</th>';
		echo '</tr>';

		echo '</table>';

		echo '</div>';

		//////////////////////////////////////////////////////////////////////////////////////////
		$year_ago = $year - 1;
		echo '<div id="printMe">';
		echo '<div class="col-lg-4">';

		echo '<h4>'.$this->rgf("customer", $customer_id, "customer_id", "customer_short_name").' Sales Trend '.$year.' Vs '.$year_ago.'</h4>';

		echo '';
		echo '<table id="table" border="1" cellspacing="0" cellpadding="2">';

		echo '<tr valign="bottom">';
		echo '<th rowspan="2">Month</th>';
		echo '<th colspan="2" style="text-align:center;">Units (MWh)</th>';
		echo '<th colspan="2" style="text-align:center;">% Variance</th>';
		echo '</tr>';
		
		echo '<tr>';
		echo '<th style="text-align:center;">'.$year_ago.'</th>';
		echo '<th style="text-align:center;">'.$year.'</th>';
		echo '<th style="text-align:center;">'.$year_ago.'</th>';
		echo '<th style="text-align:center;">'.$year.'</th>';
		echo '</tr>';

		for($i=1; $i<=12; $i++){
			$last = $this->totalYearAdvance($customer_id, $year_ago, $i);

			$current = $this->totalYearAdvance($customer_id, $year, $i);
			if($i > 1){
				$past = $this->totalYearAdvance($customer_id, $year, $i-1);
			}else{
				$past = $this->totalYearAdvance($customer_id, $year-1, 12);
			}

			$var_last = (float)(($last-$current)/$current)*100;
			$var_current = (float)(($current-$past)/$past)*100;

			$var_last = (is_nan($var_last) || is_infinite($var_last))?0:$var_last;
			$var_current = (is_nan($var_current)  || is_infinite($var_current))?0:$var_current;

			echo '<tr>';

			echo '<td style="text-align:left;">'.month_name_short($i).'</td>';
			echo '<td style="text-align:right;">'.number_format($last,2).'</td>';
			echo '<td style="text-align:right;">'.number_format($current,2).'</td>';

			echo '<td style="text-align:right;">'.number_format($var_last,2).' %</td>';
			echo '<td style="text-align:right;">'.number_format($var_current,2).' %</td>';
			echo '</tr>';
		}

		echo '</table>';
		echo '</div>';

		echo '<div class="col-lg-8">';
		echo '<div style="width:75%;">';
		echo '<canvas id="canvas"></canvas>';
		echo '</div>';
		echo '</div>';
		?>
		<script src="<?php display_url(); ?>js/Chart.min.js.js"></script>
		<script src="<?php display_url(); ?>js/utils.js"></script>

		<script>
			var MONTHS = ['Jan', 'Feb', 'Mar', 'Apr', 'May', 'Jun', 'Jul', 'Aug', 'Sepr', 'Oct', 'Nov', 'Dec'];
			var config = {
				type: 'line',
				data: {
					labels: ['Jan', 'Feb', 'Mar', 'Apr', 'May', 'Jun', 'Jul', 'Aug', 'Sep', 'Oct', 'Nov', 'Dec'],
					datasets: [{
						label: '<?php echo $year_ago?>',
						backgroundColor: window.chartColors.red,
						borderColor: window.chartColors.red,
						data: [
							<?php
								for($i=1; $i<=12; $i++){
									echo  str_replace(',', '', number_format($this->totalYearAdvance($customer_id, $year_ago, $i),2));
									echo ',';
								}
							?>
						],
						fill: false,
					}, {
						label: '<?php echo $year; ?>',
						fill: false,
						backgroundColor: window.chartColors.blue,
						borderColor: window.chartColors.blue,
						data: [
							<?php
								for($i=1; $i<=12; $i++){
									echo str_replace(',', '', number_format($this->totalYearAdvance($customer_id, $year, $i),2));
									echo ',';
								}
							?>
						],
					}]
				},
				options: {
					responsive: true,
					title: {
						display: true,
						text: '<?php echo $this->rgf("customer", $customer_id, "customer_id", "customer_short_name").' Sales Trend '.$year.' Vs '.$year_ago; ?>'
					},
					hover: {
						mode: 'nearest',
						intersect: true
					},
					scales: {
						xAxes: [{
							display: true,
							scaleLabel: {
								display: true,
								labelString: 'Month'
							}
						}],
						yAxes: [{
							display: true,
							scaleLabel: {
								display: true,
								labelString: 'Units (MWh)'
							}
						}]
					}
				}
			};

			window.onload = function() {
				var ctx = document.getElementById('canvas').getContext('2d');
				window.myLine = new Chart(ctx, config);
			};
			
		</script>
		<?php

		echo '<div class="col-md-12">';
		echo '<br/>';
		echo '<h4>'.$year.' '.$this->rgf("customer", $customer_id, "customer_id", "customer_short_name").' Advance Breakdown (MWh)</h4>';

		echo '<input type="text" style="padding:0 10px; margin-bottom: 10px;" name="search" autocomplete="off" placeholder="Search Metering Point" id="searchTerm" onkeyup="doSearch2()" onClick="this.select();">';

		echo '<table id="table2" border="1" style="">';

		echo '<tr>';
		echo '<th style="width:150px;">Metering Point</th>';
		for($i=1; $i<=12; $i++){
			echo '<th>'.month_name($i).'</th>';
		}
		echo '<th>Total</th>';
		echo '<th>% Ctrbn</th>';
		echo '</tr>';

		$db = new Db();
		$mp_ids = array();
		$select = $db->select("SELECT * FROM metering_point WHERE mp_customer_id = '$customer_id'");
		$sum_per_mp = array();
		foreach($select[0] as $row){
			extract($row);
			$sum_per_month = array();
			for($i=1; $i<=12; $i++){			
				$monthly_sum = 0;
				
				$mpid = $mp_id;
				$rea_date = $time_readings = strtotime(date($year.'-'.$i.'-15'));
				if($customer_id == 1017){
					$cal = rate_calculate($customer_id, ($rea_date), $mpid);
				}elseif($customer_id == 2021){
					$cal = rate_calculate($customer_id, ($rea_date), $mpid);
				}else{
					$cal = rate_calculate2($customer_id, ($rea_date), $mpid);
				}
				$value = ($cal[3]*0.000001);

				$sum_per_mp[$mpid] += $value;
			}
		}
		$sum_per_month = array();
		$sum_percentages = 0;
		foreach($select[0] as $row){
			extract($row);
			echo '<tr>';
			echo '<td style="">'.strtoupper(strtolower($mp_location)).'</td>';

			for($i=1; $i<=12; $i++){			
				$monthly_sum = 0;
				{
					$mpid = $mp_id;
					$rea_date = $time_readings = strtotime(date($year.'-'.$i.'-15'));
					if($customer_id == 1017){
						$cal = rate_calculate($customer_id, ($rea_date), $mpid);
					}elseif($customer_id == 2021){
						$cal = rate_calculate($customer_id, ($rea_date), $mpid);
					}else{
						$cal = rate_calculate2($customer_id, ($rea_date), $mpid);
					}
					$value = ($cal[3]*0.000001);

					//$sum_per_mp[$mpid] += $value;
					$sum_per_month[$i] += $value;
					$monthly_sum += $value;

					$value = number_format($value, 2);
					if($value == "0.00"){
						$value = "<center>-</center>";
					}else{
						$value = '<span>'.$value.'</span>';
					}				

					echo '<td align="right">'.$value.'</td>';
				}
				
			}

			$value = $sum_per_mp[$mpid];
			if($value == "0.00"){
				$value = "<center>-</center>";
			}else{
				$value = number_format($value, 2);
			}
			echo '<td style="text-align:right; font-weight:bold;">'.$value.'</td>';
						
			$value = $sum_per_mp[$mpid];
			{
				$value = number_format($value/array_sum($sum_per_mp)*100, 1);
				$sum_percentages += $value;
			}

			echo '<td style="text-align:right; font-weight:bold;">'.$value.'%</td>';				
		

			echo '</tr>';
		}

		echo '<tr>';
		echo '<th style="width:100px;">Total</th>';
		for($i=1; $i<=12; $i++){
			echo '<th style="text-align:right">'.number_format($sum_per_month[$i], 2).'</th>';
		}
		echo '<th style="text-align:right">'.number_format(array_sum($sum_per_mp), 2).'</th>';
		echo '<th style="text-align:right">'.number_format($sum_percentages).'%</th>';
		echo '</tr>';

		echo '</table>';

		echo '</div>';
		echo '</div>';

	}

	public function totalYearAdvance($customer_id, $year, $month){
		$sum_per_month = array();
		$i = $month;

		$db = new Db();
		$mp_ids = array();
		$select = $db->select("SELECT * FROM metering_point WHERE mp_customer_id = '$customer_id'");
		foreach($select[0] as $row){
			extract($row);
			$mp_ids[] = $mp_id;
		}
	
		foreach($mp_ids as $mpid){

			$rea_date = $time_readings = strtotime(date($year.'-'.$i.'-15'));
			if($customer_id == 1017){
				$cal = rate_calculate($customer_id, ($rea_date), $mpid);
			}elseif($customer_id == 2021){
				$cal = rate_calculate($customer_id, ($rea_date), $mpid);
			}else{
				$cal = rate_calculate2($customer_id, ($rea_date), $mpid);
			}
			$value = ($cal[3]*0.000001);

			$sum_per_month[$i] += $value;				
		}
		return array_sum($sum_per_month);		
	}

	public function wheelingChargeSum($customer_id, $year, $month){
		$count = 1;
		$db = new Db();

		$tou = $this->tou($customer_id, $month, $year);
		
		$peak = $tou[0];
		$shoulder = $tou[1];
		$off_peak = $tou[2];

		$select = $db->select("SELECT * FROM metering_point, r_reading WHERE mp_customer_id = '$customer_id' AND rea_cus_id = '$customer_id' AND mp_id = rea_mp_id");

		$total_mp = 0;
		foreach($select[0] as $row){
			extract($row);
			if(date('Y', $rea_date)==$year && date('m', $rea_date)==$month){
				$total_mp++;
			}
		}
		//$units = 0.001;

		$tc1 = $tc2 = $tc3 = $tc4 = 0;

		$total_mp;
		$db = new Db();
		$select = $db->select("SELECT * FROM metering_point, r_reading WHERE mp_customer_id = '$customer_id' AND rea_cus_id = '$customer_id' AND mp_id = rea_mp_id");

		$imports = array();
		$exports = array();

		foreach($select[0] as $row){
			extract($row);

			if(date('Y', $rea_date)==$year && date('m', $rea_date)==$month){
				//selecting rates
				
				$db = new Db();
				$sel = $db->select("SELECT * FROM r_rate WHERE rate_reading_id = '$rea_id'");
				@extract($sel[0][0]);
				
				//calling the function
				$cal = rate_calculate($customer_id, ($rea_date), $mp_id);
				//echo "$count != $total_mp";
				//if($count != $total_mp)
				//echo $cal[24]. '>> ';
				if($cal[24]!="([DONOT_DISPLAY])"){

					//echo $cal[24]. '>> ';
					$imports[] = (($cal[0]*$shoulder+$cal[1]*$peak+$cal[2]*$off_peak)/1000*$mp_wheeling_charge/100); //imports
					$exports[] = (($cal[4]*$shoulder+$cal[5]*$peak+$cal[6]*$off_peak)/1000*$mp_wheeling_charge/100); //exports

				}
			}
		}

		$sum = array($imports, $exports);

		return $sum;

	}

	public function totalBackflowInvoice($id, $year, $month){

		if($id == 2019 || $id == 2036 || $id == 2040 || $id == 2041){
			$c = $this->sumFormulae(2036, $year, $month);
			$c = $this->sumFormulae(2041, $year, $month);
			$arr_mod = array_map( function($val) { return $val/1000; }, $c);
		}else{
			$g0=$g1=$g2=$g3=$g4=$g5=$g6=$g7=0;
			$arr_mod = array($g0, $g1, $g2, $g3, $g4, $g5, $g6, $g7);
		}

		return $arr_mod;
	}

	public function totalBackflow($id, $year, $month){

		if($id == 2019 || $id == 2036 || $id == 2040 || $id == 2041)
			return $c = $this->sumFormulaeBack(2041, $year, $month);
		else
			return array($tc1, $tc2,  $tc3,  $tc4,  $tc5,  $tc6,  $tc7,  $tc8 );

		$tc1 = 0;
	 	$tc2 = 0;
		$tc3 = 0;
		$tc4 = 0;
		$tc5 = 0;
		$tc6 = 0;
		$tc7 = 0;
		$tc8 = 0;
					
		if($id == 2019){	
			$l = $this->sumFormulaeBackflow($id, $year, $month);

			$tou = $this->tou($id, $month, $year);
			
			$peak = $tou[0];
			$shoulder = $tou[1];
			$off_peak = $tou[2];

			$tc1 = $l[0]/1000;
		 	$tc2 = $l[1]/1000;
			$tc3 = $l[2]/1000;
			$tc4 = $l[3]/1000;
			$tc5 = $l[4]/1000;
			$tc6 = $l[5]/1000;
			$tc7 = $l[6]/1000;
			$tc8 = $l[7]/1000;
		}
		//				0	1		2		3	4							5		
		return array($tc1, $tc2,  $tc3,  $tc4,  $tc5,  $tc6,  $tc7,  $tc8 );

	}

	private function KPLC($year, $month, $customer_id = 2030){
		$reading_time = strtotime(date("$year-$month-15"));
		$db = new Db();
		//current main 1
		$select = $db->select("SELECT * FROM r_reading, r_rate, metering_point WHERE rea_date = '$reading_time' AND rea_cus_id = '$customer_id' AND rate_cus_id = '$customer_id' AND rate_reading_id = rea_id AND rea_mp_id = mp_id AND upper(mp_location) = 'MAIN 1'");
		extract($select[0][0]);

		for($i=1; $i<=6; $i++){
			if($i<=3)
				${'cm'.$i} = ${'rate_import_wh_'.$i};
			else
				${'cm'.$i} = ${'rate_export_wh_'.$i};
		}

		// //previous main 1
		$sql = "SELECT * FROM r_reading, r_rate, metering_point WHERE rea_date < '$reading_time' AND rea_id = rate_reading_id AND rea_cus_id = '2030' AND rea_mp_id = mp_id AND rate_cus_id = '2030' AND upper(mp_location) = 'MAIN 1' ORDER BY rea_date DESC";
		$sel = $db->select($sql);

		extract($sel[0][0]);

		for($i=1; $i<=6; $i++){
			if($i<=3)
				${'pm'.$i} = ${'rate_import_wh_'.$i};
			else
				${'pm'.$i} = ${'rate_export_wh_'.$i};
		}

		for($i=1; $i<=6; $i++){
			${'am'.$i} = ${'cm'.$i} - ${'pm'.$i};
		}

		//current main 2
		$select = $db->select("SELECT * FROM r_reading, r_rate, metering_point WHERE rea_date = '$reading_time' AND rea_cus_id = '$customer_id' AND rate_cus_id = '$customer_id' AND rate_reading_id = rea_id AND rea_mp_id = mp_id AND upper(mp_location) = 'MAIN 2'");
		extract($select[0][0]);

		for($i=1; $i<=6; $i++){
			if($i<=3)
				${'cm2'.$i} = ${'rate_import_wh_'.$i};
			else
				${'cm2'.$i} = ${'rate_export_wh_'.$i};
		}

		// //previous main 2
		$sql = "SELECT * FROM r_reading, r_rate, metering_point WHERE rea_date <  '$reading_time' AND rea_id = rate_reading_id AND rea_cus_id = '2030' AND rea_mp_id = mp_id AND rate_cus_id = '2030' AND upper(mp_location) = 'MAIN 2' ORDER BY rea_date DESC";
		$sel = $db->select($sql);

		extract($sel[0][0]);

		for($i=1; $i<=6; $i++){
			if($i<=3)
				${'pm2'.$i} = ${'rate_import_wh_'.$i};
			else
				${'pm2'.$i} = ${'rate_export_wh_'.$i};
		}

		for($i=1; $i<=6; $i++){
			${'am2'.$i} = ${'cm2'.$i} - ${'pm2'.$i};
		}

		$total = array();
		for($i=1; $i<=6; $i++){
			$total['main'][] = ${'am'.$i} + ${'am2'.$i};
		}

		
		

		////////////////////////////////////////////////

		//current main 1
		$select = $db->select("SELECT * FROM r_reading, r_rate, metering_point WHERE rea_date = '$reading_time' AND rea_cus_id = '$customer_id' AND rate_cus_id = '$customer_id' AND rate_reading_id = rea_id AND rea_mp_id = mp_id AND upper(mp_location) = 'CHECK 1'");
		extract($select[0][0]);

		for($i=1; $i<=6; $i++){
			if($i<=3)
				${'cm'.$i} = ${'rate_import_wh_'.$i};
			else
				${'cm'.$i} = ${'rate_export_wh_'.$i};
		}

		// //previous main 1
		$sql = "SELECT * FROM r_reading, r_rate, metering_point WHERE rea_date <  '$reading_time' AND rea_id = rate_reading_id AND rea_cus_id = '2030' AND rea_mp_id = mp_id AND rate_cus_id = '2030' AND upper(mp_location) = 'CHECK 1' ORDER BY rea_date DESC";
		$sel = $db->select($sql);

		extract($sel[0][0]);

		for($i=1; $i<=6; $i++){
			if($i<=3)
				${'pm'.$i} = ${'rate_import_wh_'.$i};
			else
				${'pm'.$i} = ${'rate_export_wh_'.$i};
		}

		for($i=1; $i<=6; $i++){
			${'am'.$i} = ${'cm'.$i} - ${'pm'.$i};
		}

		//current main 2
		$select = $db->select("SELECT * FROM r_reading, r_rate, metering_point WHERE rea_date = '$reading_time' AND rea_cus_id = '$customer_id' AND rate_cus_id = '$customer_id' AND rate_reading_id = rea_id AND rea_mp_id = mp_id AND upper(mp_location) = 'CHECK 2'");
		extract($select[0][0]);

		for($i=1; $i<=6; $i++){
			if($i<=3)
				${'cm2'.$i} = ${'rate_import_wh_'.$i};
			else
				${'cm2'.$i} = ${'rate_export_wh_'.$i};
		}

		// //previous main 2
		$sql = "SELECT * FROM r_reading, r_rate, metering_point WHERE rea_date <  '$reading_time' AND rea_id = rate_reading_id AND rea_cus_id = '2030' AND rea_mp_id = mp_id AND rate_cus_id = '2030' AND upper(mp_location) = 'CHECK 2' ORDER BY rea_date DESC";
		$sel = $db->select($sql);

		extract($sel[0][0]);

		for($i=1; $i<=6; $i++){
			if($i<=3)
				${'pm2'.$i} = ${'rate_import_wh_'.$i};
			else
				${'pm2'.$i} = ${'rate_export_wh_'.$i};
		}

		for($i=1; $i<=6; $i++){
			${'am2'.$i} = ${'cm2'.$i} - ${'pm2'.$i};
		}

		for($i=1; $i<=6; $i++){
			$total['check'][] = ${'am'.$i} + ${'am2'.$i};
		}


		for($i=0; $i<6; $i++){
			$total['average'][] = ($total['check'][$i] + $total['main'][$i])/2;
		}



		return $total;

		////////////////////////////////////////////////
	}

	public function MIRAMA($year, $month, $customer_id = 2035){
		
		$db = new Db();

		$co = 0;
		$t = new Db();

		$reading_time = strtotime(date("$year-$month-15"));

		$sql = "SELECT mp_location,mp_id,mp_region_id,rea_mp_id, rea_date, rea_id FROM metering_point, r_reading WHERE mp_customer_id = '$customer_id' AND rea_cus_id = '$customer_id' AND mp_id = rea_mp_id AND rea_date = '$reading_time' ORDER BY mp_location ASC";

		$sel = $db->select($sql);
				
		$sha1 = array();
		$sha2 = array();
	
		foreach($sel[0] as $r){
			extract($r);	

			$cal = rate_calculate($customer_id, ($rea_date), $mp_id); 

			$sha1[] = $cal[33];
			$sha2[] = $cal[34];

			$total_s += ($cal[33] - $cal[34]);
		}

		$s1 = $sha1[1] - $sha1[0];
		$s2 = $sha2[1] - $sha2[0];

		return  $total_s;


	}

	public function advanceScheduleAction(){
		
		$class = ($this->set_class)?$this->set_class:portion(1);
		$method = ($this->set_method)?$this->set_method:portion(2);
		$id = ($this->set_customer)?$this->set_customer:portion(3);
		$year = ($this->set_year)?$this->set_year: portion(4);
		$month = ($this->set_month)?$this->set_month: portion(5);
		$note = ($this->set_note)?$this->set_note: portion(6);

		if(empty($year) || empty($month) || empty($id)){
			Feedback::error("Please select Customer, Year and Month");
			$units = $this->customerAndSchedule($id, $year, $month);
			$this->energyLinks($id, $year, $month);
			$units = 0.001;
			echo '<div class="clearfix"></div>';
		}elseif(!$this->readingExist($id, $year, $month)){
			Feedback::error("No readings found");
			$units = $this->customerAndSchedule($id, $year, $month);
			$this->energyLinks($id, $year, $month);
			$units = 0.001;
			echo '<div class="clearfix"></div>';
		}else{
			if($note=="credit-note"){
				$this->advanceScheduleCreditNote($id, $year, $month);
			}else{
				$this->advanceScheduleNormal($id, $year, $month);
			}
		}
	}

	public function advanceScheduleNormal($id, $year, $month){

		$db = new Db();
		$select = $db->select("SELECT * FROM customer WHERE customer_id = '$id'");
		extract($select[0][0]);

		// echo '<pre>';
		// print_r($this->totalBackflow($id, $year, $month));
		// echo '</pre>';
	?>
	<div id="side">
		<?php 
		//echo strtotime('2022-09-01');
		$units = $this->customerAndSchedule($id, $year, $month);
		$this->energyLinks($id, $year, $month);

		?>
		<br/>
		<style type="text/css">.schedule{font-size: 12px !important;}</style>
		<div id="printMe">
						
				<?php 
				echo '<title>'.strtoupper($customer_short_name.' Advance Schedule ').' - '.($month).' '.$year.'</title>';

				$count = 1;
				$db = new Db();
				$select = $db->select("SELECT * FROM metering_point, r_reading WHERE mp_customer_id = '$customer_id' AND rea_cus_id = '$customer_id' AND mp_id = rea_mp_id ORDER mp_location ASC");

				$total_mp = 0;
				foreach($select[0] as $row){
					extract($row);
					if(date('Y', $rea_date)==$year && date('m', $rea_date)==$month){
						$total_mp++;
					}
				}
				$units = 0.001;

				$tou = $this->tou($customer_id, $month, $year);
		
				$peak = $tou[0];
				$shoulder = $tou[1];
				$off_peak = $tou[2];

				?>
				<style>table tr td, table tr th{ height: 10px !important;} table tr th{text-align: center;} table trd:hover tdd{ background-color: #f9ff95; !important;} @media print{ #hint,.add-comment, .add-hint,.net-advance-form{display: none; }}</style>
			<center><h3 class="hide">UGANDA ELECTRICITY TRANSMISSION COMPANY LTD</h3></center>

			<?php 
			if($customer_id == 2030){//KPLC
				$g = "<center><h4>";
				$g .= $customer_short_name;
				$g .= ' BILL FOR THE MONTH OF ';
				$g .= month_name($month);
				$g .= ' ';
				$g .= $year;

				$g .= "</h4></center>";
				echo strtoupper($g);

				$a = $this->KPLC($year, $month);

				echo '<style>table tr td{ border: 2px solid #000; }</style>';

				echo '<table border="1" style="margin:auto; font-size:20px; font-weight:bold;" cellpadding="2" cellspacing="0">';

				echo '<tr valign="bottom">';
				echo '<td style="border:2px solid #000;" rowspan="2" align="center" style="min-width:200px">Time Band</td>';
				echo '<td style="border:2px solid #000;" colspan="3"><center>Advance (KWh)</center></td>';
				echo '</tr>';

				echo '<tr>';
				echo '<td style="border:2px solid #000;min-width:150px;" align="center">Main Meter</td>';
				echo '<td style="border:2px solid #000;min-width:150px;" align="center">Check Meter</td>';
				echo '<td style="border:2px solid #000;min-width:150px;" align="center">Average</td>';
				echo '</tr>';

				echo '<tr style="height:100px;">';
				echo '<td style="border-bottom:1px solid #ccc;">EXPORT<BR/>Peak (18:00 -  23:00 hrs)</td>';
				echo '<td style="border-bottom:1px solid #ccc;" align="right">'.number_format($a['main'][1]/1000).'</td>';
				echo '<td style="border-bottom:1px solid #ccc;" align="right">'.number_format($a['check'][1]/1000).'</td>';
				echo '<td style="border-bottom:1px solid #ccc;" align="right">'.number_format($a['average'][1]/1000).'</td>';
				echo '</tr>';

				echo '<tr style="height:100px;">';
				echo '<td style="border-top:1px solid #ccc; border-bottom:1px solid #ccc;">Day (05:00 -  18:00 hrs)</td>';
				echo '<td style="border-top:1px solid #ccc; border-bottom:1px solid #ccc;" align="right">'.number_format($a['main'][0]/1000).'</td>';
				echo '<td style="border-top:1px solid #ccc; border-bottom:1px solid #ccc;" align="right">'.number_format($a['check'][0]/1000).'</td>';
				echo '<td style="border-top:1px solid #ccc; border-bottom:1px solid #ccc;" align="right">'.number_format($a['average'][0]/1000).'</td>';
				echo '</tr>';

				echo '<tr style="height:100px;">';
				echo '<td style="border-top:1px solid #ccc;">Night (23:00 -  05:00 hrs)</td>';
				echo '<td style="border-top:1px solid #ccc;" align="right">'.number_format($a['main'][2]/1000).'</td>';
				echo '<td style="border-top:1px solid #ccc;" align="right">'.number_format($a['check'][2]/1000).'</td>';
				echo '<td style="border-top:1px solid #ccc;" align="right">'.number_format($a['average'][2]/1000).'</td>';
				echo '</tr>';

				$tm = 0;
				for($i=0; $i<3; $i++)
					$tm += $a['main'][$i];

				$ta = 0;
				for($i=0; $i<3; $i++)
					$tc += $a['check'][$i];

				$ta = ($tm + $tc)/(2);

				echo '<tr style="height:100px;">';
				echo '<td>Total</td>';
				echo '<td align="right">'.number_format($tm/1000).'</td>';
				echo '<td align="right">'.number_format($tc/1000).'</td>';
				echo '<td align="right">'.number_format($ta/1000).'</td>';
				echo '</tr>';

				echo '</table>';


			}else if($customer_id == 2035){//UECL MIRAMA
				$g = "<center><h4>";
				$g .= $customer_short_name;
				$g .= ' BILL FOR THE MONTH OF ';
				$g .= month_name($month);
				$g .= ' ';
				$g .= $year;

				$g .= "</h4></center>";
				echo strtoupper($g);

				echo '<style>table tr td{ border: 2px solid #000; }</style>';

				echo '<table border="1" style="font-family:arial; margin:auto; font-size:20px;" cellpadding="2" cellspacing="0">';

				echo '<tr valign="bottom">';
				echo '<th style="border:2px solid #000;" align="center">Metering Node</th>';
				echo '<th style="border:2px solid #000;" align="center">Cummulative Imports</th>';
				//echo '<th style="border:2px solid #000;" align="center">Cummulative Exports</th>';
				//echo '<th style="border:2px solid #000;"><center>Import Advance (KWh)</center></th>';
				echo '</tr>';


				$tc1 = $tc2 = $tc3 = $tc4= $tc5= $tc6= $tc7= $tc8 = 0;

				$total_mp;
				$db = new Db();

				$co = 0;
				$t = new Db();

				$reading_time = strtotime(date("$year-$month-15"));

				$sql = "SELECT mp_location,mp_id,mp_region_id,rea_mp_id, rea_date, rea_id FROM metering_point, r_reading WHERE mp_customer_id = '$customer_id' AND rea_cus_id = '$customer_id' AND mp_id = rea_mp_id AND rea_date = '$reading_time' ORDER BY mp_location ASC";

				$sel = $db->select($sql);
				
				$sha1 = array();
				$sha2 = array();
				$xx1 = 0;
				$xx2 = 0;
				$xx3 = 0;
				foreach($sel[0] as $r){
					extract($r);	

					$cal = rate_calculate($customer_id, ($rea_date), $mp_id); 

					$sha1[] = $cal[33];
					$sha2[] = $cal[34];
					echo '<tr>';
					echo '<td>'.$mp_location.'</td>';
					echo '<td style="text-align:right;">'.number_format($cal[33]/1000).'</td>';
					//echo '<td style="text-align:right;">'.number_format($cal[34]/1000).'</td>';
					//echo '<td style="text-align:right;">'.number_format(($cal[33] - $cal[34])/1000).'</td>';
					echo '</tr>';
					$xx1 += $cal[33]/1000;
					$xx2 += $cal[34]/1000;
					$xx3 += ($cal[33] - $cal[34])/1000;
				}

				$s1 = $sha1[1] - $sha1[0];
				$s2 = $sha2[1] - $sha2[0];

				echo '<tr>';
				echo '<td>Total:</td>';
				// echo '<td style="text-align:right;">'.number_format($s1/1000).'</td>';
				// echo '<td style="text-align:right;">'.number_format($s2/1000).'</td>';
				// echo '<td style="text-align:right; background-color:yellow;">'.number_format(($s1-$s2)/1000).'</td>';
				echo '<td style="text-align:right; background-color:yellow;">'.number_format($xx1).'</td>';
				//echo '<td style="text-align:right;">'.number_format($xx2).'</td>';
				//echo '<td style="text-align:right;">'.number_format($xx3).'</td>';
				echo '</tr>';



				echo '</table>';


			}else{
			?>
			<table id="sortSpan" border="1" cellspacing="0" cellpadding="2" class="table schedule" style="font-size: 10px;border:none;font-family: arial; background-color:#fff;">
				<thead>
				<tr valign="bottom">
					<th colspan="7" style="border:2px solid black;">
					<?php
					$g = "";
					$g .= $customer_full_name;
					$g .= ' Advance Schedule ';
					$g .= $year;
					$g .= ' - ';
					$g .= month_name($month);
					echo $g;
					?>
					</th>
					<th rowspan="5" style="width:20px;border:none;"></th>	
					<th rowspan="2" style="border:none;"></th>	
					<th rowspan="2" style="border:none;"></th>	
					<th rowspan="2" style="border:none;"></th>				
				</tr>
				<tr valign="bottom">
					<th rowspan="4" style="border-left:2px solid black;border-bottom:2px solid black;">No.</th>	
					<th rowspan="4" style="border-left:2px solid black;border-bottom:2px solid black;">Metering Points</th>			
					<th style="border-left:2px solid black; border-right:2px solid black;" colspan="5">Advance</th>				
				</tr>
				<tr>
					<th style="border-left:2px solid black; border-right:2px solid black;" colspan="5">Imports</th>	

					<?php if($customer_id != 2031){ ?>					
					<th colspan="2" style="border-top:2px solid black; border-right:2px solid black;border-left:2px solid black;">Wheeling Charge</th>	
					<th colspan="2" style="border-top:2px solid black; border-right:2px solid black;border-left:2px solid black;">Wheeling Charge</th>		
					<?php } ?>	
				</tr>
				<tr valign="bottom">
					<?php
						
						 
						if(1){
							if($peak != (int)$peak){
								$peak1 = number_format($peak, 5);
							}else{
								$peak1 = $peak;
							}
							if($shoulder != (int)$shoulder){
								$shoulder1 = number_format($shoulder, 5);
							}else{
								$shoulder1 = $shoulder;
							}
							if($off_peak != (int)$off_peak){
								$off_peak1 = number_format($off_peak, 5);
							}else{
								$off_peak1 = $off_peak;
							}
						}else{
							if($peak != (int)$peak){
								$peak1 = number_format($peak, 5);
							}else{
								$peak1 = $peak;
							}
							if($shoulder != (int)$shoulder){
								$shoulder1 = number_format($shoulder, 5);
							}else{
								$shoulder1 = $shoulder;
							}
							if($off_peak != (int)$off_peak){
								$off_peak1 = number_format($off_peak, 5);
							}else{
								$off_peak1 = $off_peak;
							}
						}
					?>
					<?php $symbol = $this->rgf("currency",$customer_currency, "currency_id", "currency_symbol"); ?>
					<th style="border-left:2px solid black;border-bottom:2px solid black;">Shoulder<br/><span style="color:red;">@ <?php echo (float)$shoulder1.'<br/>';?></span><?php echo $symbol;?>/kWh</th>
					<th style="border-bottom:2px solid black;">Peak<br/><span style="color:red;">@ <?php echo (float)$peak1.'<br/>';?></span><?php echo $symbol;?>/kWh</th>
					<th style="border-bottom:2px solid black;">Off Peak<br/><span style="color:red;">@ <?php echo (float)$off_peak1.'<br/>';?></span><?php echo $symbol;?>/kWh</th>
					<th style="border-bottom:2px solid black;">Advance<br/>(kWh)</th>
					<th style="border-right:2px solid black;border-bottom:2px solid black;">Amount<br/>(<?php echo $symbol;?>)</th>


					<?php if($customer_id != 2031){ ?>	

					<th style="border-left:2px solid black;border-bottom:2px solid black;">Imports<br/>(Payable)<br/><b>(<?php echo $symbol;?>)</b></th>				
					<th style="border-right:2px solid black;border-bottom:2px solid black;">Payable<br/>Customer</th>
					<th style="border-bottom:2px solid black;">Exports<br/>(Payable)<br/><b>(<?php echo $symbol;?>)</b></th>
					<th style="border-right:2px solid black;border-bottom:2px solid black;">Payable<br/>Customer</th>
					<?php } ?>
				</tr>
				</thead>
				<tbody>
				<?php

				$db_values = array();
				$db_values[] = array(
					"No.", 
					"Metering Points", 
					"Shoulder<br/><span style='color:yellow;'>@ ".$shoulder1."<br/></span>".$symbol."/kWh",
					"Peak<br/><span style='color:yellow;'>@ ".$peak1."<br/></span>".$symbol."/kWh",
					"Off Peak<br/><span style='color:yellow;'>@ ".$off_peak1."<br/></span>".$symbol."/kWh",
					"Advance<br/>(kWh)",
					"Amount<br/>".$symbol,
					"EMP",
					"Percentage",
					"Imports<br/>(Payable)<br/><b>".$symbol."</b>",
					"Exports<br/>(Collectable)<br/><b>".$symbol."</b>"
				);

				$tc1 = $tc2 = $tc3 = $tc4= $tc5= $tc6= $tc7= $tc8 = 0;

				$total_mp;
				$db = new Db();

				$co = 0;
				$t = new Db();

				$reading_time = strtotime(date("$year-$month-15"));

				$sel = $t->select("SELECT DISTINCT mp_region_id FROM metering_point, r_reading, meter_region WHERE mp_customer_id = '$customer_id' AND rea_cus_id = '$customer_id' AND mp_id = rea_mp_id  AND rea_date = '$reading_time' AND mp_region_id IS NOT NULL AND mp_region_id = region_id ORDER BY region_order ASC");
				
				
				$sel = $t->select("SELECT * FROM meter_region ORDER BY region_order ASC");
	
				foreach($sel[0] as $r){
				extract($r);
			
				if((!$this->isThere("metering_point", ["mp_region_id"=>$region_id]))){
					continue;
				}else{
					$mp_region_id = $region_id;
				}

				if($customer_id == 2019 || $customer_id == 2036 || $customer_id == 2040 || $customer_id == 2041){
					$select = $db->select("SELECT mp_location,mp_id,mp_region_id,rea_mp_id, rea_date, rea_id FROM metering_point, r_reading WHERE mp_customer_id = '$customer_id' AND rea_cus_id = '$customer_id' AND mp_id = rea_mp_id AND mp_region_id = '$mp_region_id' AND rea_date = '$reading_time' ORDER BY mp_location ASC");

				echo '<tr><td style="border-left:none; border-right:none; border-bottom: 2px solid #000;"></td>
					<td colspan="6" style="border-left:none; border-right:none; border-bottom: 2px solid #000;color:red;"><b>'.strtoupper($this->rgf("meter_region", $mp_region_id, "region_id", "region_name")).'</b></td>
					<td style="border:none;"></td>
					<td colspan="3" style="border-left:none; border-right:none; border-bottom: 2px solid #000;"></td></tr>';

				$db_values[] = array("", '&nbsp;<Br/><span style="color:red;font-weight: bold">'.strtoupper($this->rgf("meter_region", $mp_region_id, "region_id", "region_name")).'</span>',
				'','','','','','',
				//'&nbsp;<Br/><span style="color:red;font-weight: bold">'.strtoupper($this->rgf("meter_region", $mp_region_id, "region_id", "region_name")).'</span>',
				);
				}else{
					$select = $db->select("SELECT mp_location,mp_id,mp_region_id,rea_mp_id, rea_date, rea_id FROM metering_point, r_reading WHERE mp_customer_id = '$customer_id' AND rea_cus_id = '$customer_id' AND mp_id = rea_mp_id AND mp_region_id = '$mp_region_id' AND rea_date = '$reading_time' ORDER BY mp_location ASC");
				}

				$tw = 0;
				

				$total_amount = $total_amount2 = $total_wheeling = $total_wheeling2 = $wheeling2 = 0;
				$tc1 = $tc2 = $tc3 = $tc4= $tc5= $tc6= $tc7= $tc8 = 0;
				foreach($select[0] as $row){
					extract($row);


						$cal = rate_calculate($customer_id, ($rea_date), $mp_id);

					$v = $this->get_advance($customer_id, $mp_id, date('m',$rea_date), date('Y',$rea_date), $cal[23]);

					$g0 = ($v['AR1']);
					$g1 = ($v['AR2']);
					$g2 = ($v['AR3']);

					if(($customer_id == 2036 || $customer_id == 2041) && ($g0+$g1+$g2 == 0)){

					}else{
						//selecting rates

						if($co%2 != 0){
							$df = "background-color: #e6e6e6;";
						}else{
							$df = "background-color:#fff;";
						}
						
						// $db = new Db();
						// $sel = $db->select("SELECT rate_id FROM r_rate WHERE rate_reading_id = '$rea_id'");
						// @extract($sel[0][0]);
						
						//calling the function
											
						
						// echo '<pre>';
						// print_r($cal); echo '</pre>';
						//if($count != $total_mp){
						if(1){		
							$less = ($region_id == 10 || $region_id == 8)?"(Less) ":"";

							if($cal[21]){ //new label name
								$mp_location2 = $less.'<span title="'.$mp_location.'">'.str_replace(' ', '&nbsp;', strtoupper($cal[21])).'*</span>';
							}else{
								$mp_location2 = $less.''.str_replace(' ', '&nbsp;', $mp_location);
							}
							
							if($cal[23] == "([DONOT_DISPLAY])"){
								echo '<tr id="'.($rea_id+5).'" style="display:none">';
							}else{
								echo '<tr id="'.($rea_id+5).'">';
								$co++;
							}
								
							
							echo '<td style="'.$df.' border-left:2px solid #000;">'.($co).'.</td>';
							//echo "UPDATE r_rate SET rate_sfi = '([E@$mp_id])', rate_lfe ='([Exports@$mp_location])' WHERE rate_reading_id = '".$cal[26]."'; ";
							echo '<td style="'.$df.' border-left:2px solid black;">'.$mp_location2;

							//rc_year = '$year' AND rc_month = '$month' AND 
							$d =  new Db();
							$se = $d->select("SELECT * FROM readings_comment WHERE rc_mp_id = '$mp_id' AND rc_customer_id = '$customer_id' AND rc_month = '$month' AND rc_year = '$year' AND rc_last IS NULL");
						

							if($d->num_rows()){
								echo '<form action="" method="post" style="padding:0; margin:0;">';

								if(isset($_POST['delete'.$mp_id])){

									 $t = $_POST["rc$mp_id"];
									 $customer = $_POST["customer$mp_id"];
									 $readings = $_POST["rea_id$mp_id"];


									$db = new Db();
									$sql = "DELETE FROM readings_comment WHERE rc_id = '$t'";
									$delete = $db->query($sql);

									$select = $db->update("r_rate",[
										"rate_advance_1"=>NULL,
										"rate_advance_2"=>NULL,
										"rate_advance_3"=>NULL,
										"rate_advance_4"=>NULL,
										"rate_advance_5"=>NULL,
										"rate_advance_6"=>NULL
									], ["rate_cus_id"=>$customer, "rate_reading_id"=>$readings]);

									FeedBack::redirect(return_url()."/".portion(1)."/".portion(2)."/".portion(3)."/".portion(4)."/".portion(5)."#$readings");
								}

								echo '<div id="hint" style=""><div class="message">';
								echo '<p style="font-weight:bold;">'.$mp_location.'</p>';
								foreach($se[0] as $r){
									extract($r);
									echo '<b>'.$this->full_name($rc_added_by).':</b><br/>';
									echo nl2br($rc_comment);
									echo '<br/>';
									echo '<span style="color:#aaa;">'.Feedback::date_fm($rc_date_added).'</span>';
								}

								echo '<hr style="margin:0; padding:0; margin-top:8px;"/>';
								$x = array('is', 'es', 'ip', 'ep', 'io', 'eo');
								$order = array(1, 4, 2, 5, 3,6);
								for($i=1; $i<=6; $i++){
									${"rate_advance_".$order[$i-1]} = $this->rgf("r_rate", $rea_id, "rate_reading_id", "rate_advance_".$order[$i-1]);
									echo '<div class="col-md-6" style="color:#aaa; padding:0; margin:0;">';
									echo strtoupper($x[$i-1]).': '.number_format(${"rate_advance_".$order[$i-1]}/1000, 2)."";
									echo '</div>';
								}
								echo '<div class="clearfix"></div>';

								echo '<input type="hidden" name="rc'.$mp_id.'" value="'.$rc_id.'" />';
								echo '<input type="hidden" name="customer'.$mp_id.'" value="'.$customer_id.'" />';
								echo '<input type="hidden" name="rea_id'.$mp_id.'" value="'.$rea_id.'" />';

								if(!$this->isLocked($customer_id, $year, $month)){

								echo '<input type="hidden" id="year'.$mp_id.'" value="'.$year.'">';
								echo '<input type="hidden" id="month'.$mp_id.'" value="'.$month.'">';
								echo '<input type="hidden" id="customer_id'.$mp_id.'" value="'.$id.'">';
								echo '<input type="hidden" id="rea_id'.$mp_id.'" value="'.$rea_id.'">';
								echo '<input type="hidden" id="user_id" value="'.user_id().'">';
								echo '<div style="width:30px; color:white;"><form>
								<button type="button" class="edit-hint btn-success"  id="edit-hint'.$co.'"  data-comment-id="'.$rc_id.'"  data-id="'.$rea_id.'" name="edit'.$mp_id.'"><i class="fa fa-fw fa-pencil"></i></button></form></div>';
                             ?>
                             <script type="text/javascript">
								 $(document).off('click','#<?php echo 'edit-hint'.$co; ?>').on('click', '#<?php echo 'edit-hint'.$co; ?>', function(e){
								 	var urlPath = $('#urlPath').val();
								 	var id = $(this).attr('data-id');
								 	var comment = $(this).attr('data-comment-id');
								 	var form_data = new FormData();
							        form_data.append('id', id);
							        form_data.append('comment', comment);
								 	 $.ajax({
							            url: urlPath+"ajax/selectNetAdvance.php",
							            type: "POST",
							            data: form_data,
							            contentType: false,
							            cache: false,
							            processData:false,
							            success: function(data){
							            	 var values = JSON.parse(data);
							             	 document.getElementById("rate__id").value = values.rate_idd;
							                 document.getElementById("comment__id").value = values.comment_idd;
							                 document.getElementById("<?php echo 'myModal_'.$co; ?>").style.display = "block";
							            }
							        });
	                            });
	                        </script>
							
                             <?php	
								echo '<div id="add-new-hint"><button type="button" class="delete-net-advance" data-comment-id="'.$rc_id.'"  data-id="'.$rea_id.'" name="delete'.$mp_id.'"><i class="fa fa-fw fa-times"></i></button></div>';
								?>
								<div id="myModal_<?php echo $co; ?>" style="" class="net-advance-form modal">
								  <div class="modal-content" style="width:40%;">
								    <span id="close_<?php echo $co; ?>" class="close" style="color:red;">&times;</span>
								    <p></p>
								 
							<?php
								echo '<div>'; 
								
                    $current_customer = $this->rgf('customer', $id, 'customer_id', 'customer_short_name');
                    if($current_customer){
                        $type = $this->set_method;
                        $year;
                        $month;
                        $sep = "/";
                    }

                    echo "<h2 class='text-dark text-center' style='font-size:2em;padding:0 20px; font-weight:bold;color:black;'> $current_customer $type $month$sep$year</h2>";
                				//echo ">>>".$rc_id.">>>".$ccc;
								echo '<form action=""  method="post" style="padding:0; margin:0;">';
								echo '<input type="hidden" id="rate__id" value="">';
								echo '<input type="hidden" id="comment__id" value="">';
								echo '<input type="hidden" id="year'.$mp_id.'" value="'.$year.'">';
								echo '<input type="hidden" id="month'.$mp_id.'" value="'.$month.'">';
								echo '<input type="hidden" id="customer_id'.$mp_id.'" value="'.$id.'">';
								echo '<input type="hidden" id="rea_id'.$mp_id.'" value="'.$rea_id.'">';
								echo '<input type="hidden" id="user_id" value="'.user_id().'">';
								
								echo '<div class="add-comment" style="color:black;">';
								echo '<div style="padding:10px;">';
								echo '<p style="font-weight:bold; color:blue;"> Edit details for '.$mp_location.'</p>';
								echo '<b>Add Comment</b><Br/> ';
								echo '<textarea required style="width:100%; padding:10px;" placeholder="comment" name="comment" id="comment'.$mp_id.'">'.$rc_comment.'</textarea>';
								// echo '<b>Show Comment On:</b><br/> &nbsp; &nbsp; &nbsp; ';

								// $show = array("Sch", "WC", "INV");
								// for($i=0; $i<count($show); $i++){
								// 	echo '<input class="chk-col-blue" type="checkbox" value="'.$i.'" name="showon[]" id="invoice'.$i.$mp_id.'"/><label for="invoice'.$i.$mp_id.'">'.$show[$i].' &nbsp; &nbsp; </label>';
								// }

								echo '<br/><br/>Advance Imports: <b>(KWh)</b><br/>';
								echo '<input class="number" value="'.number_format($rate_advance_1/1000).'" style="width:30%; min-width:100px; padding:1%; margin:1%;" placeholder="Shoulder" name="is" id="is'.$mp_id.'" autocomplete="off" />';
								echo '<input class="number" value="'.number_format($rate_advance_2/1000).'" style="width:30%; min-width:100px; padding:1%; margin:1%;" placeholder="Peak" name="ip" id="ip'.$mp_id.'" autocomplete="off" />';
								echo '<input class="number" style="width:30%; min-width:100px; padding:1%; margin:1%;" placeholder="Off Peak" value="'.number_format($rate_advance_3/1000).'" name="io" id="io'.$mp_id.'" autocomplete="off" />';

								echo '<br/>Advance Exports: <b>(KWh)</b><br/>';
								echo '<input class="number" value="'.number_format($rate_advance_4/1000).'" style="width:30%; min-width:100px; padding:1%; margin:1%;" placeholder="Shoulder" id="es'.$mp_id.'" name="es" autocomplete="off"/>';
								echo '<input class="number" value="'.number_format($rate_advance_5/1000).'" style="width:30%; min-width:100px; padding:1%; margin:1%;" placeholder="Peak"  id="ep'.$mp_id.'" name="ep" autocomplete="off" />';
								echo '<input class="number" value="'.number_format($rate_advance_6/1000).'" style="width:30%; min-width:100px; padding:1%; margin:1%;" placeholder="Off Peak" name="eo" id="eo'.$mp_id.'" autocomplete="off" />';

								echo '<br/><br/><button type="button" data-id = "'.$mp_id.'" class="editNetAdvanceBtn"><i class="fa fa-fw fa-save"></i> Save</button><span id="netAdvanceBtnStatus'.$mp_id.'"></span>';

								echo '</div>';
								echo '</div>';
								echo '</div>';
								
								echo '</div>';

								echo '</form>';

								?>


								 </div>

								</div>

								<script>
							 //   function	edit_me_now(){		
								// var cc document.getElementById("<?php echo 'edit-hint'.$co; ?>");
								// 	alert(cc.value);
								//   document.getElementById("<?php echo 'myModal_'.$co; ?>").style.display = "block";
								
							 //  }
								document.getElementById("<?php echo 'close_'.$co; ?>").onclick = function() {
								  document.getElementById("<?php echo 'myModal_'.$co; ?>").style.display = "none";
								}
								window.onclick = function(event) {
								  if (event.target == document.getElementById("<?php echo 'myModal_'.$co; ?>")) {
								    document.getElementById("<?php echo 'myModal_'.$co; ?>").style.display = "none";
								  }
								}
							</script>
							
								<?php

								}
								echo '</div></div>';
								echo '</form>';
							}else{ 
								if(!$this->isLocked($customer_id, $year, $month)){
								echo '<div class="add-hint" id="add-hint'.$co.'" style="color:white;"><i class="fa fa-fw fa-plus"></i></div>';
								}
							?>

								<div id="myModal<?php echo $co; ?>" class="net-advance-form modal">
								  <div class="modal-content" style="width:40%;">
								    <span id="close<?php echo $co; ?>" class="close" style="color:red;">&times;</span>
								    <p></p>
								 
							<?php
								echo '<div>'; 
								
                    $current_customer = $this->rgf('customer', $id, 'customer_id', 'customer_short_name');
                    if($current_customer){
                        $type = $this->set_method;
                        $year;
                        $month;
                        $sep = "/";
                    }

                    echo "<h2 class='text-dark text-center' style='font-size:2em;padding:0 20px; font-weight:bold;color:black;'> $current_customer $type $month$sep$year</h2>";
                
								echo '<form action=""  method="post" style="padding:0; margin:0;">';

								echo '<input type="hidden" id="year'.$mp_id.'" value="'.$year.'">';
								echo '<input type="hidden" id="month'.$mp_id.'" value="'.$month.'">';
								echo '<input type="hidden" id="customer_id'.$mp_id.'" value="'.$id.'">';
								echo '<input type="hidden" id="rea_id'.$mp_id.'" value="'.$rea_id.'">';
								echo '<input type="hidden" id="user_id" value="'.user_id().'">';
								
								echo '<div class="add-comment" style="color:black;">';
								echo '<div style="padding:10px;">';
								echo '<p style="font-weight:bold; color:blue;">'.$mp_location.'</p>';
								echo '<b>Add Comment</b><Br/> ';
								echo '<textarea required style="width:100%; padding:10px;" placeholder="comment" name="comment" id="comment'.$mp_id.'"></textarea>';
								// echo '<b>Show Comment On:</b><br/> &nbsp; &nbsp; &nbsp; ';

								// $show = array("Sch", "WC", "INV");
								// for($i=0; $i<count($show); $i++){
								// 	echo '<input class="chk-col-blue" type="checkbox" value="'.$i.'" name="showon[]" id="invoice'.$i.$mp_id.'"/><label for="invoice'.$i.$mp_id.'">'.$show[$i].' &nbsp; &nbsp; </label>';
								// }

								echo '<br/><br/>Advance Imports: <b>(KWh)</b><br/>';
								echo '<input class="number" style="width:30%; min-width:100px; padding:1%; margin:1%;" placeholder="Shoulder" name="is" id="is'.$mp_id.'" autocomplete="off" />';
								echo '<input class="number" style="width:30%; min-width:100px; padding:1%; margin:1%;" placeholder="Peak" name="ip" id="ip'.$mp_id.'" autocomplete="off" />';
								echo '<input class="number" style="width:30%; min-width:100px; padding:1%; margin:1%;" placeholder="Off Peak" name="io" id="io'.$mp_id.'" autocomplete="off" />';

								echo '<br/>Advance Exports: <b>(KWh)</b><br/>';
								echo '<input class="number" style="width:30%; min-width:100px; padding:1%; margin:1%;" placeholder="Shoulder" id="es'.$mp_id.'" name="es" autocomplete="off"/>';
								echo '<input class="number" style="width:30%; min-width:100px; padding:1%; margin:1%;" placeholder="Peak"  id="ep'.$mp_id.'" name="ep" autocomplete="off" />';
								echo '<input class="number" style="width:30%; min-width:100px; padding:1%; margin:1%;" placeholder="Off Peak" name="eo" id="eo'.$mp_id.'" autocomplete="off" />';

								echo '<br/><br/><button type="button" data-id = "'.$mp_id.'" class="netAdvanceBtn"><i class="fa fa-fw fa-save"></i> Save</button><span id="netAdvanceBtnStatus'.$mp_id.'"></span>';

								echo '</div>';
								echo '</div>';
								echo '</div>';
								
								echo '</div>';

								echo '</form>';

								?>


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
							

							echo '</td>';


							if(1){

								$v = $this->get_advance($customer_id, $mp_id, $month, $year, $cal[23]);

								$g0 = ($v['AR1']);
								$g1 = ($v['AR2']);
								$g2 = ($v['AR3']);
								
								$g3 = $g1 + $g2 + $g0;
								$g4 = ($g0*$shoulder + $g1*$peak + $g2*$off_peak)/1000;
								$amount = ($g0*$shoulder+$g1*$peak+$g2*$off_peak)/1000; //imports
								$amount2 = ($cal[4]*$shoulder+$cal[5]*$peak+$cal[6]*$off_peak)/1000; //exports
								
								$tw += $g3*$mp_wheeling_charge/100;
																
								$total_amount += $amount;
								$total_wheeling += $amount*$mp_wheeling_charge/100;

								$wheeling2 = 0; //$amount2*$mp_wheeling_charge/100;

								$wc = $this->get_advanceWheeling($customer_id, $mp_id, $month, $year, $cal[27]);
								
								$wheeling2 = $wc['AMOUNT'];
								$total_wheeling2 += $wheeling2;

								$wcE = $this->get_advanceWheeling($customer_id, $mp_id, $month, $year, $cal[28]);
								
								$wheeling2E = $wcE['AMOUNT'];
								$total_wheeling2 += $wheeling2+$wheeling2E;

								echo '<td style="'.$df.'border-left:2px solid black; text-align:right;">'.number_format($g0*$units);
								//if(!$this->isLocked($customer_id, $year, $month))
								// {
								// echo '<div class="add-hint formula-space space2" style="color:white;"><i class="fa fa-fw fa-calculator"></i>
								// 	<div class="formula-message">
								// 		<div class="area">';
								// 		echo $rate_lfi;
								// 		//$this->formulaDisplay($customer_id, $mp_id, $year, $month);
								// 		echo '</div></div>
								// </div>';
								// }
								echo '</td>';
								echo '<td style="'.$df.' text-align:right;">'.number_format($g1*$units).'</td>';
								echo '<td style="'.$df.' text-align:right;">'.number_format($g2*$units).'</td>';

			
								echo '<td style="'.$df.' text-align:right;">'.number_format($g3*$units).'</td>';
								echo '<td style="'.$df.'border-right:2px solid black; text-align:right;">'.number_format($g4).'</td>';
							}

							echo '<td style="border:none;"></td>';
							//@@@@@@@@@@@@@@@@@@@
							//if( $this->payable($mp_id, $customer_id,0, $year, $month))
							if($customer_id != 2031){

								echo '<td style="border-left:2px solid #000;'.$df.'text-align: right;" title="'.$cal[29].'">
								'.number_format($wheeling2).'</td>';

								echo '<td style="'.$df.'border-right:2px solid black;text-align: right;">'.$wc['PAYABLE'].'</td>';	
								echo '<td style="border-left:2px solid #000;'.$df.'text-align: right;" title="'.$cal[30].'">
								'.number_format($wheeling2E).'</td>';

								echo '<td style="'.$df.'border-right:2px solid black;text-align: right;">'.$wcE['PAYABLE'].'</td>';	

							}

							echo '</tr>';

							$tc1 += $g0;
							$tc2 += $g1;
							$tc3 += $g2;
							$tc4 += $g3;

							$tc5 += $cal[4];
							$tc6 += $cal[5];
							$tc7 += $cal[6];
							$tc8 += $cal[7];
						}

						$db_values[] = array(
							$co, 
							$mp_location,
							number_format($cal[0]*$units),
							number_format($cal[1]*$units),
							number_format($cal[2]*$units),
							number_format($cal[3]*$units),
							number_format($amount),
							"",
							number_format($mp_wheeling_charge,2).'&nbsp;%',
							number_format($amount*$mp_wheeling_charge/100),
							number_format($wheeling2)
						);

					}					
				}
				
				// $l = $this->sumFormulae($customer_id, $year, $month);

				// $tc1 = $l[0];
				// $tc2 = $l[1];
				// $tc3 = $l[2];
				// $tc4 = $l[3];
				// $tc5 = $l[4];
				// $tc6 = $l[5];
				// $tc7 = $l[6];
				// $tc8 = $l[7];

				$tot = ($tc1*$shoulder+$tc2*$peak+$tc3*$off_peak)/1000;

				if($co%2 != 0){
					$df = "background-color: #e6e6e6;";
				}else{
					$df = "background-color:#fff;";
				}
				//$co++;

				if($customer_id == 2019 || $customer_id == 2036 || $customer_id == 2040 || $customer_id == 2041){

				echo '<tr>';
				echo '<td style="'.$df.'border-top:2px solid #000; border-left:2px solid #000;border-bottom:2px solid black;border-top:2px solid #000;"></td>';
				echo '<td style="'.$df.'border-top:2px solid #000; border-left:2px solid black; border-bottom:2px solid black; text-align:right;font-weight: bold; ">Total</td>';
				
				echo '<td style="'.$df.'border-top:2px solid #000; border-bottom:2px solid black;  border-left:2px solid black;  text-align:right;font-weight: bold;">'.number_format($tc1*$units).'</td>';
				echo '<td style="'.$df.'border-top:2px solid #000; border-bottom:2px solid black;  text-align:right;font-weight: bold;">'.number_format($tc2*$units).'</td>';
				echo '<td style="'.$df.'border-top:2px solid #000; border-bottom:2px solid black;  text-align:right;font-weight: bold;">'.number_format($tc3*$units).'</td>';
				echo '<td style="'.$df.'border-top:2px solid #000; border-bottom:2px solid black;  text-align:right;font-weight: bold;">'.number_format($tc4*$units).'</td>';
				echo '<td style="'.$df.'border-top:2px solid #000; border-right:2px solid black; border-bottom:2px solid black; text-align:right; font-weight: bold; ">'.number_format($tot).'</td>';

				echo '<td style="border:none;"></td>';
				echo '<td colspan="3" style="border:none;border-top:2px solid #000; "></td>';

				echo '</tr>';
				}

				}
				
				$l = $this->sumFormulae($customer_id, $year, $month);

				$tc1 = $l[0];
			 	$tc2 = $l[1];
				$tc3 = $l[2];
				$tc4 = $l[3];
				

				if(strtotime($year.'-'.$month.'-01') <= strtotime('2025-03-01'))
				$b = $this->sumFormulae(2036, $year, $month);
				else
				$b = $this->sumFormulae(2041, $year, $month);

				$tc5 = $b[0];
				$tc6 = $b[1];
				$tc7 = $b[2];
				$tc8 = $b[3];

				$a1 = $a2 = $a3 = $a4 = 0;

				// if(portion(3) == 2020){ //units from KRECS exports
				// 	$reading_time = strtotime(date("$year-$month-15"));
				// 	$t = rate_calculate(2022, $reading_time, 5315);

				// 	$a1 = $t[0];
				// 	$a2 = $t[1];
				// 	$a3 = $t[2];
				// 	$a4 = $t[3];
				// }

				$tot = (($tc1+$a1)*$shoulder+($tc2+$a2)*$peak+($tc3+$a3)*$off_peak)/1000;

				if($co%2 != 0){
					$df = "background-color: #e6e6e6;";
				}else{
					$df = "background-color:#fff;";
				}
				//$co++;

				echo '<tr style="">';
				echo '<td style="'.$df.'border-top:2px solid #000; border-left:2px solid #000;border-bottom:2px solid black;border-top:2px solid #000;"></td>';
				echo '<td style="'.$df.'border-top:2px solid #000; border-left:2px solid black; border-bottom:2px solid black; text-align:right;font-weight: bold; ">Gross Units</td>';
				
				echo '<td style="'.$df.'border-top:2px solid #000; border-bottom:2px solid black;  border-left:2px solid black;  text-align:right;font-weight: bold;">'.number_format(($tc1+$a1)*$units,2).'</td>';
				echo '<td style="'.$df.'border-top:2px solid #000; border-bottom:2px solid black;  text-align:right;font-weight: bold;">'.number_format(($tc2+$a2)*$units,2).'</td>';
				echo '<td style="'.$df.'border-top:2px solid #000; border-bottom:2px solid black;  text-align:right;font-weight: bold;">'.number_format(($tc3+$a3)*$units,2).'</td>';
				echo '<td style="'.$df.'border-top:2px solid #000; border-bottom:2px solid black;  text-align:right;font-weight: bold;">'.number_format(($tc4+$a4)*$units,2).'</td>';
				echo '<td style="'.$df.'border-top:2px solid #000; border-right:2px solid black; border-bottom:2px solid black; text-align:right; font-weight: bold; ">'.number_format($tot).'</td>';

				echo '<td colspan="4" style="border-top:2px solid #000; border:none;"></td>';
				
				echo '</tr>';

				echo '<tr><td colspan="7" style="border:none;"></td></tr>';

				if($id == 2019 || $id == 2040){
					echo '<tr>';
					echo '<td style="'.$df.'border-top:2px solid #000; border-left:2px solid #000;border-bottom:2px solid black;border-top:2px solid #000;"></td>';
					echo '<td style="'.$df.'border-top:2px solid #000; border-left:2px solid black; border-bottom:2px solid black; font-weight: bold; ">Backflows</td>';
					
					echo '<td style="'.$df.'border-top:2px solid #000; border-bottom:2px solid black;  border-left:2px solid black;  text-align:right;font-weight: bold;">'.number_format($tc5*$units,2).'</td>';
					echo '<td style="'.$df.'border-top:2px solid #000; border-bottom:2px solid black;  text-align:right;font-weight: bold;">'.number_format($tc6*$units,2).'</td>';
					echo '<td style="'.$df.'border-top:2px solid #000; border-bottom:2px solid black;  text-align:right;font-weight: bold;">'.number_format($tc7*$units,2).'</td>';
					echo '<td style="'.$df.'border-top:2px solid #000; border-bottom:2px solid black;  text-align:right;font-weight: bold;">'.number_format($tc8*$units,2).'</td>';
					echo '<td style="'.$df.'border-top:2px solid #000; border-right:2px solid black; border-bottom:2px solid black; text-align:right; font-weight: bold; ">'.number_format(($tc5*$shoulder+$tc6*$peak+$tc7*$off_peak)*$units,2).'</td>';

					echo '<td colspan="4" style="border-top:2px solid #000; border:none;"></td>';
					
					echo '</tr>';
					echo '<tr>';
					echo '<td style="'.$df.'border-top:2px solid #000; border-left:2px solid #000;border-bottom:2px solid black;border-top:2px solid #000;"></td>';
					echo '<td style="'.$df.'border-top:2px solid #000; border-left:2px solid black; border-bottom:2px solid black; font-weight: bold; ">Gross Units - Backflows</td>';
					
					echo '<td style="'.$df.'border-top:2px solid #000; border-bottom:2px solid black;  border-left:2px solid black;  text-align:right;font-weight: bold;">'.number_format(($tc1 - $tc5)*$units,2).'</td>';
					echo '<td style="'.$df.'border-top:2px solid #000; border-bottom:2px solid black;  text-align:right;font-weight: bold;">'.number_format(($tc2 - $tc6)*$units,2).'</td>';
					echo '<td style="'.$df.'border-top:2px solid #000; border-bottom:2px solid black;  text-align:right;font-weight: bold;">'.number_format(($tc3 - $tc7)*$units,2).'</td>';
					echo '<td style="'.$df.'border-top:2px solid #000; border-bottom:2px solid black;  text-align:right;font-weight: bold;">'.number_format(($tc4 - $tc8)*$units,2).'</td>';
					echo '<td style="'.$df.'border-top:2px solid #000; border-right:2px solid black; border-bottom:2px solid black; text-align:right; font-weight: bold; ">'.number_format((($tc1 - $tc5)*$shoulder+($tc2 - $tc6)*$peak+($tc3 - $tc7)*$off_peak)*$units,2).'</td>';

					echo '<td colspan="4" style="border-top:2px solid #000; border:none;"></td>';
					
					echo '</tr>';

				}


				if($id == 2020){ //units from KRECS exports
					echo '<tr>';
					echo '<td style="'.$df.'border-top:2px solid #000; border-left:2px solid #000;border-bottom:2px solid black;border-top:2px solid #000;"></td>';
					echo '<td style="'.$df.'border-top:2px solid #000; border-left:2px solid black; border-bottom:2px solid black; font-weight: bold; ">Units from KRECS(Imports)</td>';
					
					echo '<td style="'.$df.'border-top:2px solid #000; border-bottom:2px solid black;  border-left:2px solid black;  text-align:right;font-weight: bold;">'.number_format($a1*$units,2).'</td>';
					echo '<td style="'.$df.'border-top:2px solid #000; border-bottom:2px solid black;  text-align:right;font-weight: bold;">'.number_format($a2*$units,2).'</td>';
					echo '<td style="'.$df.'border-top:2px solid #000; border-bottom:2px solid black;  text-align:right;font-weight: bold;">'.number_format($a3*$units,2).'</td>';
					echo '<td style="'.$df.'border-top:2px solid #000; border-bottom:2px solid black;  text-align:right;font-weight: bold;">'.number_format($a4*$units,2).'</td>';
					echo '<td style="'.$df.'border-top:2px solid #000; border-right:2px solid black; border-bottom:2px solid black; text-align:right; font-weight: bold; ">'.number_format(($a1*$shoulder+$a2*$peak+$a3*$off_peak)*$units,2).'</td>';

					echo '<td colspan="4" style="border-top:2px solid #000; border:none;"></td>';
					
					echo '</tr>';
					echo '<tr>';
					echo '<td style="'.$df.'border-top:2px solid #000; border-left:2px solid #000;border-bottom:2px solid black;border-top:2px solid #000;"></td>';
					echo '<td style="'.$df.'border-top:2px solid #000; border-left:2px solid black; border-bottom:2px solid black; font-weight: bold; ">Net Units</td>';

					$a1 = $a2 = $a3 = $a4 = 0;
					
					echo '<td style="'.$df.'border-top:2px solid #000; border-bottom:2px solid black;  border-left:2px solid black;  text-align:right;font-weight: bold;">'.number_format(($tc1 - $a1)*$units,2).'</td>';
					echo '<td style="'.$df.'border-top:2px solid #000; border-bottom:2px solid black;  text-align:right;font-weight: bold;">'.number_format(($tc2 - $a2)*$units,2).'</td>';
					echo '<td style="'.$df.'border-top:2px solid #000; border-bottom:2px solid black;  text-align:right;font-weight: bold;">'.number_format(($tc3 - $a3)*$units,2).'</td>';
					echo '<td style="'.$df.'border-top:2px solid #000; border-bottom:2px solid black;  text-align:right;font-weight: bold;">'.number_format(($tc4 - $a4)*$units,2).'</td>';
					echo '<td style="'.$df.'border-top:2px solid #000; border-right:2px solid black; border-bottom:2px solid black; text-align:right; font-weight: bold; ">'.number_format((($tc1 - $a1)*$shoulder+($tc2 - $a2)*$peak+($tc3 - $a3)*$off_peak)*$units).'sdfsdfsd</td>';

					echo '<td colspan="4" style="border-top:2px solid #000; border:none;"></td>';
					
					echo '</tr>';

				}


				if($customer_id != 2031){

				echo '<tr>';
				echo '<td style="border:none; border-bottom:2px solid #000;"></td>';
				echo '<td colspan="6" style="border:none; border-bottom:2px solid #000;">&nbsp;<Br/><b>WHEELING CHARGE PAYABLE TO</b></td>';
			
				
				echo '<td colspan="5" style="border:none;"></td>';
				echo '</tr>';

				echo '<tr style="border:2px solid #000">';
				echo '<td style="border-right:2px solid #000; text-align:center; font-weight:bold;">No.</td>';
				echo '<td style="border-right:2px solid #000; text-align:center; font-weight:bold;">Customer</td>';
				echo '<td style="text-align:center; font-weight:bold;">Shoulder</td>';
				echo '<td style="text-align:center; font-weight:bold;">Peak</td>';
				echo '<td style="text-align:center; font-weight:bold;">Off Peak</td>';
				echo '<td style="text-align:center; font-weight:bold;">Advance</td>';
				echo '<td style="text-align:center; font-weight:bold;">Amount</td>';
				echo '</tr>';

				$customer_payables = array();

				$time_readings = strtotime(date($year.'-'.$month.'-15'));
				$db = new Db();
				$sql = "SELECT rea_mp_id, rate_wc_sfi,rate_wc_sfe FROM r_rate,r_reading WHERE rea_id = rate_reading_id AND rea_date = '$time_readings' AND rate_cus_id = '$customer_id' AND (rate_wc_sfi IS NOT NULL OR rate_wc_sfe IS NOT NULL) AND rate_lfi != '([DONOT_DISPLAY])'";
				$customer_payables = array();
				$select = $db->select($sql);
				foreach($select[0] as $row){
					extract($row);					
					$wc = $this->get_advanceWheeling($customer_id, $rea_mp_id, $month, $year, $rate_wc_sfi);
					if($rate_wc_sfi){
						$rate = explode('=>',$rate_wc_sfi);
						$rate = str_replace('([', '', $rate[1]);
						$rate = str_replace('])', '', $rate);
						$customer_payables[$rate][] = $wc;
					}

					$wc = $this->get_advanceWheeling($customer_id, $rea_mp_id, $month, $year, $rate_wc_sfe);
					if($rate_wc_sfe){
						
						$rate = explode('=>',$rate_wc_sfe);
						$rate = str_replace('([', '', $rate[1]);
						$rate = str_replace('])', '', $rate);
						$customer_payables[$rate][] = $wc;
					}

				}

				// echo '<pre>';
				// print_r($customer_payables);
				// echo '</pre>';

				$co++;
				$tot1=$tot2=$tot3=$tot4=$tot5=0;
				foreach($customer_payables as $cus=>$pay){
					if($co%2 != 0){
						$df = "background-color: #e6e6e6;";
					}else{
						$df = "background-color:#fff;";
					}

					echo '<tr>';
					echo '<td style="border-left:2px solid #000;'.$df.'">'.($co++).'.</td>';
					echo '<td style="border-left:2px solid #000;'.$df.'">'.$cus.'</td>';
					$tAR1 = $tAR2 = $tAR3 = $tATOTAL = $tAMR1 = $tAMR2 = $tAMR3 = $tALL = 0;
					foreach($pay as $x){
						extract($x);
						$tAR1 += $AR1;
						$tAR2 += $AR2;
						$tAR3 += $AR3;
						$tATOTAL += $ATOTAL;
						$tAMR1 += $AMR1;
						$tAMR2 += $AMR2;
						$tAMR3 += $AMR3;
						$tALL += $AMR1+$AMR2+$AMR3;
					}

					$tot1 += $tAR1;
					$tot2 += $tAR2;
					$tot3 += $tAR3;
					$tot4 += $tATOTAL;
					$tot5 += $tALL;

					echo '<td style="text-align: right; border-left:2px solid #000;'.$df.'">'.number_format($tAR1).'</td>';
					echo '<td style="text-align: right;'.$df.'">'.number_format($tAR2).'</td>';
					echo '<td style="text-align: right;'.$df.'">'.number_format($tAR3).'</td>';
					echo '<td style="text-align: right;'.$df.'">'.number_format($tATOTAL).'</td>';
					echo '<td style="text-align:right;border-right:2px solid #000;'.$df.'">'.number_format($tALL).'</td>';
					echo '</tr>';
				}

				if($co%2 != 0){
					$df = "background-color: #e6e6e6;";
				}else{
					$df = "background-color:#fff;";
				}
				//$co++;

				echo '<tr>';
				echo '<td style="'.$df.' border-bottom: 2px solid #000; border-left: 2px solid #000; border-top:2px solid #000; "></td>';
				echo '<td style="'.$df.' border-bottom: 2px solid #000; border-left: 2px solid #000; border-top:2px solid #000; text-align: right; font-weight: bold; ">Total</td>';
				echo '<td align="right" style="'.$df.' border-bottom: 2px solid #000; border-top:2px solid #000;  border-left: 2px solid #000; font-weight: bold;">'.number_format($tot1).'</td>';
				echo '<td align="right" style="'.$df.' border-bottom: 2px solid #000; border-top:2px solid #000; font-weight: bold; ">'.number_format($tot2).'</td>';
				echo '<td align="right" style="'.$df.' border-bottom: 2px solid #000; border-top:2px solid #000; font-weight: bold; ">'.number_format($tot3).'</td>';
				echo '<td align="right" style="'.$df.' border-bottom: 2px solid #000;  border-top:2px solid #000; font-weight: bold;">'.number_format($tot4).'</td>';				
				echo '<td align="right" style="'.$df.' border-bottom: 2px solid #000; border-right:2px solid #000; font-weight: bold; border-top:2px solid #000; ">'.number_format($tot5).'</td>';
				
				echo '<td style="border:none;"></td>';
				echo '<td style="border:none;"></td>';
				echo '<td style="border:none;"></td>';
				echo '<td style="border:none;"></td>';
				echo '</tr>';

				echo '<tr>';
				
				echo '<tr>';
				echo '<td style="border:none; border-bottom:2px solid #000;"></td>';
				echo '<td colspan="6" style="border:none; border-bottom:2px solid #000;">&nbsp;<br/><b>WHEELING GHARGE COLLECTABLE FROM</b></td>';
				
				echo '<td colspan="5" style="border:none;"></td>';
				echo '</tr>';

				echo '<tr style="border:2px solid #000">';
				echo '<td style="border-right:2px solid #000; text-align:center; font-weight:bold;">No.</td>';
				echo '<td style="border-right:2px solid #000; text-align:center; font-weight:bold;">Customer</td>';
				echo '<td style="text-align:center; font-weight:bold;">Shoulder</td>';
				echo '<td style="text-align:center; font-weight:bold;">Peak</td>';
				echo '<td style="text-align:center; font-weight:bold;">Off Peak</td>';
				echo '<td style="text-align:center; font-weight:bold;">Advance</td>';
				echo '<td style="text-align:center; font-weight:bold;">Amount</td>';
				echo '</tr>';


				$customer_payables = array();

				$time_readings = strtotime(date($year.'-'.$month.'-15'));
				$db = new Db();
				$sql = "SELECT rea_cus_id, rea_mp_id, rate_wc_sfi,rate_wc_sfe FROM r_rate,r_reading WHERE rea_id = rate_reading_id AND rea_date = '$time_readings' AND (rate_payable_imports = '$customer_id' OR rate_payable_exports = '$customer_id') AND (rate_wc_sfi IS NOT NULL OR rate_wc_sfe IS NOT NULL) AND rate_lfi != '([DONOT_DISPLAY])'";
				$customer_payables = array();
				$select = $db->select($sql);
				foreach($select[0] as $row){
					extract($row);					
					$wc = $this->get_advanceWheeling($rea_cus_id, $rea_mp_id, $month, $year, $rate_wc_sfi,$customer_id);
					if($rate_wc_sfi){
						$rate = explode('=>',$rate_wc_sfi);
						$rate = str_replace('([', '', $rate[1]);
						$rate = str_replace('])', '', $rate);
						$customer_payables[$this->rgf("customer",$rea_cus_id,"customer_id", "customer_short_name")][] = $wc;
					}

					$wc = $this->get_advanceWheeling($rea_cus_id, $rea_mp_id, $month, $year, $rate_wc_sfe,$customer_id);
					if($rate_wc_sfe){
						$rate = explode('=>',$rate_wc_sfe);
						$rate = str_replace('([', '', $rate[1]);
						$rate = str_replace('])', '', $rate);
						$customer_payables[$this->rgf("customer",$rea_cus_id,"customer_id", "customer_short_name")][] = $wc;
					}

				}

				// echo '<pre>';
				// print_r($customer_payables);
				// echo '</pre>';

				//$co++;
				$tot1=$tot2=$tot3=$tot4=$tot5=0;
				foreach($customer_payables as $cus=>$pay){
					if($co%2 != 0){
						$df = "background-color: #e6e6e6;";
					}else{
						$df = "background-color:#fff;";
					}

					echo '<tr>';
					echo '<td style="border-left:2px solid #000;'.$df.'">'.($co++).'.</td>';
					echo '<td style="border-left:2px solid #000;'.$df.'">'.$cus.'</td>';
					$tAR1 = $tAR2 = $tAR3 = $tATOTAL = $tAMR1 = $tAMR2 = $tAMR3 = $tALL = 0;
					foreach($pay as $x){
						extract($x);
						$tAR1 += $AR1;
						$tAR2 += $AR2;
						$tAR3 += $AR3;
						$tATOTAL += $ATOTAL;
						$tAMR1 += $AMR1;
						$tAMR2 += $AMR2;
						$tAMR3 += $AMR3;
						$tALL += $AMR1+$AMR2+$AMR3;
					}

					$tot1 += $tAR1;
					$tot2 += $tAR2;
					$tot3 += $tAR3;
					$tot4 += $tATOTAL;
					$tot5 += $tALL;

					echo '<td style="text-align: right; border-left:2px solid #000;'.$df.'">'.number_format($tAR1).'</td>';
					echo '<td style="text-align: right;'.$df.'">'.number_format($tAR2).'</td>';
					echo '<td style="text-align: right;'.$df.'">'.number_format($tAR3).'</td>';
					echo '<td style="text-align: right;'.$df.'">'.number_format($tATOTAL).'</td>';
					echo '<td style="text-align:right;border-right:2px solid #000;'.$df.'">'.number_format($tALL).'</td>';
					echo '</tr>';
				}

				if($co%2 != 0){
					$df = "background-color: #e6e6e6;";
				}else{
					$df = "background-color:#fff;";
				}
				//$co++;


				echo '<tr>';
				echo '<td style="'.$df.' border-bottom: 2px solid #000; border-left: 2px solid #000; border-top:2px solid #000; "></td>';
				echo '<td style="'.$df.' border-bottom: 2px solid #000; border-left: 2px solid #000; border-top:2px solid #000; text-align: right; font-weight: bold; ">Total</td>';
				echo '<td align="right" style="'.$df.' border-bottom: 2px solid #000; border-top:2px solid #000;  border-left: 2px solid #000; font-weight: bold;">'.number_format($tot1).'</td>';
				echo '<td align="right" style="'.$df.' border-bottom: 2px solid #000; border-top:2px solid #000; font-weight: bold; ">'.number_format($tot2).'</td>';
				echo '<td align="right" style="'.$df.' border-bottom: 2px solid #000; border-top:2px solid #000; font-weight: bold; ">'.number_format($tot3).'</td>';
				echo '<td align="right" style="'.$df.' border-bottom: 2px solid #000;  border-top:2px solid #000; font-weight: bold;">'.number_format($tot4).'</td>';				
				echo '<td align="right" style="'.$df.' border-bottom: 2px solid #000; border-right:2px solid #000; font-weight: bold; border-top:2px solid #000; ">'.number_format($tot5).'</td>';
				
				echo '<td style="border:none;"></td>';
				echo '<td style="border:none;"></td>';
				echo '<td style="border:none;"></td>';
				echo '<td style="border:none;"></td>';
				echo '</tr>';

			}
			

				?>
			</tbody></table>
		<?php } ?>
			<?php 
			$this->showReadingComments();
			?>
		</div>
			<?php	
			if($customer_id == 2030){
				echo '<div style="margin:auto;">';
			}
			$this->addReadingComment();

			$t = new TableCreatorScheduleWheeling();
			$heading = "$g";
			$t->open($this->full_name(user_id()), $heading);
			//$t->title($customer_short_name.' '.$year.' '.$month);
			$t->thd($db_values);
			$t->close();
			$t->results();

			$e = new Exporter();
			echo $e->getDisplay($heading, $t->results());

			if($customer_id == 2030){
				echo '</div>';
			}

			?>		
	
	</div>
	<?php
	}

	public function addReadingComment(){
		$p1 = portion(1);
		$p2 = portion(2);
		$p3 = portion(3);
		$p4 = portion(4);
		$p5 = portion(5);
		$p6 = portion(6);

		if($p6 == "credit-note"){
			$type = $p2.' '.$p6;
		}else{
			$type = $p2;
		}

		$customer_id = $p3;
		$year = $p4;
		$month = $p5;

		if(isset($_POST['sendC'])){
			$lastComment = $_POST['lastComment'];

			$db = new Db();
			$insert = $db->insert("readings_comment", ["rc_comment"=>$lastComment, "rc_date_added"=>time(), "rc_added_by"=>user_id(), "rc_customer_id"=>$customer_id, "rc_year"=>$year, "rc_month"=>$month, "rc_type"=>$type, "rc_last"=>1]);

			if(empty($db->error())){
				Feedback::success();

				if($p6 == "credit-note")
					Feedback::redirect(return_url().$p1."/".$p2."/".$p3."/".$p4."/".$p5."/credit-note#lastPart");
				else
					Feedback::redirect(return_url().$p1."/".$p2."/".$p3."/".$p4."/".$p5."#lastPart");
			}
		}

		if(!$this->isLocked($customer_id, $year, $month)){

			echo '<form action="" method="post">';
			echo '<div style="width:50%;padding:10px; margin:10px 0; border:1px solid #F00;border-radius:10px; background-color: #ffe0e0;">';
			echo '<div style="">';
			echo '<b>Add Notes:</b><Br/>';
			echo '<textarea style="width: 100%; margin:10px 0;padding:10px;" name="lastComment"></textarea>';
			echo '<button name="sendC" type="submit" class="btn btn-danger btn-block"><i class="fa fa-fx fa-save"></i> Save </button>';
			echo '</div>';
			echo '</div>';
			echo '</form>';
		}
	}

	public function showReadingComments(){
		$p1 = portion(1);
		$p2 = portion(2);
		$p3 = portion(3);
		$p4 = portion(4);
		$p5 = portion(5);
		$p6 = portion(6);

		if($p6 == "credit-note"){
			$type = $p2.' '.$p6;
		}else{
			$type = $p2;
		}

		$customer_id = $p3;
		$year = $p4;
		$month = $p5;
		
		//echo return_url();
		$db = new Db();
		$select = $db->select("SELECT * FROM readings_comment WHERE rc_customer_id = '$customer_id' AND rc_last = 1 AND rc_year = '$year' AND rc_month = '$month' AND rc_type = '$type'");
		if($db->num_rows()){
			echo '<b style="display:block;margin-top:15px;">Notes:</b>';
		}else{
			if($p2 == "view-invoice"){
				echo '<div style="margin:2pt;">&nbsp;</div>';
			}
		}
		echo '<style>.added{display:none;} .readings-comment:hover .delete .added{display:inline-block;}</style>';
		echo '<ol style="margin-top:0; padding-top:0; font-size:12px; font-family: arial;">';
		foreach($select[0] as $row){
			extract($row);
			if($rc_comment){
				echo '<li>';
				echo '<span class="readings-comment">';
				echo nl2br($rc_comment);
				echo '<span class="delete">';
				echo '<a href="'.return_url().$p1."/delete-reading-comment/".$p3."/".$p4.'/'.$p5.'/'.$p2.'/'.$rc_id.'/'.$p6.'" class="">';
				echo '<i class="fa fa-fw fa-times"></i>';
				echo '</a>';
				echo '<span class="added"> Added by:'.$this->full_name($rc_added_by).'</span>';
				echo '</span>';
				echo '</li>';
			}
		}
		echo '</ol>';
		echo '<div id="lastPart"></div>';
	}

	public function deleteReadingComment(){
		$id = portion(7);
		$p8 = portion(8);

		if($p8 == "credit-note"){
			$type = portion(6).' '.$p8;
		}else{
			$type = portion(6);
		}

		$db = new Db();
		$delete = $db->query("DELETE FROM readings_comment WHERE rc_id = '$id' AND rc_type = '$type' ");
		echo $db->error();
		Feedback::success("Successfully Deleted");

		if($p8 == "credit-note")
			Feedback::redirect(return_url().portion(1).'/'.portion(6)."/".portion(3)."/".portion(4)."/".portion(5)."/credit-note#lastPart");
		else
			Feedback::redirect(return_url().portion(1).'/'.portion(6)."/".portion(3)."/".portion(4)."/".portion(5)."#lastPart");
	}

	
	public function advanceScheduleCreditNote($id, $year, $month){

		$db = new Db();
		$select = $db->select("SELECT * FROM customer WHERE customer_id = '$id'");
		extract($select[0][0]);

		// echo '<pre>';
		// print_r($this->totalBackflow($id, $year, $month));
		// echo '</pre>';
	?>
	<div id="side">
		<?php 
		//echo strtotime('2022-09-01');
		$units = $this->customerAndSchedule($id, $year, $month);
		$this->energyLinks($id, $year, $month);

		?>
		<br/>
		<style type="text/css">.schedule{font-size: 12px !important;}</style>
		<div id="printMe">
						
				<?php 
				echo '<title>'.strtoupper($customer_short_name.' Advance Schedule ').' - '.($month).' '.$year.'</title>';


				$reading_time = strtotime("$year-$month-15");

				$count = 1;
				$db = new Db();
				$select = $db->select("SELECT * FROM metering_point, r_reading_cn, r_rate_cn WHERE mp_customer_id = '$customer_id' AND n_rea_cus_id = '$customer_id' AND mp_id = n_rea_mp_id AND n_rea_id = n_rate_reading_id AND n_rea_date = '$reading_time' ORDER BY mp_location ASC");

				$total_mp = 0;
				foreach($select[0] as $row){
					extract($row);
					if(date('Y', $n_rea_date)==$year && date('m', $n_rea_date)==$month){
						$total_mp++;
					}
				}
				$units = 0.001;

				$tou = $this->tou($customer_id, $month, $year, 1);
		
				$peak = $tou[0];
				$shoulder = $tou[1];
				$off_peak = $tou[2];

				?>
				<style>table tr td, table tr th{ height: 10px !important;} table tr th{text-align: center;} table trd:hover tdd{ background-color: #f9ff95; !important;} @media print{ #hint,.add-comment, .add-hint,.net-advance-form{display: none; }}</style>
			<center><h3 class="hide">UGANDA ELECTRICITY TRANSMISSION COMPANY LTD</h3></center>

			<?php 
			if($customer_id == 2030){//KPLC
				$g = "<center><h4>";
				$g .= $customer_short_name;
				$g .= ' BILL FOR THE MONTH OF ';
				$g .= month_name($month);
				$g .= ' ';
				$g .= $year;

				$g .= "</h4></center>";
				echo strtoupper($g);

				$a = $this->KPLC($year, $month);

				echo '<style>table tr td{ border: 2px solid #000; }</style>';

				echo '<table border="1" style="margin:auto; font-size:20px; font-weight:bold;" cellpadding="2" cellspacing="0">';

				echo '<tr valign="bottom">';
				echo '<td style="border:2px solid #000;" rowspan="2" align="center" style="min-width:200px">Time Band</td>';
				echo '<td style="border:2px solid #000;" colspan="3"><center>Advance (KWh)</center></td>';
				echo '</tr>';

				echo '<tr>';
				echo '<td style="border:2px solid #000;min-width:150px;" align="center">Main Meter</td>';
				echo '<td style="border:2px solid #000;min-width:150px;" align="center">Check Meter</td>';
				echo '<td style="border:2px solid #000;min-width:150px;" align="center">Average</td>';
				echo '</tr>';

				echo '<tr style="height:100px;">';
				echo '<td style="border-bottom:1px solid #ccc;">EXPORT<BR/>Peak (18:00 -  23:00 hrs)</td>';
				echo '<td style="border-bottom:1px solid #ccc;" align="right">'.number_format($a['main'][1]/1000).'</td>';
				echo '<td style="border-bottom:1px solid #ccc;" align="right">'.number_format($a['check'][1]/1000).'</td>';
				echo '<td style="border-bottom:1px solid #ccc;" align="right">'.number_format($a['average'][1]/1000).'</td>';
				echo '</tr>';

				echo '<tr style="height:100px;">';
				echo '<td style="border-top:1px solid #ccc; border-bottom:1px solid #ccc;">Day (05:00 -  18:00 hrs)</td>';
				echo '<td style="border-top:1px solid #ccc; border-bottom:1px solid #ccc;" align="right">'.number_format($a['main'][0]/1000).'</td>';
				echo '<td style="border-top:1px solid #ccc; border-bottom:1px solid #ccc;" align="right">'.number_format($a['check'][0]/1000).'</td>';
				echo '<td style="border-top:1px solid #ccc; border-bottom:1px solid #ccc;" align="right">'.number_format($a['average'][0]/1000).'</td>';
				echo '</tr>';

				echo '<tr style="height:100px;">';
				echo '<td style="border-top:1px solid #ccc;">Night (23:00 -  05:00 hrs)</td>';
				echo '<td style="border-top:1px solid #ccc;" align="right">'.number_format($a['main'][2]/1000).'</td>';
				echo '<td style="border-top:1px solid #ccc;" align="right">'.number_format($a['check'][2]/1000).'</td>';
				echo '<td style="border-top:1px solid #ccc;" align="right">'.number_format($a['average'][2]/1000).'</td>';
				echo '</tr>';

				$tm = 0;
				for($i=0; $i<3; $i++)
					$tm += $a['main'][$i];

				$ta = 0;
				for($i=0; $i<3; $i++)
					$tc += $a['check'][$i];

				$ta = ($tm + $tc)/(2);

				echo '<tr style="height:100px;">';
				echo '<td>Total</td>';
				echo '<td align="right">'.number_format($tm/1000).'</td>';
				echo '<td align="right">'.number_format($tc/1000).'</td>';
				echo '<td align="right">'.number_format($ta/1000).'</td>';
				echo '</tr>';

				echo '</table>';
			}else{
			?>
			<table id="sortSpan" border="1" cellspacing="0" cellpadding="2" class="table schedule" style="font-size: 10px;border:none;font-family: arial; background-color:#fff;">
				<thead>
				<tr valign="bottom">
					<th colspan="7" style="border:2px solid black;">
					<?php
					$g = "";
					$g .= $customer_full_name;
					$g .= ' Advance Schedule ';
					$g .= $year;
					$g .= ' - ';
					$g .= month_name($month);
					echo $g;
					?>
					</th>
					<th rowspan="5" style="width:20px;border:none;"></th>	
					<th rowspan="2" style="border:none;"></th>	
					<th rowspan="2" style="border:none;"></th>	
					<th rowspan="2" style="border:none;"></th>				
				</tr>
				<tr valign="bottom">
					<th rowspan="4" style="border-left:2px solid black;border-bottom:2px solid black;">No.</th>	
					<th rowspan="4" style="border-left:2px solid black;border-bottom:2px solid black;">Metering Points</th>			
					<th style="border-left:2px solid black; border-right:2px solid black;" colspan="5">Advance</th>				
				</tr>
				<tr>
					<th style="border-left:2px solid black; border-right:2px solid black;" colspan="5">Imports</th>					
					<th colspan="2" style="border-top:2px solid black; border-right:2px solid black;border-left:2px solid black;">Wheeling Charge</th>	
					<th colspan="2" style="border-top:2px solid black; border-right:2px solid black;border-left:2px solid black;">Wheeling Charge</th>			
				</tr>
				<tr valign="bottom">
					<?php
						
						 
						if(1){
							if($peak != (int)$peak){
								$peak1 = number_format($peak, 5);
							}else{
								$peak1 = $peak;
							}
							if($shoulder != (int)$shoulder){
								$shoulder1 = number_format($shoulder, 5);
							}else{
								$shoulder1 = $shoulder;
							}
							if($off_peak != (int)$off_peak){
								$off_peak1 = number_format($off_peak, 5);
							}else{
								$off_peak1 = $off_peak;
							}
						}else{
							if($peak != (int)$peak){
								$peak1 = number_format($peak, 5);
							}else{
								$peak1 = $peak;
							}
							if($shoulder != (int)$shoulder){
								$shoulder1 = number_format($shoulder, 5);
							}else{
								$shoulder1 = $shoulder;
							}
							if($off_peak != (int)$off_peak){
								$off_peak1 = number_format($off_peak, 5);
							}else{
								$off_peak1 = $off_peak;
							}
						}
					?>
					<?php $symbol = $this->rgf("currency",$customer_currency, "currency_id", "currency_symbol"); ?>
					<th style="border-left:2px solid black;border-bottom:2px solid black;">Shoulder<br/><span style="color:red;">@ <?php echo (float)$shoulder1.'<br/>';?></span><?php echo $symbol;?>/kWh</th>
					<th style="border-bottom:2px solid black;">Peak<br/><span style="color:red;">@ <?php echo (float)$peak1.'<br/>';?></span><?php echo $symbol;?>/kWh</th>
					<th style="border-bottom:2px solid black;">Off Peak<br/><span style="color:red;">@ <?php echo (float)$off_peak1.'<br/>';?></span><?php echo $symbol;?>/kWh</th>
					<th style="border-bottom:2px solid black;">Advance<br/>(kWh)</th>
					<th style="border-right:2px solid black;border-bottom:2px solid black;">Amount<br/>(<?php echo $symbol;?>)</th>

					<th style="border-left:2px solid black;border-bottom:2px solid black;">Imports<br/>(Payable)<br/><b>(<?php echo $symbol;?>)</b></th>
					<th style="border-right:2px solid black;border-bottom:2px solid black;">Payable<br/>Customer</th>
					<th style="border-bottom:2px solid black;">Exports<br/>(Payable)<br/><b>(<?php echo $symbol;?>)</b></th>
					<th style="border-right:2px solid black;border-bottom:2px solid black;">Payable<br/>Customer</th>
				</tr>
				</thead>
				<tbody>
				<?php

				$db_values = array();
				$db_values[] = array(
					"No.", 
					"Metering Points", 
					"Shoulder<br/><span style='color:yellow;'>@ ".$shoulder1."<br/></span>".$symbol."/kWh",
					"Peak<br/><span style='color:yellow;'>@ ".$peak1."<br/></span>".$symbol."/kWh",
					"Off Peak<br/><span style='color:yellow;'>@ ".$off_peak1."<br/></span>".$symbol."/kWh",
					"Advance<br/>(kWh)",
					"Amount<br/>".$symbol,
					"EMP",
					"Percentage",
					"Imports<br/>(Payable)<br/><b>".$symbol."</b>",
					"Exports<br/>(Collectable)<br/><b>".$symbol."</b>"
				);

				$tc1 = $tc2 = $tc3 = $tc4= $tc5= $tc6= $tc7= $tc8 = 0;

				$total_mp;
				$db = new Db();

				$co = 0;
				$t = new Db();
				$sel = $t->select("SELECT DISTINCT mp_region_id FROM metering_point, r_reading_cn, meter_region, r_rate_cn WHERE mp_customer_id = '$customer_id' AND n_rea_cus_id = '$customer_id' AND n_rate_reading_id = n_rea_id AND mp_id = n_rea_mp_id AND mp_region_id IS NOT NULL AND mp_region_id = region_id ORDER BY region_order ASC");
				
				
				$sel = $t->select("SELECT * FROM meter_region ORDER BY region_order ASC");
	
				foreach($sel[0] as $r){
				extract($r);
			
				if((!$this->isThere("metering_point", ["mp_region_id"=>$region_id]))){
					continue;
				}else{
					$mp_region_id = $region_id;
				}

				if($customer_id == 2019|| $customer_id == 2036 || $customer_id == 2040 || $customer_id == 2041){
					$select = $db->select("SELECT * FROM metering_point, r_reading_cn, r_rate_cn WHERE mp_customer_id = '$customer_id' AND n_rea_cus_id = '$customer_id' AND mp_id = n_rea_mp_id AND mp_region_id = '$mp_region_id' AND n_rate_reading_id = n_rea_id AND n_rea_date = '$reading_time' ORDER BY mp_location ASC");

				echo '<tr><td style="border-left:none; border-right:none; border-bottom: 2px solid #000;"></td>
					<td colspan="6" style="border-left:none; border-right:none; border-bottom: 2px solid #000;color:red;"><b>'.strtoupper($this->rgf("meter_region", $mp_region_id, "region_id", "region_name")).'</b></td>
					<td style="border:none;"></td>
					<td colspan="3" style="border-left:none; border-right:none; border-bottom: 2px solid #000;"></td></tr>';

				$db_values[] = array("", '&nbsp;<Br/><span style="color:red;font-weight: bold">'.strtoupper($this->rgf("meter_region", $mp_region_id, "region_id", "region_name")).'</span>',
				'','','','','','',
				//'&nbsp;<Br/><span style="color:red;font-weight: bold">'.strtoupper($this->rgf("meter_region", $mp_region_id, "region_id", "region_name")).'</span>',
				);
				}else{
					$select = $db->select("SELECT * FROM metering_point, r_reading_cn, r_rate_cn WHERE mp_customer_id = '$customer_id' AND n_rea_cus_id = '$customer_id' AND mp_id = n_rea_mp_id AND mp_region_id = '$mp_region_id' AND n_rea_id = n_rate_reading_id AND n_rea_date = '$reading_time' AND (n_rate_import_wh_1 > 0 OR n_rate_import_wh_2 > 0 OR n_rate_import_wh_3 > 0 OR n_rate_export_wh_4 > 0 OR n_rate_export_wh_5 > 0 OR n_rate_export_wh_6 > 0) ORDER BY mp_location ASC");
				}

				$tw = 0;
				

				$total_amount = $total_amount2 = $total_wheeling = $total_wheeling2 = $wheeling2 = 0;
				$tc1 = $tc2 = $tc3 = $tc4= $tc5= $tc6= $tc7= $tc8 = 0;
				foreach($select[0] as $row){
					extract($row);

					//if(date('Y', $rea_date)==$year && date('m', $rea_date)==$month)
					{
						//selecting rates

						if($co%2 != 0){
							$df = "background-color: #e6e6e6;";
						}else{
							$df = "background-color:#fff;";
						}
						
						$db = new Db();
						$sql = "SELECT * FROM r_rate_cn WHERE n_rate_reading_id = '$n_rea_id'";
						$sel = $db->select($sql);
						@extract($sel[0][0]);
						
						//calling the function
						$cal = rate_calculate_cn($customer_id, ($n_rea_date), $mp_id);
											
						
						// echo '<pre>';
						// print_r($cal); echo '</pre>';
						//if($count != $total_mp){
						if(1){		
							$less = ($region_id == 10 || $region_id == 8)?"(Less) ":"";

							if($cal[21]){ //new label name
								$mp_location2 = $less.'<span title="'.$mp_location.'">'.str_replace(' ', '&nbsp;', strtoupper($cal[21])).'*</span>';
							}else{
								$mp_location2 = $less.''.str_replace(' ', '&nbsp;', $mp_location);
							}

							$v = $this->get_advance($customer_id, $mp_id, $month, $year, $cal[23], 1);//CREDIT_NOTE

							$g0 = ($v['AR1']);
							$g1 = ($v['AR2']);
							$g2 = ($v['AR3']);

							$g3 = $g1 + $g2 + $g0;
							
							if($cal[23] == "([DONOT_DISPLAY])" || $g3 == 0){
								echo '<tr id="'.($n_rea_id+5).'" style="display:none">';
							}else{
								echo '<tr id="'.($n_rea_id+5).'">';
								$co++;
							}
								
							
							echo '<td style="'.$df.' border-left:2px solid #000;">'.($co).'.</td>';
							//echo "UPDATE r_rate SET rate_sfi = '([E@$mp_id])', rate_lfe ='([Exports@$mp_location])' WHERE rate_reading_id = '".$cal[26]."'; ";
							echo '<td style="'.$df.' border-left:2px solid black;">'.$mp_location2;

							//rc_year = '$year' AND rc_month = '$month' AND 
							$d =  new Db();
							$se = $d->select("SELECT * FROM readings_comment WHERE rc_mp_id = '$mp_id' AND rc_customer_id = '$customer_id' AND rc_month = '$month' AND rc_year = '$year' AND rc_last IS NULL");
						

							if($d->num_rows()){
								echo '<form action="" method="post" style="padding:0; margin:0;">';

								if(isset($_POST['delete'.$mp_id])){

									 $t = $_POST["rc$mp_id"];
									 $customer = $_POST["customer$mp_id"];
									 $readings = $_POST["rea_id$mp_id"];


									$db = new Db();
									$sql = "DELETE FROM readings_comment WHERE rc_id = '$t'";
									$delete = $db->query($sql);

									$select = $db->update("r_rate",[
										"rate_advance_1"=>NULL,
										"rate_advance_2"=>NULL,
										"rate_advance_3"=>NULL,
										"rate_advance_4"=>NULL,
										"rate_advance_5"=>NULL,
										"rate_advance_6"=>NULL
									], ["rate_cus_id"=>$customer, "rate_reading_id"=>$readings]);

									FeedBack::redirect(return_url()."/".portion(1)."/".portion(2)."/".portion(3)."/".portion(4)."/".portion(5)."#$readings");
								}

								echo '<div id="hint" style=""><div class="message">';
								echo '<p style="font-weight:bold;">'.$mp_location.'dsfsdf</p>';
								foreach($se[0] as $r){
									extract($r);
									echo '<b>'.$this->full_name($rc_added_by).':</b><br/>';
									echo nl2br($rc_comment);
									echo '<br/>';
									echo '<span style="color:#aaa;">'.Feedback::date_fm($rc_date_added).'</span>';
								}
								echo '<hr style="margin:0; padding:0; margin-top:8px;"/>';
								$x = array('is', 'es', 'ip', 'ep', 'io', 'eo');
								$order = array(1, 4, 2, 5, 3,6);
								for($i=1; $i<=6; $i++){
									echo '<div class="col-md-6" style="color:#aaa; padding:0; margin:0;">';
									echo strtoupper($x[$i-1]).': '.number_format(${"rate_advance_".$order[$i-1]}/1000, 2)."";
									echo '</div>';
								}
								echo '<div class="clearfix"></div>';

								echo '<input type="hidden" name="rc'.$mp_id.'" value="'.$rc_id.'" />';
								echo '<input type="hidden" name="customer'.$mp_id.'" value="'.$customer_id.'" />';
								echo '<input type="hidden" name="rea_id'.$mp_id.'" value="'.$rea_id.'" />';

								if(!$this->isLocked($customer_id, $year, $month)){
									echo '<div id="add-new-hint"><button type="submit" name="delete'.$mp_id.'"><i class="fa fa-fw fa-times"></i></button></div>';
								}
								echo '</div></div>';
								echo '</form>';
							}else{ 
								if(!$this->isLocked($customer_id, $year, $month)){
								echo '<div class="add-hint" id="add-hint'.$co.'" style="color:white;"><i class="fa fa-fw fa-plus"></i></div>';
								}
							?>
								<div id="myModal<?php echo $co; ?>" class="net-advance-form modal">
								  <div class="modal-content" style="width:40%;">
								    <span id="close<?php echo $co; ?>" class="close" style="color:red;">&times;</span>
								    <p></p>
								 
							<?php
								echo '<div>'; 
								
                    $current_customer = $this->rgf('customer', $id, 'customer_id', 'customer_short_name');
                    if($current_customer){
                        $type = $this->set_method;
                        $year;
                        $month;
                        $sep = "/";
                    }

                    echo "<h2 class='text-dark text-center' style='font-size:2em;padding:0 20px; font-weight:bold;color:black;'> $current_customer $type $month$sep$year</h2>";
                
								echo '<form action=""  method="post" style="padding:0; margin:0;">';

								if(isset($_POST['addComment'.$mp_id])){
									$comment = $_POST['comment'];

									$is = $_POST['is'];
									$ip = $_POST['ip'];
									$io = $_POST['io'];
									$es = $_POST['es'];
									$ep = $_POST['ep'];
									$eo = $_POST['eo'];

									if(empty($comment)){
										FeedBack::error("Enter Comment");
									}else{
										$db = new Db();
										$show = array("schedule", "advance-schedule", "view-invoice");
										
										$showon = $_POST['showon'];
										for($i=0; $i<count($showon); $i++){
											$type = $show[$showon[$i]];
											$select = $db->insert("readings_comment", ["rc_comment"=>$comment, "rc_date_added"=>time(), "rc_added_by"=>user_id(), "rc_customer_id"=>$customer_id, "rc_year"=>$year, "rc_month"=>$month, "rc_mp_id"=>$mp_id, "rc_type"=>$type, "rc_last"=>1]);
										}

										$select = $db->insert("readings_comment", ["rc_comment"=>$comment, "rc_date_added"=>time(), "rc_added_by"=>user_id(), "rc_customer_id"=>$customer_id, "rc_year"=>$year, "rc_month"=>$month, "rc_mp_id"=>$mp_id, "rc_type"=>portion(2)]);

										$x = array('is', 'ip', 'io', 'es', 'ep', 'eo');

										for($i=0; $i<count($x); $i++){
											$y = $x[$i];
											if($$y != ""){
												$a = array();
												$a["rate_advance_".($i+1)] = str_replace(',', '', ($$y))*1000;
												$select = $db->update("r_rate", $a, ["rate_cus_id"=>$customer_id, "rate_reading_id"=>$rea_id]);
											}
										}

										FeedBack::redirect(return_url()."/".portion(1)."/".portion(2)."/".portion(3)."/".portion(4)."/".portion(5)."#$rea_id");
									}
								}
								
								echo '<div class="add-comment" style="color:black;">';
								echo '<div style="padding:10px;">';
								echo '<p style="font-weight:bold; color:blue;">'.$mp_location.'</p>';
								echo '<b>Add Comment</b><Br/> ';
								echo '<textarea required style="width:100%; padding:10px;" placeholder="comment" name="comment"></textarea>';
								echo '<b>Show Comment On:</b><br/> &nbsp; &nbsp; &nbsp; ';

								$show = array("Sch", "WC", "INV");
								for($i=0; $i<count($show); $i++){
									echo '<input class="chk-col-blue" type="checkbox" value="'.$i.'" name="showon[]" id="invoice'.$i.$mp_id.'"/><label for="invoice'.$i.$mp_id.'">'.$show[$i].' &nbsp; &nbsp; </label>';
								}

								echo '<br/><br/>Advance Imports: <b>(KWh)</b><br/>';
								echo '<input class="number" style="width:30%; min-width:100px; padding:1%; margin:1%;" placeholder="Shoulder" name="is" autocomplete="off" />';
								echo '<input class="number" style="width:30%; min-width:100px; padding:1%; margin:1%;" placeholder="Peak" name="ip" autocomplete="off" />';
								echo '<input class="number" style="width:30%; min-width:100px; padding:1%; margin:1%;" placeholder="Off Peak" name="io" autocomplete="off" />';

								echo '<br/>Advance Exports: <b>(KWh)</b><br/>';
								echo '<input class="number" style="width:30%; min-width:100px; padding:1%; margin:1%;" placeholder="Shoulder" name="es" autocomplete="off"/>';
								echo '<input class="number" style="width:30%; min-width:100px; padding:1%; margin:1%;" placeholder="Peak" name="ep" autocomplete="off" />';
								echo '<input class="number" style="width:30%; min-width:100px; padding:1%; margin:1%;" placeholder="Off Peak" name="eo" autocomplete="off" />';

								echo '<br/><br/><input class="number" type="submit" class="btn btn-danger btn-block" value="Save" name="addComment'.$mp_id.'">';
								echo '</div>';
								echo '</div>';
								echo '</div>';
								
								echo '</div>';

								echo '</form>';

								?>


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

							

							echo '</td>';


							if(1){

								$g4 = ($g0*$shoulder + $g1*$peak + $g2*$off_peak)/1000;
								$amount = ($g0*$shoulder+$g1*$peak+$g2*$off_peak)/1000; //imports
								$amount2 = ($cal[4]*$shoulder+$cal[5]*$peak+$cal[6]*$off_peak)/1000; //exports
								
								$tw += $g3*$mp_wheeling_charge/100;
																
								$total_amount += $amount;
								$total_wheeling += $amount*$mp_wheeling_charge/100;

								$wheeling2 = 0; //$amount2*$mp_wheeling_charge/100;

								$wc = $this->get_advanceWheeling($customer_id, $mp_id, $month, $year, $cal[27], 0, 1);
								
								$wheeling2 = $wc['AMOUNT'];
								$total_wheeling2 += $wheeling2;

								$wcE = $this->get_advanceWheeling($customer_id, $mp_id, $month, $year, $cal[28], 0, 1);
								
								$wheeling2E = $wcE['AMOUNT'];
								$total_wheeling2 += $wheeling2+$wheeling2E;

								echo '<td style="'.$df.'border-left:2px solid black; text-align:right;">'.number_format($g0*$units);
								//if(!$this->isLocked($customer_id, $year, $month))
								{
								// echo '<div class="add-hint formula-space space2" style="color:white;"><i class="fa fa-fw fa-calculator"></i>
								// 	<div class="formula-message">
								// 		<div class="area">';
								// 		echo $rate_lfi;
								// 		//$this->formulaDisplay($customer_id, $mp_id, $year, $month, 1);
								// 		echo '</div></div>
								// </div>';
								}
								echo '</td>';
								echo '<td style="'.$df.' text-align:right;">'.number_format($g1*$units).'</td>';
								echo '<td style="'.$df.' text-align:right;">'.number_format($g2*$units).'</td>';

			
								echo '<td style="'.$df.' text-align:right;">'.number_format($g3*$units).'</td>';
								echo '<td style="'.$df.'border-right:2px solid black; text-align:right;">'.number_format($g4).'</td>';
							}

							echo '<td style="border:none;"></td>';
							//@@@@@@@@@@@@@@@@@@@
							//if( $this->payable($mp_id, $customer_id,0, $year, $month))
							{

								echo '<td style="border-left:2px solid #000;'.$df.'text-align: right;" title="'.$cal[29].'">
								'.number_format($wheeling2).'</td>';

								echo '<td style="'.$df.'border-right:2px solid black;text-align: right;">'.$wc['PAYABLE'].'</td>';	
								echo '<td style="border-left:2px solid #000;'.$df.'text-align: right;" title="'.$cal[30].'">
								'.number_format($wheeling2E).'</td>';

								echo '<td style="'.$df.'border-right:2px solid black;text-align: right;">'.$wcE['PAYABLE'].'</td>';	

							}

							echo '</tr>';

							$tc1 += $g0;
							$tc2 += $g1;
							$tc3 += $g2;
							$tc4 += $g3;

							$tc5 += $cal[4];
							$tc6 += $cal[5];
							$tc7 += $cal[6];
							$tc8 += $cal[7];
						}

						$db_values[] = array(
							$co, 
							$mp_location,
							number_format($cal[0]*$units),
							number_format($cal[1]*$units),
							number_format($cal[2]*$units),
							number_format($cal[3]*$units),
							number_format($amount),
							"",
							number_format($mp_wheeling_charge,2).'&nbsp;%',
							number_format($amount*$mp_wheeling_charge/100),
							number_format($wheeling2)
						);

					}					
				}
				
				// $l = $this->sumFormulae($customer_id, $year, $month);

				// $tc1 = $l[0];
				// $tc2 = $l[1];
				// $tc3 = $l[2];
				// $tc4 = $l[3];
				// $tc5 = $l[4];
				// $tc6 = $l[5];
				// $tc7 = $l[6];
				// $tc8 = $l[7];

				$tot = ($tc1*$shoulder+$tc2*$peak+$tc3*$off_peak)/1000;

				if($co%2 != 0){
					$df = "background-color: #e6e6e6;";
				}else{
					$df = "background-color:#fff;";
				}
				//$co++;

				if($customer_id == 2019){

				echo '<tr>';
				echo '<td style="'.$df.'border-top:2px solid #000; border-left:2px solid #000;border-bottom:2px solid black;border-top:2px solid #000;"></td>';
				echo '<td style="'.$df.'border-top:2px solid #000; border-left:2px solid black; border-bottom:2px solid black; text-align:right;font-weight: bold; ">Total</td>';
				
				echo '<td style="'.$df.'border-top:2px solid #000; border-bottom:2px solid black;  border-left:2px solid black;  text-align:right;font-weight: bold;">'.number_format($tc1*$units).'</td>';
				echo '<td style="'.$df.'border-top:2px solid #000; border-bottom:2px solid black;  text-align:right;font-weight: bold;">'.number_format($tc2*$units).'</td>';
				echo '<td style="'.$df.'border-top:2px solid #000; border-bottom:2px solid black;  text-align:right;font-weight: bold;">'.number_format($tc3*$units).'</td>';
				echo '<td style="'.$df.'border-top:2px solid #000; border-bottom:2px solid black;  text-align:right;font-weight: bold;">'.number_format($tc4*$units).'</td>';
				echo '<td style="'.$df.'border-top:2px solid #000; border-right:2px solid black; border-bottom:2px solid black; text-align:right; font-weight: bold; ">'.number_format($tot).'</td>';

				echo '<td style="border:none;"></td>';
				echo '<td colspan="3" style="border:none;border-top:2px solid #000; "></td>';

				echo '</tr>';
				}

				}
				
				$l = $this->sumFormulae($customer_id, $year, $month, 1);

				$tc1 = $l[0];
			 	$tc2 = $l[1];
				$tc3 = $l[2];
				$tc4 = $l[3];
				$tc5 = $l[4];
				$tc6 = $l[5];
				$tc7 = $l[6];
				$tc8 = $l[7];

				$a1 = $a2 = $a3 = $a4 = 0;

				// if(portion(3) == 2020){ //units from KRECS exports
				// 	$reading_time = strtotime(date("$year-$month-15"));
				// 	$t = rate_calculate(2022, $reading_time, 5315);

				// 	$a1 = $t[0];
				// 	$a2 = $t[1];
				// 	$a3 = $t[2];
				// 	$a4 = $t[3];
				// }

				$tot = (($tc1+$a1)*$shoulder+($tc2+$a2)*$peak+($tc3+$a3)*$off_peak)/1000;

				if($co%2 != 0){
					$df = "background-color: #e6e6e6;";
				}else{
					$df = "background-color:#fff;";
				}
				//$co++;

				echo '<tr style="">';
				echo '<td style="'.$df.'border-top:2px solid #000; border-left:2px solid #000;border-bottom:2px solid black;border-top:2px solid #000;"></td>';
				echo '<td style="'.$df.'border-top:2px solid #000; border-left:2px solid black; border-bottom:2px solid black; text-align:right;font-weight: bold; ">Gross Units</td>';
				
				echo '<td style="'.$df.'border-top:2px solid #000; border-bottom:2px solid black;  border-left:2px solid black;  text-align:right;font-weight: bold;">'.number_format(($tc1+$a1)*$units,2).'</td>';
				echo '<td style="'.$df.'border-top:2px solid #000; border-bottom:2px solid black;  text-align:right;font-weight: bold;">'.number_format(($tc2+$a2)*$units,2).'</td>';
				echo '<td style="'.$df.'border-top:2px solid #000; border-bottom:2px solid black;  text-align:right;font-weight: bold;">'.number_format(($tc3+$a3)*$units,2).'</td>';
				echo '<td style="'.$df.'border-top:2px solid #000; border-bottom:2px solid black;  text-align:right;font-weight: bold;">'.number_format(($tc4+$a4)*$units,2).'</td>';
				echo '<td style="'.$df.'border-top:2px solid #000; border-right:2px solid black; border-bottom:2px solid black; text-align:right; font-weight: bold; ">'.number_format($tot).'</td>';

				echo '<td colspan="4" style="border-top:2px solid #000; border:none;"></td>';
				
				echo '</tr>';

				echo '<tr><td colspan="7" style="border:none;"></td></tr>';

				if($id == 2040){
					echo '<tr>';
					echo '<td style="'.$df.'border-top:2px solid #000; border-left:2px solid #000;border-bottom:2px solid black;border-top:2px solid #000;"></td>';
					echo '<td style="'.$df.'border-top:2px solid #000; border-left:2px solid black; border-bottom:2px solid black; font-weight: bold; ">asdBackflows</td>';
					
					echo '<td style="'.$df.'border-top:2px solid #000; border-bottom:2px solid black;  border-left:2px solid black;  text-align:right;font-weight: bold;">'.number_format($tc5*$units,2).'</td>';
					echo '<td style="'.$df.'border-top:2px solid #000; border-bottom:2px solid black;  text-align:right;font-weight: bold;">'.number_format($tc6*$units,2).'</td>';
					echo '<td style="'.$df.'border-top:2px solid #000; border-bottom:2px solid black;  text-align:right;font-weight: bold;">'.number_format($tc7*$units,2).'</td>';
					echo '<td style="'.$df.'border-top:2px solid #000; border-bottom:2px solid black;  text-align:right;font-weight: bold;">'.number_format($tc8*$units,2).'</td>';
					echo '<td style="'.$df.'border-top:2px solid #000; border-right:2px solid black; border-bottom:2px solid black; text-align:right; font-weight: bold; ">'.number_format(($tc5*$shoulder+$tc6*$peak+$tc7*$off_peak)*$units,2).'</td>';

					echo '<td colspan="4" style="border-top:2px solid #000; border:none;"></td>';
					
					echo '</tr>';
					echo '<tr>';
					echo '<td style="'.$df.'border-top:2px solid #000; border-left:2px solid #000;border-bottom:2px solid black;border-top:2px solid #000;"></td>';
					echo '<td style="'.$df.'border-top:2px solid #000; border-left:2px solid black; border-bottom:2px solid black; font-weight: bold; ">Gross Units - Backflows</td>';
					
					echo '<td style="'.$df.'border-top:2px solid #000; border-bottom:2px solid black;  border-left:2px solid black;  text-align:right;font-weight: bold;">'.number_format(($tc1 - $tc5)*$units,2).'</td>';
					echo '<td style="'.$df.'border-top:2px solid #000; border-bottom:2px solid black;  text-align:right;font-weight: bold;">'.number_format(($tc2 - $tc6)*$units,2).'</td>';
					echo '<td style="'.$df.'border-top:2px solid #000; border-bottom:2px solid black;  text-align:right;font-weight: bold;">'.number_format(($tc3 - $tc7)*$units,2).'</td>';
					echo '<td style="'.$df.'border-top:2px solid #000; border-bottom:2px solid black;  text-align:right;font-weight: bold;">'.number_format(($tc4 - $tc8)*$units,2).'</td>';
					echo '<td style="'.$df.'border-top:2px solid #000; border-right:2px solid black; border-bottom:2px solid black; text-align:right; font-weight: bold; ">'.number_format((($tc1 - $tc5)*$shoulder+($tc2 - $tc6)*$peak+($tc3 - $tc7)*$off_peak)*$units,2).'</td>';

					echo '<td colspan="4" style="border-top:2px solid #000; border:none;"></td>';
					
					echo '</tr>';

				}


				if($id == 2020){ //units from KRECS exports
					echo '<tr>';
					echo '<td style="'.$df.'border-top:2px solid #000; border-left:2px solid #000;border-bottom:2px solid black;border-top:2px solid #000;"></td>';
					echo '<td style="'.$df.'border-top:2px solid #000; border-left:2px solid black; border-bottom:2px solid black; font-weight: bold; ">Units from KRECS(Imports)</td>';
					
					echo '<td style="'.$df.'border-top:2px solid #000; border-bottom:2px solid black;  border-left:2px solid black;  text-align:right;font-weight: bold;">'.number_format($a1*$units,2).'</td>';
					echo '<td style="'.$df.'border-top:2px solid #000; border-bottom:2px solid black;  text-align:right;font-weight: bold;">'.number_format($a2*$units,2).'</td>';
					echo '<td style="'.$df.'border-top:2px solid #000; border-bottom:2px solid black;  text-align:right;font-weight: bold;">'.number_format($a3*$units,2).'</td>';
					echo '<td style="'.$df.'border-top:2px solid #000; border-bottom:2px solid black;  text-align:right;font-weight: bold;">'.number_format($a4*$units,2).'</td>';
					echo '<td style="'.$df.'border-top:2px solid #000; border-right:2px solid black; border-bottom:2px solid black; text-align:right; font-weight: bold; ">'.number_format(($a1*$shoulder+$a2*$peak+$a3*$off_peak)*$units,2).'</td>';

					echo '<td colspan="4" style="border-top:2px solid #000; border:none;"></td>';
					
					echo '</tr>';
					echo '<tr>';
					echo '<td style="'.$df.'border-top:2px solid #000; border-left:2px solid #000;border-bottom:2px solid black;border-top:2px solid #000;"></td>';
					echo '<td style="'.$df.'border-top:2px solid #000; border-left:2px solid black; border-bottom:2px solid black; font-weight: bold; ">Net Units</td>';

					$a1 = $a2 = $a3 = $a4 = 0;
					
					echo '<td style="'.$df.'border-top:2px solid #000; border-bottom:2px solid black;  border-left:2px solid black;  text-align:right;font-weight: bold;">'.number_format(($tc1 - $a1)*$units,2).'</td>';
					echo '<td style="'.$df.'border-top:2px solid #000; border-bottom:2px solid black;  text-align:right;font-weight: bold;">'.number_format(($tc2 - $a2)*$units,2).'</td>';
					echo '<td style="'.$df.'border-top:2px solid #000; border-bottom:2px solid black;  text-align:right;font-weight: bold;">'.number_format(($tc3 - $a3)*$units,2).'</td>';
					echo '<td style="'.$df.'border-top:2px solid #000; border-bottom:2px solid black;  text-align:right;font-weight: bold;">'.number_format(($tc4 - $a4)*$units,2).'</td>';
					echo '<td style="'.$df.'border-top:2px solid #000; border-right:2px solid black; border-bottom:2px solid black; text-align:right; font-weight: bold; ">'.number_format((($tc1 - $a1)*$shoulder+($tc2 - $a2)*$peak+($tc3 - $a3)*$off_peak)*$units).'</td>';

					echo '<td colspan="4" style="border-top:2px solid #000; border:none;"></td>';
					
					echo '</tr>';

				}



				echo '<tr>';
				echo '<td style="border:none; border-bottom:2px solid #000;"></td>';
				echo '<td colspan="6" style="border:none; border-bottom:2px solid #000;">&nbsp;<Br/><b>WHEELING CHARGE PAYABLE TO</b></td>';
			
				
				echo '<td colspan="5" style="border:none;"></td>';
				echo '</tr>';

				echo '<tr style="border:2px solid #000">';
				echo '<td style="border-right:2px solid #000; text-align:center; font-weight:bold;">No.</td>';
				echo '<td style="border-right:2px solid #000; text-align:center; font-weight:bold;">Customer</td>';
				echo '<td style="text-align:center; font-weight:bold;">Shoulder</td>';
				echo '<td style="text-align:center; font-weight:bold;">Peak</td>';
				echo '<td style="text-align:center; font-weight:bold;">Off Peak</td>';
				echo '<td style="text-align:center; font-weight:bold;">Advance</td>';
				echo '<td style="text-align:center; font-weight:bold;">Amount</td>';
				echo '</tr>';

				$customer_payables = array();

				$time_readings = strtotime(date($year.'-'.$month.'-15'));
				$db = new Db();
				
				$sql = "SELECT n_rea_mp_id, n_rate_wc_sfi,n_rate_wc_sfe FROM r_rate_cn,r_reading_cn WHERE n_rea_id = n_rate_reading_id AND n_rea_date = '$time_readings' AND n_rate_cus_id = '$customer_id' AND (n_rate_wc_sfi IS NOT NULL OR n_rate_wc_sfe IS NOT NULL) AND n_rate_lfi != '([DONOT_DISPLAY])' AND (n_rate_import_wh_1 > 0 OR n_rate_import_wh_3 > 0 OR n_rate_export_wh_4 > 0 OR n_rate_export_wh_5 > 0 OR n_rate_export_wh_6 > 0 )";

				$customer_payables = array();
				$select = $db->select($sql);
				foreach($select[0] as $row){
					extract($row);					
					$wc = $this->get_advanceWheeling($customer_id, $n_rea_mp_id, $month, $year, $n_rate_wc_sfi, 0, 1);
					if($n_rate_wc_sfi){
						$rate = explode('=>',$n_rate_wc_sfi);
						$rate = str_replace('([', '', $rate[1]);
						$rate = str_replace('])', '', $rate);
						$customer_payables[$rate][] = $wc;
					}

					$wc = $this->get_advanceWheeling($customer_id, $n_rea_mp_id, $month, $year, $rate_wc_sfe, 0, 1);
					if($n_rate_wc_sfe){
						$rate = explode('=>',$n_rate_wc_sfe);
						$rate = str_replace('([', '', $rate[1]);
						$rate = str_replace('])', '', $rate);
						$customer_payables[$rate][] = $wc;
					}

				}

				// echo '<pre>';
				// print_r($customer_payables);
				// echo '</pre>';

				$co++;
				$tot1=$tot2=$tot3=$tot4=$tot5=0;
				foreach($customer_payables as $cus=>$pay){
					if($co%2 != 0){
						$df = "background-color: #e6e6e6;";
					}else{
						$df = "background-color:#fff;";
					}

					echo '<tr>';
					echo '<td style="border-left:2px solid #000;'.$df.'">'.($co++).'.</td>';
					echo '<td style="border-left:2px solid #000;'.$df.'">'.$cus.'</td>';
					$tAR1 = $tAR2 = $tAR3 = $tATOTAL = $tAMR1 = $tAMR2 = $tAMR3 = $tALL = 0;
					foreach($pay as $x){
						extract($x);
						$tAR1 += $AR1;
						$tAR2 += $AR2;
						$tAR3 += $AR3;
						$tATOTAL += $ATOTAL;
						$tAMR1 += $AMR1;
						$tAMR2 += $AMR2;
						$tAMR3 += $AMR3;
						$tALL += $AMR1+$AMR2+$AMR3;
					}

					$tot1 += $tAR1;
					$tot2 += $tAR2;
					$tot3 += $tAR3;
					$tot4 += $tATOTAL;
					$tot5 += $tALL;

					echo '<td style="text-align: right; border-left:2px solid #000;'.$df.'">'.number_format($tAR1).'</td>';
					echo '<td style="text-align: right;'.$df.'">'.number_format($tAR2).'</td>';
					echo '<td style="text-align: right;'.$df.'">'.number_format($tAR3).'</td>';
					echo '<td style="text-align: right;'.$df.'">'.number_format($tATOTAL).'</td>';
					echo '<td style="text-align:right;border-right:2px solid #000;'.$df.'">'.number_format($tALL).'</td>';
					echo '</tr>';
				}

				if($co%2 != 0){
					$df = "background-color: #e6e6e6;";
				}else{
					$df = "background-color:#fff;";
				}
				//$co++;

				echo '<tr>';
				echo '<td style="'.$df.' border-bottom: 2px solid #000; border-left: 2px solid #000; border-top:2px solid #000; "></td>';
				echo '<td style="'.$df.' border-bottom: 2px solid #000; border-left: 2px solid #000; border-top:2px solid #000; text-align: right; font-weight: bold; ">Total</td>';
				echo '<td align="right" style="'.$df.' border-bottom: 2px solid #000; border-top:2px solid #000;  border-left: 2px solid #000; font-weight: bold;">'.number_format($tot1).'</td>';
				echo '<td align="right" style="'.$df.' border-bottom: 2px solid #000; border-top:2px solid #000; font-weight: bold; ">'.number_format($tot2).'</td>';
				echo '<td align="right" style="'.$df.' border-bottom: 2px solid #000; border-top:2px solid #000; font-weight: bold; ">'.number_format($tot3).'</td>';
				echo '<td align="right" style="'.$df.' border-bottom: 2px solid #000;  border-top:2px solid #000; font-weight: bold;">'.number_format($tot4).'</td>';				
				echo '<td align="right" style="'.$df.' border-bottom: 2px solid #000; border-right:2px solid #000; font-weight: bold; border-top:2px solid #000; ">'.number_format($tot5).'</td>';
				
				echo '<td style="border:none;"></td>';
				echo '<td style="border:none;"></td>';
				echo '<td style="border:none;"></td>';
				echo '<td style="border:none;"></td>';
				echo '</tr>';

				echo '<tr>';
				
				echo '<tr>';
				echo '<td style="border:none; border-bottom:2px solid #000;"></td>';
				echo '<td colspan="6" style="border:none; border-bottom:2px solid #000;">&nbsp;<br/><b>WHEELING GHARGE COLLECTABLE FROM</b></td>';
				
				echo '<td colspan="5" style="border:none;"></td>';
				echo '</tr>';

				echo '<tr style="border:2px solid #000">';
				echo '<td style="border-right:2px solid #000; text-align:center; font-weight:bold;">No.</td>';
				echo '<td style="border-right:2px solid #000; text-align:center; font-weight:bold;">Customer</td>';
				echo '<td style="text-align:center; font-weight:bold;">Shoulder</td>';
				echo '<td style="text-align:center; font-weight:bold;">Peak</td>';
				echo '<td style="text-align:center; font-weight:bold;">Off Peak</td>';
				echo '<td style="text-align:center; font-weight:bold;">Advance</td>';
				echo '<td style="text-align:center; font-weight:bold;">Amount</td>';
				echo '</tr>';


				$customer_payables = array();

				$time_readings = strtotime(date($year.'-'.$month.'-15'));
				$db = new Db();
				$sql = "SELECT n_rea_cus_id, n_rea_mp_id, n_rate_wc_sfi,n_rate_wc_sfe FROM r_rate_cn,r_reading_cn WHERE n_rea_id = n_rate_reading_id AND n_rea_date = '$time_readings' AND (n_rate_payable_imports = '$customer_id' OR n_rate_payable_exports = '$customer_id') AND (n_rate_wc_sfi IS NOT NULL OR n_rate_wc_sfe IS NOT NULL) AND n_rate_lfi != '([DONOT_DISPLAY])'";
				$customer_payables = array();
				$select = $db->select($sql);
				foreach($select[0] as $row){
					extract($row);					
					$wc = $this->get_advanceWheeling($n_rea_cus_id, $n_rea_mp_id, $month, $year, $n_rate_wc_sfi,$customer_id,1);
					if($n_rate_wc_sfi){
						$rate = explode('=>',$n_rate_wc_sfi);
						$rate = str_replace('([', '', $rate[1]);
						$rate = str_replace('])', '', $rate);
						$customer_payables[$this->rgf("customer",$n_rea_cus_id,"customer_id", "customer_short_name")][] = $wc;
					}

					$wc = $this->get_advanceWheeling($n_rea_cus_id, $n_rea_mp_id, $month, $year, $n_rate_wc_sfe,$customer_id,1);
					if($n_rate_wc_sfe){
						$rate = explode('=>',$n_rate_wc_sfe);
						$rate = str_replace('([', '', $rate[1]);
						$rate = str_replace('])', '', $rate);
						$customer_payables[$this->rgf("customer",$n_rea_cus_id,"customer_id", "customer_short_name")][] = $wc;
					}

				}

				// echo '<pre>';
				// print_r($customer_payables);
				// echo '</pre>';

				//$co++;
				$tot1=$tot2=$tot3=$tot4=$tot5=0;
				foreach($customer_payables as $cus=>$pay){
					if($co%2 != 0){
						$df = "background-color: #e6e6e6;";
					}else{
						$df = "background-color:#fff;";
					}

					echo '<tr>';
					echo '<td style="border-left:2px solid #000;'.$df.'">'.($co++).'.</td>';
					echo '<td style="border-left:2px solid #000;'.$df.'">'.$cus.'</td>';
					$tAR1 = $tAR2 = $tAR3 = $tATOTAL = $tAMR1 = $tAMR2 = $tAMR3 = $tALL = 0;
					foreach($pay as $x){
						extract($x);
						$tAR1 += $AR1;
						$tAR2 += $AR2;
						$tAR3 += $AR3;
						$tATOTAL += $ATOTAL;
						$tAMR1 += $AMR1;
						$tAMR2 += $AMR2;
						$tAMR3 += $AMR3;
						$tALL += $AMR1+$AMR2+$AMR3;
					}

					$tot1 += $tAR1;
					$tot2 += $tAR2;
					$tot3 += $tAR3;
					$tot4 += $tATOTAL;
					$tot5 += $tALL;

					echo '<td style="text-align: right; border-left:2px solid #000;'.$df.'">'.number_format($tAR1).'</td>';
					echo '<td style="text-align: right;'.$df.'">'.number_format($tAR2).'</td>';
					echo '<td style="text-align: right;'.$df.'">'.number_format($tAR3).'</td>';
					echo '<td style="text-align: right;'.$df.'">'.number_format($tATOTAL).'</td>';
					echo '<td style="text-align:right;border-right:2px solid #000;'.$df.'">'.number_format($tALL).'</td>';
					echo '</tr>';
				}

				if($co%2 != 0){
					$df = "background-color: #e6e6e6;";
				}else{
					$df = "background-color:#fff;";
				}
				//$co++;

				echo '<tr>';
				echo '<td style="'.$df.' border-bottom: 2px solid #000; border-left: 2px solid #000; border-top:2px solid #000; "></td>';
				echo '<td style="'.$df.' border-bottom: 2px solid #000; border-left: 2px solid #000; border-top:2px solid #000; text-align: right; font-weight: bold; ">Total</td>';
				echo '<td align="right" style="'.$df.' border-bottom: 2px solid #000; border-top:2px solid #000;  border-left: 2px solid #000; font-weight: bold;">'.number_format($tot1).'</td>';
				echo '<td align="right" style="'.$df.' border-bottom: 2px solid #000; border-top:2px solid #000; font-weight: bold; ">'.number_format($tot2).'</td>';
				echo '<td align="right" style="'.$df.' border-bottom: 2px solid #000; border-top:2px solid #000; font-weight: bold; ">'.number_format($tot3).'</td>';
				echo '<td align="right" style="'.$df.' border-bottom: 2px solid #000;  border-top:2px solid #000; font-weight: bold;">'.number_format($tot4).'</td>';				
				echo '<td align="right" style="'.$df.' border-bottom: 2px solid #000; border-right:2px solid #000; font-weight: bold; border-top:2px solid #000; ">'.number_format($tot5).'</td>';
				
				echo '<td style="border:none;"></td>';
				echo '<td style="border:none;"></td>';
				echo '<td style="border:none;"></td>';
				echo '<td style="border:none;"></td>';
				echo '</tr>';
			

				?>
			</tbody></table>
		<?php } ?>
			<?php 
			$this->showReadingComments();
			?>
		</div>
			<?php	
			if($customer_id == 2030){
				echo '<div style="margin:auto;">';
			}
			$this->addReadingComment();

			$t = new TableCreatorScheduleWheeling();
			$heading = "$g";
			$t->open($this->full_name(user_id()), $heading);
			//$t->title($customer_short_name.' '.$year.' '.$month);
			$t->thd($db_values);
			$t->close();
			$t->results();

			$e = new Exporter();
			echo $e->getDisplay($heading, $t->results());

			if($customer_id == 2030){
				echo '</div>';
			}

			?>		
	
	</div>
	<?php
	}


	public function backflowScheduleAction(){

		$class = ($this->set_class)?$this->set_class:portion(1);
		$method = ($this->set_method)?$this->set_method:portion(2);
		$id = ($this->set_customer)?$this->set_customer:portion(3);
		$year = ($this->set_year)?$this->set_year: portion(4);
		$month = ($this->set_month)?$this->set_month: portion(5);


		$db = new Db();
		$select = $db->select("SELECT * FROM customer WHERE customer_id = '$id'");
		extract($select[0][0]);
	
	?>
	<div id="side">
		<?php 
		$units = $this->customerAndSchedule($id, $year, $month);
		$this->energyLinks($id, $year, $month);

		?>
		<br/>
		<style type="text/css">.schedule{font-size: 12px !important;}</style>
		<div id="printMe">
						
				<?php 
				echo '<title>'.strtoupper($customer_short_name.' BACKFLOW SCHEDULE').' - '.($month).' '.$year.'</title>';
				$time = strtotime($year.'-'.$month.'-15');
				$count = 1;
				$db = new Db();
				$sql = "SELECT * FROM metering_point, r_reading, r_rate WHERE rea_date = '$time' AND mp_customer_id = '$customer_id' AND rea_cus_id = '$customer_id' AND mp_id = rea_mp_id AND rea_id = rate_reading_id AND ( rate_lfe != '([DONOT_DISPLAY])')";
				$select = $db->select($sql);

				$total_mp = 0;
				foreach($select[0] as $row){
					extract($row);
					if(date('Y', $rea_date)==$year && date('m', $rea_date)==$month){
						$total_mp++;
					}
				}

				$units = 0.001;

				$tou = $this->tou($customer_id, $month, $year);
		
				$peak = $tou[0];
				$shoulder = $tou[1];
				$off_peak = $tou[2];

				?>
				<style>table tr td, table tr th{ height: 10px !important;} table tr th{text-align: center;} table trd:hover tdd{ background-color: #f9ff95; !important;} @media print{ #hint, #add-hint{display: none; }}</style>
			<center><h3 class="hide">UGANDA ELECTRICITY TRANSMISSION COMPANY LTD</h3></center>
			<table border="1" cellspacing="0" cellpadding="2" class="table schedule" style="font-size: 10px;border:none;font-family: arial;">
				<thead>
				<tr valign="bottom">
					<th colspan="10" style="border:2px solid black;">
					<?php
					$g = "";
					$g .= $customer_full_name;
					$g .= " BACKFLOWS SCHEDULE ";
					$g .= '<br/>';
					$g .= $year;
					$g .= ' - ';
					$g .= month_name($month);
					echo $g;
					?>
					</th>
					<th rowspan="8" style="width:20px;border:none;"></th>	
					<th style="border:none;"></th>	
					<th style="border:none;"></th>	
					<th style="border:none;"></th>				
				</tr>
				<tr valign="bottom">
					<th rowspan="4" style="border-left:2px solid black;border-bottom:2px solid black;">No.</th>	
					<th rowspan="4" style="border-left:2px solid black;border-bottom:2px solid black;">Metering Points</th>			
					<th style="border-left:2px solid black; border-right:2px solid black;" colspan="2">Shoulder</th>			
					<th style="border-left:2px solid black; border-right:2px solid black;" colspan="2">Peak</th>


					<th style="border-left:2px solid black; border-right:2px solid black;" colspan="2">Off Peak</th>	
					<th rowspan="2" style="border-left:2px solid black; border-right:2px solid black;">Total Units</th>	
					<th rowspan="2" style="border-left:2px solid black; border-right:2px solid black;">Total Value</th>			
				</tr>
				<tr valign="bottom">
					<?php $symbol = $this->rgf("currency",$customer_currency, "currency_id", "currency_symbol"); ?>

					<th style="border-left:2px solid black;border-bottom:2px solid black;">Units<br/><br/><span style="color:red;">kWh</th>

					<th style="border-right:2px solid black; border-bottom:2px solid black;">Rate<br/><span style="color:red;">@ <?php echo $shoulder.'<br/>';?></span><?php echo $symbol;?>/kWh</th>

					<th style="border-bottom:2px solid black;">Units<br/><br/><span style="color:red;">kWh</th>

					<th style="border-right:2px solid black; border-bottom:2px solid black;">Rate<br/><span style="color:red;">@ <?php echo $peak.'<br/>';?></span><?php echo $symbol;?>/kWh</th>

					<th style="border-bottom:2px solid black;">Units<br/><br/><span style="color:red;">kWh</th>

					<th style="border-right:2px solid black; border-bottom:2px solid black;">Rate<br/><span style="color:red;">@ <?php echo $off_peak.'<br/>';?></span><?php echo $symbol;?>/kWh</th>

					

					
				</tr>
				</thead>
				<tbody>
				<?php

				$db_values = array();
				$db_values[] = array(
					"No.", 
					"Metering Points", 
					"Shoulder<br/><span style='color:yellow;'>@ ".$shoulder."<br/></span>".$symbol."/kWh",
					"Peak<br/><span style='color:yellow;'>@ ".$peak."<br/></span>".$symbol."/kWh",
					"Off Peak<br/><span style='color:yellow;'>@ ".$off_peak."<br/></span>".$symbol."/kWh",
					"Advance<br/>(kWh)",
					"Amount<br/>".$symbol,
					"EMP",
					"Percentage",
					"Imports<br/>(Payable)<br/><b>".$symbol."</b>",
					"Exports<br/>(Collectable)<br/><b>".$symbol."</b>"
				);

				$tc1 = $tc2 = $tc3 = $tc4= $tc5= $tc6= $tc7= $tc8 = 0;
				$tot_amount2 = 0;
				$total_mp;
				$db = new Db();

				$units = 1; //0.001;

				$co = 0;
				$t = new Db();
				$sel = $t->select("SELECT DISTINCT mp_region_id FROM metering_point, r_reading, r_rate WHERE rea_date = '$time' AND mp_customer_id = '$customer_id' AND rea_cus_id = '$customer_id' AND mp_id = rea_mp_id AND mp_region_id IS NOT NULL AND rea_id = rate_reading_id  AND (rate_lfe != '([DONOT_DISPLAY])')");

				foreach($sel[0] as $r){
				extract($r);
			
				if(!$t->num_rows()){
					//continue;
				}else{
					//echo $t->num_rows();
				}
				$sql = "SELECT * FROM metering_point, r_reading, r_rate WHERE mp_customer_id = '$customer_id' AND rea_cus_id = '$customer_id' AND mp_id = rea_mp_id AND mp_region_id = '$mp_region_id' AND rea_id = rate_reading_id  AND (rate_lfe != '([DONOT_DISPLAY])')";

				$select = $db->select($sql);
				$tw = 0;
				
				echo '<tr><td style="border-top:2px solid black; border-left:none; border-right:none; border-bottom: 2px solid #000;"></td>
					<td colspan="9" style="border-top:2px solid black; border-left:none; border-right:none; border-bottom: 2px solid #000;color:red;"><b>'.strtoupper($this->rgf("meter_region", $mp_region_id, "region_id", "region_name")).'</b></td>
					<td style="border:none;"></td>
					</tr>';

				$db_values[] = array("", '&nbsp;<Br/><span style="color:red;font-weight: bold">'.strtoupper($this->rgf("meter_region", $mp_region_id, "region_id", "region_name")).'</span>',
				'','','','','','',
				//'&nbsp;<Br/><span style="color:red;font-weight: bold">'.strtoupper($this->rgf("meter_region", $mp_region_id, "region_id", "region_name")).'</span>',
				);

				
				foreach($select[0] as $row){
					extract($row);

					if(date('Y', $rea_date)==$year && date('m', $rea_date)==$month && $id == 2019){
						//selecting rates

						
						$db = new Db();
						$sel = $db->select("SELECT * FROM r_rate WHERE rate_reading_id = '$rea_id' ");
						@extract($sel[0][0]);
						
						//calling the function
						$cal = rate_calculate2($customer_id, ($rea_date), $mp_id);
						//echo $mp_location.' == '.$cal[4].'<br/>';
						$tw += $cal[3]*$mp_wheeling_charge/100;
						$amount2 = ($cal[4]*$shoulder+$cal[5]*$peak+$cal[6]*$off_peak)/1000; //exports

						// if($amount2){
						if($amount2 == 0){ //PATH AT ZAMBIA 
							continue;
						}else{
						

						$tot_amount2 += $amount2;
						if($co%2 != 0){
							$df = "background-color: #e6e6e6;";
						}else{
							$df = "background-color:#fff;";
						}
						$co++;
						
						$db_values[] = array(
							$co, 
							$mp_location,
							number_format($cal[0]*$units),
							number_format($cal[1]*$units),
							number_format($cal[2]*$units),
							number_format($cal[3]*$units),
							number_format($amount),
							"",
							number_format($mp_wheeling_charge,2).'&nbsp;%',
							number_format($amount*$mp_wheeling_charge/100),
							number_format($wheeling2)
						);
						//if($count != $total_mp){
						if(1){
							echo '<tr id="'.($rea_id+5).'">';
							echo '<td style="'.$df.' border-left:2px solid #000;">'.($co).'.</td>';
							echo '<td style="'.$df.' border-left:2px solid black;">'.' '.$mp_location;

							{
								echo '<td style="'.$df.'border-left:2px solid black; text-align:right;">'.number_format($cal[4]*$units/1000).'</td>';
								echo '<td style="'.$df.'border-right:2px solid black; text-align:right;">'.number_format($cal[4]*$units*$shoulder/1000).'</td>';

								echo '<td style="'.$df.' text-align:right;">'.number_format($cal[5]*$units/1000).'</td>';
								echo '<td style="'.$df.'border-right:2px solid black; text-align:right;">@@@@'.number_format($cal[5]*$units*$peak/1000).'</td>';

								echo '<td style="'.$df.'text-align:right;">'.number_format($cal[6]*$units/1000).'</td>';
								echo '<td style="'.$df.'border-right:2px solid black;  text-align:right;">'.number_format($cal[6]*$units*$off_peak/1000).'</td>';

			
								echo '<td style="'.$df.'border-right:2px solid black;  text-align:right;">'.number_format($cal[7]*$units/1000).'</td>';
								echo '<td style="'.$df.'border-right:2px solid black; text-align:right;">'.number_format($amount2).'</td>';
							}

													
							echo '</tr>';

							$tc1 += $cal[0];
							$tc2 += $cal[1];
							$tc3 += $cal[2];
							$tc4 += $cal[3];
							$tc5 += $cal[4];
							$tc6 += $cal[5];
							$tc7 += $cal[6];
							$tc8 += $cal[7];
						}					

					}					
				}
				
				}

				}
				
				{
					$l = $this->sumFormulae($customer_id, $year, $month,1);

					//$tc1 = $l[0];
					//$tc2 = $l[1];
					//$tc3 = $l[2];
					//$tc4 = $l[3];
					//$tc5 = $l[4];
					//$tc6 = $l[5];
					//$tc7 = $l[6];
					//$tc8 = $l[7];

					echo '<tr>';
					
					echo '<td colspan="2" style="border-top:2px solid black; border-left:2px solid black; border-bottom:2px solid black; border-right:2px solid black; font-weight: bold; ">Total</td>';			
				

					if($customer_id == 2019){
						echo '<td style="border-top:2px solid black; border-left:2px solid black; border-bottom:2px solid black;  text-align:right;">'.number_format($tc5*$units/1000).'</td>';

						echo '<td style="border-top:2px solid black; border-right:2px solid black; border-bottom:2px solid black;  text-align:right;">'.number_format($tc5*$units*$shoulder/1000).'</td>';

						echo '<td style="border-top:2px solid black; border-bottom:2px solid black;  text-align:right;">'.number_format($tc6*$units/1000).'</td>';
						echo '<td style="border-top:2px solid black;border-right:2px solid black; border-bottom:2px solid black;  text-align:right;">'.number_format($tc6*$units*$peak/1000).'</td>';

						echo '<td style="border-top:2px solid black; border-bottom:2px solid black;  text-align:right;">'.number_format($tc7*$units/1000).'</td>';
						echo '<td style="border-top:2px solid black;border-right:2px solid black; border-bottom:2px solid black;  text-align:right;">'.number_format($tc7*$units*$off_peak/1000).'</td>';

						echo '<td style="border-top:2px solid black; border-right:2px solid black; border-bottom:2px solid black; text-align:right; ">'.number_format($tc8*$units/1000).'</td>';
						echo '<td style="border-top:2px solid black; border-right:2px solid black; border-bottom:2px solid black; text-align:right; ">'.number_format($tot_amount2).'</td>';
					}

					echo '</tr>';
				}
				

				?>
			</tbody></table>

			<?php $this->energyApprovals(user_id(), $year, $month, $invoice=1); ?>
		</div>
				
	
	</div>
	<?php
	}

	public function noSpace($string){
		return str_replace(" ", "&nbsp;", $string);
	}

	public function collectable($customer_payable, $customer_collectable, $year, $month, $consider=0){

		$db = new Db();

		$isSNEL = $customer_collectable;		

		$tou = $this->tou($customer_collectable, $month, $year);
		
		$peak = $tou[0];
		$shoulder = $tou[1];
		$off_peak = $tou[2];

		$all_points_names = array();
		$all_points_ids = array();



		$db = new Db();
		
		$sel = $db->select("SELECT bn_metering_point FROM boundary_node WHERE bn_customer_payable_to = '$customer_payable' AND bn_customer = '$customer_collectable' ");
		
		foreach($sel[0] as $ro){
			extract($ro);
			
			$zz = rate_calculate($customer_collectable, strtotime(date("$year-$month-15")), $bn_metering_point);
			if($zz[24]!="([DONOT_DISPLAY])")
			{
				//echo ucwords(strtolower($this->rgf("metering_point", $bn_metering_point, "mp_id", "mp_location")));
				//echo $bn_metering_point.'>> ';
				$all_points_ids[] = $bn_metering_point;
			}
		}
		
		foreach($all_points_ids as $point_id){
			$all_points_names[] = ucwords(strtolower($this->rgf("metering_point", $point_id, "mp_id", "mp_location")));
		}
		/*
		if($customer_collectable == 1017 && $customer_payable == 2019){
			$tou = $this->tou($customer_payable, $month, $year);
			
			$peak = $tou[0];
			$shoulder = $tou[1];
			$off_peak = $tou[2];

			$l = $this->sumFormulae($customer_collectable, $year, $month);
						
		}else
		*/

		if($customer_payable == 2019 && $customer_collectable == 101712){ //BECS - kiitumba to UMEME;
			$tou = $this->tou($customer_collectable, $month, $year);
			
			$peak = $tou[0];
			$shoulder = $tou[1];
			$off_peak = $tou[2];

			$select = "SELECT mp_wheeling_charge FROM metering_point WHERE mp_id = $mp_id";
			$db = new Db();
			$select = $db->select($select);
			extract($select[0][0]);
						
			$k = rate_calculate($customer_collectable, strtotime(date("$year-$month-15")), $mp_id);
			$l[0] = $k[0]*$mp_wheeling_charge/100;
			$l[1] = $k[1]*$mp_wheeling_charge/100;
			$l[2] = $k[2]*$mp_wheeling_charge/100;
			$l[3] = $k[3]*$mp_wheeling_charge/100;

		}elseif($customer_payable == 202012 && $customer_collectable == 202212){ //KRECS - kiseruka to UMEME;

			$tou = $this->tou($customer_payable, $month, $year);
			
			$peak = $tou[0];
			$shoulder = $tou[1];
			$off_peak = $tou[2];

			$k = rate_calculate($customer_collectable, strtotime(date("$year-$month-15")), 5315);
			
			$l[0] = $k[4];
			$l[1] = $k[5];
			$l[2] = $k[6];
			$l[3] = $k[7];

		}elseif($customer_payable == 2019 && $customer_collectable == 1017122){ //BECS - kiitumba to UMEME;

			$tou = $this->tou($customer_collectable, $month, $year);
			
			$peak = $tou[0];
			$shoulder = $tou[1];
			$off_peak = $tou[2];

			$k = rate_calculate($customer_collectable, strtotime(date("$year-$month-15")), 3009);
			print_r($k);
			echo $l[0] = $k[4];
			echo $l[1] = $k[5];
			$l[2] = $k[6];
			$l[3] = $k[7];

		}else{
			$l = $this->rateSum2($customer_collectable, $year, $month, $all_points_ids);
		}
		
		//====================imports===========================
		$tc1 = $l[0]; //imports shoulder advance
		$tc2 = $l[1]; //imports peak advance
		$tc3 = $l[2]; //imports off peak advance
		$tc4 = $l[3]; //total imports



		//======================exports=========================
		$tc5 = $l[4];   
		$tc6 = $l[5];
		$tc7 = $l[6];
		$tc8 = $l[7];

		$tot = ($l[0]*$shoulder+$l[1]*$peak+$l[2]*$off_peak)/1000;

		$tot1 = ($l[0])/1000;
		$tot2 = ($l[1])/1000;
		$tot3 = ($l[2])/1000;

		$tot4 = ($l[0]+$l[1]+$l[2])/1000;

		$all = array(
			$all_points_ids, 
			$all_points_names, 
			$tot+$to, 
									// 2 ==> total amount payable by the customer
			$tot1+$to1, 
			$tot2+$to2, 
			$tot3+$to3, 
			$tot4+$to4
		);

		return $all;

	}

	public function collectable2($customer_payable, $customer_collectable, $year, $month, $consider=0){

		$db = new Db();

		$isSNEL = $customer_payable;

		$tou = $this->tou($customer_payable, $month, $year);
		
		$peak = $tou[0];
		$shoulder = $tou[1];
		$off_peak = $tou[2];	

		$db = new Db();
		
		$sel = $db->select("SELECT bn_metering_point FROM boundary_node WHERE bn_customer_payable_to = '$customer_payable' AND bn_customer = '$customer_collectable' ");
		

		$all_points_ids = array();
		foreach($sel[0] as $ro){
			extract($ro);
			$all_points_ids[] = $bn_metering_point;

		}

		$all_points_names = array();
		foreach($all_points_ids as $point_id){
			$all_points_names[] = ucwords(strtolower($this->rgf("metering_point", $point_id, "mp_id", "mp_location")));
		}


		if($customer_payable == 2019 && $customer_collectable != 2021 && $customer_collectable != 2020){
			if($customer_collectable == 1017){
				$tou = $this->tou($customer_payable, $month, $year);
			
				$peak = $tou[0];
				$shoulder = $tou[1];
				$off_peak = $tou[2];

				$l = $this->sumFormulae($customer_collectable, $year, $month);
			}elseif($customer_collectable == 2021){
				$tou = $this->tou($customer_payable, $month, $year);
			
				echo '>>'.$peak = $tou[0];
				echo '>>'.$shoulder = $tou[1];
				echo '>>'.$off_peak = $tou[2];

				$l = $this->rateSum2($customer_collectable, $year, $month, $all_points_ids);
			}else{
				$l = $this->rateSum2($customer_collectable, $year, $month, $all_points_ids, 100);
			}
		}else{
			if($customer_payable == 2019 && ($customer_collectable == 2021 || $customer_collectable == 2020)){
				$tou = $this->tou($customer_payable, $month, $year);
			
				$peak = $tou[0];
				$shoulder = $tou[1];
				$off_peak = $tou[2];
			}

			$l = $this->rateSum2($customer_collectable, $year, $month, $all_points_ids);
		}

		//====================imports===========================
		$tc1 = $l[0]; //imports shoulder advance
		$tc2 = $l[1]; //imports peak advance
		$tc3 = $l[2]; //imports off peak advance
		$tc4 = $l[3]; //total imports

		//======================exports=========================
		$tc5 = $l[4];   
		$tc6 = $l[5];
		$tc7 = $l[6];
		$tc8 = $l[7];

		$tot = ($l[0]*$shoulder+$l[1]*$peak+$l[2]*$off_peak)/1000;

		$tot1 = ($l[0])/1000;
		$tot2 = ($l[1])/1000;
		$tot3 = ($l[2])/1000;

		$tot4 = ($l[0]+$l[1]+$l[2])/1000;

		$all = array(
			$all_points_ids, 
			$all_points_names, 
			$tot, 
									// 2 ==> total amount payable by the customer
			$tot1, 
			$tot2, 
			$tot3, 
			$tot4
		);

		return $all;

	}
	public function payable($mp_id, $customer_id, $consider = 0, $year=0, $month=0){
		$db = new Db();
		//echo $year.' >>> '.$month.'<br/>';
		$select = $db->select("SELECT bn_customer_payable_to FROM boundary_node WHERE bn_metering_point = '$mp_id' AND bn_customer = '$customer_id'");
		extract($select[0][0]);
		$zz = rate_calculate($customer_id, strtotime(date("$year-$month-15")), $mp_id);
						
		if($zz[24]!="([DONOT_DISPLAY])"){
			return $bn_customer_payable_to;
		}else{
			return 0;
		}
		
	}

	public function swap(){

		$mp_id = portion(3);
		$customer_id = portion(4);
		$year = portion(5);
		$month = portion(6);
		$current_month = portion(7);

		if(empty($current_month)){
			$y = $year;
			$m = $month;
		}else{
			if($current_month==12){
				$y = $year+1;
				$m = $current_month;
			}else{
				$y = $year;
				$m = $current_month;
			}
		}

		$time_readings = strtotime(date($year.'-'.$month.'-15'));
		$point = "";
		$db = new Db();
		$select = $db->select("SELECT * FROM r_reading WHERE rea_date = '$time_readings' AND rea_cus_id = '$customer_id' AND rea_mp_id = '$mp_id'");

		if($db->num_rows()){
			extract($select[0][0]);
			$rrr = $rea_id;

			//=====================================================================
			//checking which part to use for advance
			//=====================================================================

			if(1){ //imports as advance
				$select = $db->select("SELECT * FROM r_rate WHERE rate_cus_id = '$customer_id' AND rate_reading_id = '$rrr'");

				extract($select[0][0]);

				$rate1 = $rate_import_wh_1;
				$rate2 = $rate_import_wh_2;
				$rate3 = $rate_import_wh_3;
				$rate4 = $rate_export_wh_4;
				$rate5 = $rate_export_wh_5;
				$rate6 = $rate_export_wh_6;
			
				$select = $db->update("r_rate",[
					"rate_import_wh_1"=>$rate4,
					"rate_import_wh_2"=>$rate5,
					"rate_import_wh_3"=>$rate6,
					"rate_export_wh_4"=>$rate1,
					"rate_export_wh_5"=>$rate2,
					"rate_export_wh_6"=>$rate3,
					"rate_added_by"=>user_id(),
					"rate_date_added"=>time(),
				], ["rate_cus_id"=>$customer_id, "rate_reading_id"=>$rrr]);

				FeedBack::redirect(return_url().'services/readings/'.$customer_id.'/'.$y.'/'.$m);
				
			}
		}else{
			$error[] = "Readings Do not Exist for <b>".$this->rgf('customer',$customer_id, 'customer_id', 'customer_short_name').'</b> Metering Point <b>'.$point.'</b>';
		}

	}

	public function editInvoiceReadingAction(){

		echo '<form action="" method="post"> ';

		echo '<a href="'.return_url().'" class="btn btn-success btn-xs"><i class="fa fa-fw fa-undo"></i> Back </a><br/><br/>';

		$mp_id = portion(3);
		$customer_id = portion(4);
		$year = portion(5);
		$month = portion(6);
		$part = portion(7);
		$parts = ["Shoulder Imports", "Peak Imports", "Off Peak Imports","Shoulder Exports", "Peak Exports", "Off Peak Exports"];

		if($part <= 3){
			$rat = 'rate_import_wh_'.$part;
		}else{
			$rat = 'rate_export_wh_'.$part;
		}

		$time_readings = strtotime(date($year.'-'.$month.'-15'));
		$point = "";
		$db = new Db();
		$select = $db->select("SELECT * FROM r_reading WHERE rea_date = '$time_readings' AND rea_cus_id = '$customer_id' AND rea_mp_id = '$mp_id'");

		if($db->num_rows()){
			extract($select[0][0]);
			$rrr = $rea_id;

			//=====================================================================
			//checking which part to use for advance
			//=====================================================================

			if(1){ //imports as advance
				$select = $db->select("SELECT * FROM r_rate WHERE rate_cus_id = '$customer_id' AND rate_reading_id = '$rrr'");

				extract($select[0][0]);

				$rate = "${$rat}";
				if(isset($_POST['send'])){
					$rate = $_POST['rate'];
					$x[$rat] = $rate;
					$select = $db->update("r_rate",$x, ["rate_cus_id"=>$customer_id, "rate_reading_id"=>$rrr]);

					FeedBack::redirect(return_url().'services/readings/'.$customer_id.'/'.$year.'/'.$month);
				}
				echo '<br/>';
				echo '<table>';
				echo '<tr>';
				echo '<td><b>Customer: </b></td><td>'.$this->rgf("customer", $customer_id, "customer_id", "customer_short_name").'</td>';
				echo '<tr>';
				echo '<td><b>Reading:</b></td><td> '.$parts[$part-1].'</td>';
				echo '</tr>';

				echo '<tr>';
				echo '<td><b>Year: </b></td><td>'.$year.'</td>';
				echo '</tr>';
				
				echo '<tr>';
				echo '<td><b>Month: </b></td><td>'.month_name($month).'</td>';
				echo '</tr>';
				
				echo '<tr>';
				echo '<td><b>Metering Point: </b> &nbsp; </td><td>'.$this->rgf("metering_point", $mp_id, "mp_id", "mp_location").'</td>';
				echo '</tr>';
				
				echo '</table>';

				echo '<br/><br/>';
				echo '<b><span style="color:red;">Readings In: kWh</span></b>';
				
				echo '<input type="number" step="any" value="'.$rate.'" name="rate"/>';
				echo '<input type="submit" value="Update" name="send"/>';
				echo '</form>';
			
				
				
			}
		}else{
			$error[] = "Readings Do not Exist for <b>".$this->rgf('customer',$customer_id, 'customer_id', 'customer_short_name').'</b> Metering Point <b>'.$point.'</b>';
		}

	}

	public function ScheduleAction(){
		
		$class = ($this->set_class)?$this->set_class:portion(1);
		$method = ($this->set_method)?$this->set_method:portion(2);
		$id = ($this->set_customer)?$this->set_customer:portion(3);
		$year = ($this->set_year)?$this->set_year: portion(4);
		$month = ($this->set_month)?$this->set_month: portion(5);
		$note = ($this->set_note)?$this->set_note: portion(6);

		if(empty($year) || empty($month) || empty($id)){
			Feedback::error("Please select Customer, Year and Month");
			$units = $this->customerAndSchedule($id, $year, $month);
			$this->energyLinks($id, $year, $month);
			$units = 0.001;
			echo '<div class="clearfix"></div>';
		}elseif(!$this->readingExist($id, $year, $month)){
			Feedback::error("No readings found");
			$units = $this->customerAndSchedule($id, $year, $month);
			$this->energyLinks($id, $year, $month);
			$units = 0.001;
			echo '<div class="clearfix"></div>';
		}else{
			if($note == "credit-note"){
				$this->ScheduleCreditNote($id, $year, $month);
			}else{
				$this->ScheduleNormal($id, $year, $month);
			}
		}


	}

	private function ex_rate($year, $month, $curr){

		$db = new Db();
		$select = $db->select("SELECT * FROM exchange_rate WHERE ex_year='$year' AND ex_month='$month'");
		if($db->num_rows()){
			extract($select[0][0]);
			
			return ${"ex_".$curr};
		}
	}

	private function mixPrice($year, $month, $customer_id, $value){

		$db = new Db();
		$select = $db->select("SELECT * FROM mix_price WHERE mix_year='$year' AND mix_month='$month' AND mix_generator_id = '$customer_id'");
		if($db->num_rows()){
			extract($select[0][0]);

			return ${"mix_".$value};
		}
	}

	private function values($year, $month, $customer_id){

		$units = $this->mixPrice($year, $month, $customer_id, "units");
		$amount = $this->mixPrice($year, $month, $customer_id, "amount");
		$amount2 = $this->mixPrice($year, $month, $customer_id, "amount2");
		$amount3 = $this->mixPrice($year, $month, $customer_id, "amount3");
		$cd = $this->mixPrice($year, $month, $customer_id, "currency");

		$curr = $this->rgf("generators", $customer_id, "ge_id", "ge_currency");

		$db = new Db();
		$select = $db->select("SELECT ex_usd, ex_euro, ex_shs FROM exchange_rate WHERE ex_year = '$year' AND ex_month = '$month'");
		extract($select[0][0]);

		$value = array(); 

		if($customer_id == 4){ //bujagali
			$value['usd'] = $amount;
			$value['shs'] = $amount3;
			$value['euro'] = $amount*$ex_usd/$ex_euro;
		}elseif($customer_id == 10){ //jacobsen
			$value['usd'] = $amount;
			$value['shs'] = $amount*$ex_usd;
			$value['euro'] = $amount2;
		}elseif(strtoupper($curr) == "USD"){
			$value['usd'] = $amount;
			$value['shs'] = $amount*$ex_usd;
			$value['euro'] = $amount*$ex_usd/$ex_euro;
		}elseif(strtoupper($curr) == "UGX"){
			$value['shs'] = $amount3;
			$value['usd'] = $amount3/$ex_usd;
			$value['euro'] = $amount3/$ex_euro;
		}elseif(strtoupper($curr) == "EUR"){
			$value['euro'] = $amount2;
			$value['shs'] = $amount2*$ex_euro;
			$value['usd'] = $amount2*$ex_euro/$ex_usd;
		}
		
		//echo '<br/>'.$value['euro'];
		// if(empty($value['usd']) && empty($value['$units'])){
		// 	$value['price'] = "A&U";
		// }elseif(empty($value['$units'])){
		// 	$value['price'] = "U";
		// }elseif(empty($value['usd'])){
		// 	$value['price'] = "A";
		// }else
		{
			

			$v = ((float)$value['usd']/(float)($units));
			$v = (is_nan($v))?0:$v;

			if($customer_id == 4){ //bujagali
				$value['price'] = ($value['usd']+$value['shs']/$ex_usd)/$units;
				$value['formulae'] = "(Bill_USD+Bill_SHS/Ex_USD)/Units";
			}elseif($customer_id == 10){//jacobsen
				$value['price'] = ($value['usd']+($value['euro']*$ex_euro/$ex_usd))/$units;
				$value['formulae'] = "(Bill_USD+(Bill_EUR*Ex_EUR/Ex_USD))/Units";
			}elseif($customer_id == 3){
				$value['price'] = $v+0.03;
				$value['formulae'] = "Bill_USD/Units+0.03";
			}else{
				$value['price'] = $v;
				$value['formulae'] = "Bill_USD/Units";
			}

			$value['units'] = $units;
			$value['currency'] = $cd;
		}

		return $value;
	}

	public function sum($year, $month, $type){
		$db = new Db();
		
		if($type==5){
			$sql = "SELECT ge_name, ge_id, ge_category FROM generators WHERE ge_id = 1015 or ge_id = 38 or ge_id = 10 ORDER BY ge_name ASC";
			$select = $db->select($sql);
		}else{
			//$sql = "SELECT ge_name, ge_id,ge_category FROM generators WHERE ge_id != 38 AND ge_id != 10 ORDER BY ge_name ASC";
			$sql = "SELECT ge_name, ge_id,ge_category FROM generators ORDER BY ge_name ASC";
			$select = $db->select($sql);
		}

		$top = 0;
		$bottom =  0;



		foreach($select[0] as $row){
			extract($row);
			$v = $this->get_bill_price2($year, $month, $ge_id);
							
			//echo '<br/> >>> '.($v['units']);
			//echo '<br/>'.
			$val = str_replace(',', '', $v['units'])*$v['price'];

			//$val = (is_nan($val))?0:$val;
			//$val = ($val==INF)?0:$val;

			//echo $val.'... ';
			$top += str_replace(',', '', $v['units'])*$v['price'];
			$bottom += str_replace(',','',$v['units']);


			// echo '<tr>';
			// echo '<td>';
			// echo $ge_name;
			// echo '</td>';
			// echo '<td>'.$v['units'];
			// echo '</td>';
			// echo '<td>'.$v['units'];
			// echo '</td>';
			// echo '</tr>';
		//echo '<br/>';

		//echo '<p style="text-align:left;"><b>'.$ge_name.'</b><br/>'.'['.($v['units']).'*<b>'.$v['price'].'</b>='.(str_replace(',','',$v['units'])*$v['price']).']</p>';

			//echo $ge_name.'='.$v['units'].'='.$v['price'].'='.$top.'<br/>';

		}


		//echo 't: '.number_format($top, 4).'<br/>';

		//echo 'b: '.number_format($bottom, 10).'<br/>';

		if($type!=5){
			$select = $db->select("SELECT ex_loss_factor FROM exchange_rate WHERE ex_year = '$year' AND ex_month = '$month'");
			extract($select[0][0]);
			$bottom = (1-$ex_loss_factor)*$bottom;
		}
		
		//echo 'Top:'.$top.'<br/>Bot: '.$bottom.'<br/>';

		$value = number_format(($top/$bottom)*100, 20);
		//echo '<br/>';
		//$value;
		//return $top;
		return $value;
	}

	public function ScheduleNormal($id, $year, $month){

		$db = new Db();
		$select = $db->select("SELECT * FROM customer WHERE customer_id = '$id'");
		extract($select[0][0]);
	
	?>
	<div id="side">
		<?php 
		$units = $this->customerAndSchedule($id, $year, $month);
		$this->energyLinks($id, $year, $month);

		//tanseco
		echo '<style>.s{height:100px;} thead{background-color:white; }</style>';

		if($id == 2029000 || $id == 20170000) //tanesco and snel
{			echo '<div class="clearfix"></div>';
			echo '<div id="printMe">';
			echo '<style>@media print{table{font-size:10px}} @media print{ @page {size: A4; font-size:10px;  @top-left {content: "";}} </style>';
			//which quater
			$q1 = array(1,2,3);
			$q2 = array(4,5,6);
			$q3 = array(7,8,9);
			$q4 = array(10,11,12);

			$use = "";

			if(in_array($month, $q1)){
				$use = "q1";
				$factor_month = $$use[0];
			}elseif(in_array($month, $q2)){
				$use = "q2";
				$factor_month = $$use[0];
			}elseif(in_array($month, $q3)){
				$use = "q3";
				$factor_month = $$use[0];
			}elseif(in_array($month, $q4)){
				$use = "q4";
				$factor_month = $$use[0];
			}

			$q_name = strtoupper("$use $year UETCL GENERATION MIX PRICE");

			echo '<h4>'.$q_name.'</h4>';
			echo '<table id="table" border="1" cellpadding="2" cellspacing="0" style="font-family:arial;font-size:12px;">';
			echo '<thead>';
			echo '<tr align="center" valign="bottom" style=" font-weight:bold;">';			
			echo '<th style="height:120px; border-top:2px solid #000; border-bottom:2px solid #000; border-left:2px solid #000; " rowspan="3">No.</th>';
			echo '<th style="border-top:2px solid #000; border-bottom:2px solid #000; border-right:2px solid #000; border-left:2px solid #000; " rowspan="3">Customer</th>';

			foreach($$use as $m){
				if(1)
						echo '<th colspan="3" style="border-top:2px solid #000; border-right:2px solid #000;"><b>'.month_name($m).'</b></th>';
				else
						echo '<th colspan="5" style="border-top:2px solid #000; border-right:2px solid #000;"><b>'.month_name($m).'</b></th>';
			}
			echo '</tr>';

			echo '<tr align="center" valign="bottom" style="font-weight:bold;">';
			foreach($$use as $m){
				echo '<th rowspan="2" style="border-bottom:2px solid #000;">Units<b><br/><span style="">(KWH)</span></th>';
				if(1)
				echo '<th>Bill</td>';
				else
				echo '<th colspan="3">Bill</th>';
				echo '<th rowspan="2" style="border-bottom:2px solid #000; border-right:2px solid #000;">Price<br/><span style="">(USD/kWh)</span></th>';
			}
			echo '</tr>';

			echo '<tr align="center" valign="bottom" style="font-weight:bold;">';
			foreach($$use as $m)
			{

				
				echo '<th style="border-bottom:2px solid #000;">USD:<span style="">';
			
				echo '@'.number_format($this->ex_rate($year, $m, 'usd'), 2).'/shs</span>';
				echo '<br/>Euro:<span style="">@'.number_format($this->ex_rate($year, $m, 'euro'), 2).'/shs</span>';
				//echo '<br/>Shs:<span style="color:red;"></span>';
				echo '</th>';
				

			}

			echo '</tr>';
			echo '</thead>';
			echo '<tbody>';
			$select = $db->select("SELECT * FROM generators ORDER BY ge_name ASC ");
			$no = 1;
			foreach($select[0] as $row){
				extract($row);
				if(1){
					echo '<tr>';
					echo '<td style="border-left:2px solid #000;">'.($no++).'.</td>';
					echo '<td style="min-width:120px; border-left:2px solid #000; border-right:2px solid #000;">'.strtoupper(($ge_name)).'</td>';

					foreach($$use as $m){
						$v = $this->get_bill_price2($year, $m, $ge_id);
						echo '<td style="text-align:right; min-width:80px;">'.number_format($this->mixPrice($year, $m, $ge_id, 'units')/1000).'</td>';

						if(1){
						echo '<td style="text-align:left; min-width:80px;">';
						if($v['usd'] != 0)
							echo '<b>USD:</b>&nbsp;'.number_format($v['usd']/1000).'<br/>';
						
						if($v['euro'] != 0)
							echo '<b>EUR:</b>&nbsp;'.number_format($v['euro']/1000).'<br/>';

						if($v['shs'] != 0)
							echo '<b>SHS:</b>&nbsp;'.number_format($v['shs']/1000);

						echo '</td>';

						}else{
						echo '<td style="text-align:right; min-width:80px;">'.number_format($v['usd']/1000).'</td>';
						echo '<td style="text-align:right; min-width:80px;">'.number_format($v['euro']/1000).'</td>';
						echo '<td style="text-align:right; min-width:80px;">'.number_format($v['shs']/1000).'</td>';

						}

						echo '<td style="text-align:right; border-right:2px solid #000;">';
						echo (float)str_replace(',','',number_format($v['price'],6));
						echo '<br/>';
						//echo $this->get_bill_price2($ge_id, $year, $m, $formula);
						echo '</td>';
					}

					// echo '<td></td>';
					// echo '<td></td>';
					// echo '<td></td>';
					// echo '<td></td>';
					// echo '<td style="border-right:2px solid #000"></td>';
					// echo '<td></td>';
					// echo '<td></td>';
					// echo '<td></td>';
					// echo '<td></td>';
					// echo '<td style="border-right:2px solid #000;"></td>';
					echo '</tr>';
				}
			}

			echo '<tr>';
			echo '<td colspan="2" style="border-top:2px solid #000; border-left:2px solid #000;border-right:2px solid #000;border-bottom:2px solid #000;">';
			echo 'Wt. Av. Price of Thermal :<br/>';

			$select = $db->select("SELECT * FROM generators WHERE ge_category = 5");
			$i = 0;
			$x = array();
			foreach($select[0] as $row){
				extract($row);
				$x[]=$ge_name;
			}

			//echo implode(', ', $x);

			echo '</td>';

			foreach($$use as $m){
				if(1){
					echo '<td colspan="3" style="border-top:2px solid #000; border-right:2px solid #000; border-bottom:2px solid #000; text-align:right">';
					echo $this->sum($year, $m, 5);
					echo '</td>';
				}else{				
					echo '<td colspan="5" style="border-top:2px solid #000; border-right:2px solid #000; border-bottom:2px solid #000; text-align:right">';
					echo $this->sum($year, $m, 5);
					echo '</td>';
				}
			}
			echo '</tr>';

			echo '<tr>';
			echo '<td colspan="2" style="border-left:2px solid #000;border-right:2px solid #000;border-bottom:2px solid #000;">';
			echo 'WT Ave. Price :<br/>';

			echo '</td>';

			$e = 0;

			foreach($$use as $m){
				if(1){
					echo '<td colspan="3" style="border-right:2px solid #000; border-bottom:2px solid #000; text-align:right">';
					
					if($id == 2017) $ts = 1.05; else 
					$ts = 1;
					
					$e += $this->sum($year, $m, 0)*$ts;
					echo $this->sum($year, $m, 0)*$ts;
					
					echo '</td>';
				}else{
					echo '<td colspan="5" style="border-right:2px solid #000; border-bottom:2px solid #000; text-align:right">';
					$e += $this->sum($year, $m, 0);
					echo $this->sum($year, $m, 0);
					echo '</td>';
				}
			}
			echo '</tr>';

			

			$avr = ($e/3.0);


			echo '</tbody>';
			echo '</table>';

			echo '<div style="margin:20px;margin-right:0;margin-bottom:0;font-weight:bold;">';
			echo '<div style="float:right; margin:45px; margin-right:0; font-size:20px;color:#fff; background-color:black; padding:10px;">'.$avr.'</div>';
			if($customer_id == '2017')
				echo '<div style="float:right;"><img src="'.return_url().'images/formula2.png"></div>';
			else
				echo '<div style="float:right;"><img src="'.return_url().'images/formula.png"></div>';

			echo '<div style="clear:both;"></div>';
			echo '</div>';

			

			$select = $db->select("SELECT ex_loss_factor FROM exchange_rate WHERE ex_year = '$year' AND ex_month='$factor_month'");
			extract($select[0][0]);
			$loss_factor = $ex_loss_factor;

			echo '<div style="border:2px solid #000;font-weight:bold;">';
			echo '<div style="float:left;padding:10px;">Transmission Loss Factor (Ref ERA)</div>';
			echo '<div style="float:right; color:#fff; background-color:#000; padding:10px;">'.number_format($loss_factor, 5).'</div>';
			echo '<div style="clear:both;"></div>';
			echo '</div>';

			$this->showReadingComments();

			echo '</div>';
			echo '<br/><br/>';
			$this->addReadingComment(); 
			$vvv = array();
			$select = $db->select("SELECT * FROM generators ORDER BY ge_name ASC ");
			$no = 1;
					
			// foreach($$use as $m){
			// foreach($select[0] as $row){
			// 	extract($row);
			// 	if(1){
			// 			$v = $this->get_bill_price2($year, $m, $ge_id);
			// 			$vvv[month_name($m)][] = array($v['formulae'], $ge_name);
			// 		}
			// 	}
			// }
			// $colors = array("blue", "green", "red", "green", "black", "orange", "purple",);
			// foreach($vvv as $month=>$values){
			// 	$vv = array();
			// 	echo '<br><b>'.$month.':</b><br/>';
			// 	foreach($values as $v){
			// 		$vv[] = str_replace(' ', '&nbsp;', '[<b>'.$v[1].'</b> = '.$v[0].']');
			// 	}
			// 	for($i=0; $i<count($vv); $i++){
			// 		echo '<span style="color:'.$colors[$i%count($colors)].'">'.str_replace(' ','&nbsp;',$vv[$i]).'</span>';
			// 		if($i < count($vv)-1){
			// 			echo '&nbsp; +&nbsp;';
			// 		}
			// 	}
				
			// }
		}else{
		?>
		<!-- <input type="search" class="search" data-column="all" placeholder="Search all columns">
		<button type="button" class="reset">Reset</button> -->
		<style>.tg{width:700px; margin:auto;}</style>
		<div id="printMe">
			<style>table{border:none;}table tr td, table tr th{ height: 10px !important;} table tr th{ text-align: center;} <?php if($id != 2019 && $id != 2030 && $id != 2028) { echo '@media print{ #rt{display:none;} @page {size: A4 landscape;  @top-left {content: "Ghost Stories";}} .net-advance-form, .area{display:none;}}'; }else{ echo '@media print{.net-advance-form{display:none;}}'; } ?> </style>
			<?php 
			$set_year = ($this->set_year)?$this->set_year:portion(4);
			$set_month = ($this->set_month)?$this->set_month:portion(5);
			$set_customer = ($this->set_customer)?$this->set_customer:portion(3);

			echo '<title>'.strtoupper($customer_short_name.' Meter Readings').' - '.($month = $set_month).' '.$set_year.'</title>';
			?>
			<center class="hide"><h3>UGANDA ELECTRICITY TRANSMISSION COMPANY LTD</h3></center>

			<?php 
				echo '<div style="clear:both;"></div>';

				if($set_customer==2028){

					$db = new Db();
					//current main 1
					$customer_id = $set_customer;
					$reading_time = strtotime(date("$year-$month-15"));

					$sql = "SELECT * FROM r_reading, r_rate, metering_point WHERE rea_date = '$reading_time' AND rea_cus_id = '$customer_id' AND rate_cus_id = '$customer_id' AND rate_reading_id = rea_id AND rea_mp_id = mp_id AND upper(mp_location) = 'KATUNA-MAIN'";
					$select = $db->select($sql);
					extract($select[0][0]);
					$present_main = $rate_cum_imports;

					//previous main reading
					$sql = "SELECT TOP 1 * FROM r_reading, r_rate, metering_point WHERE rea_date < '$rea_date' AND rea_id = rate_reading_id AND rea_cus_id = '$customer_id' AND rea_mp_id = mp_id AND rate_cus_id = '$customer_id' AND upper(mp_location) = 'KATUNA-MAIN' ORDER BY rea_date DESC";
					$sel = $db->select($sql);
					extract($sel[0][0]);
					$previous_main = $rate_cum_imports;


					///////////////////// check

					$sql = "SELECT * FROM r_reading, r_rate, metering_point WHERE rea_date = '$reading_time' AND rea_cus_id = '$customer_id' AND rate_cus_id = '$customer_id' AND rate_reading_id = rea_id AND rea_mp_id = mp_id AND upper(mp_location) = 'KATUNA-CHECK'";
					$select = $db->select($sql);
					extract($select[0][0]);
					$present_check = $rate_cum_imports;

					//previous main reading
					$sql = "SELECT TOP 1 * FROM r_reading, r_rate, metering_point WHERE rea_date < '$rea_date' AND rea_id = rate_reading_id AND rea_cus_id = '$customer_id' AND rea_mp_id = mp_id AND rate_cus_id = '$customer_id' AND upper(mp_location) = 'KATUNA-CHECK' ORDER BY rea_date DESC";
					$sel = $db->select($sql);
					extract($sel[0][0]);
					$previous_check = $rate_cum_imports;

					$advance_main = $present_main - $previous_main;
					$advance_check = $present_check - $previous_check;

					$average = ($advance_main+ $advance_check)/2;


				?>
				<div class="tg">
				<h3 style="text-align: center;">VISION METER DISPLAY</h3>
				<h3>METER READING POINT: &nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp; Katuna</h3>
				<h3 style="text-align: center;"><b>BULK SUPPLY METER READING FOR POWER EXPORT TO RWANDA<br/></h3>
				<h3 style="text-align: right123"> MONTH: <span style="border-bottom:2px solid #000; padding:0 10px; "><?php echo month_name($set_month).' - '.$set_year; ?></span></h3>
				<p style="font-weight: bold;">A. MAIN METER:</p>
				<table border="1" cellpadding="2" cellspacing="0" style="width:80%; text-align: center; margin-bottom:20px; font-size: 20px;">
					<tr>
						<td style="border-top:2px solid #000;border-right:2px solid #000; border-left:2px solid #000;" colspan="3" align="center">KWH Reading (Cumm Imports)</td>
					</tr>
					<tr>
						<td style="border-left:2px solid #000; ">Present</td>
						<td>Previous</td>
						<td style="border-right:2px solid #000;">Advance</td>
					</tr>
					<tr>
						<td style="border-bottom:2px solid #000;border-left:2px solid #000; "><b><?php echo number_format($present_main/1000, 2); ?></b></td>
						<td style="border-bottom:2px solid #000;"><b><?php echo number_format($previous_main/1000, 2); ?></b></td>
						<td style="border-bottom:2px solid #000; border-right:2px solid #000;"><b><?php echo number_format(($present_main-$previous_main)/1000, 2); ?></b></td>
					</tr>
				</table>
				<p style="font-weight:bold;">B. CHECK METER:</p>
				<table border="1" cellpadding="2" cellspacing="0" style="width:80%; text-align: center; margin-bottom:20px; font-size: 20px;">
					<tr>
						<td style="border-top:2px solid #000;border-right:2px solid #000; border-left:2px solid #000;" colspan="3" align="center">KWH Reading (Cumm Imports)</td>
					</tr>
					<tr>
						<td style="border-left:2px solid #000; ">Present</td>
						<td>Previous</td>
						<td style="border-right:2px solid #000;"">Advance</td>
					</tr>
					<tr>
						<td style="border-bottom:2px solid #000;border-left:2px solid #000; "><b><?php echo number_format($present_check/1000, 2); ?></b></td>
						<td style="border-bottom:2px solid #000;"><b><?php echo number_format($previous_check/1000, 2); ?></b></td>
						<td style="border-bottom:2px solid #000; border-right:2px solid #000;"><b><?php echo number_format(($present_check - $previous_check)/1000, 2); ?></b></td>
					</tr>
				</table>
				<table cellpadding="0" cellspacing="0" border="0" style="width:auto; font-size: 20px;">
					<tr>
						<td style="width:200px;">KWH</td>
						<td> &nbsp; = &nbsp; </td>
						<td style="border-left:2px solid #000;"></td>
						<td style="border-top:2px solid #000; width:10px;border-bottom:2px solid #000; width:10px;"></td>
						<td style="text-align:center;"><span style="border-bottom:2px solid #000; padding:0 15px; margin:10px;"><?php echo number_format($advance_main/1000, 2).' + '.number_format($advance_check/1000, 2); ?></span><br/>2</td>
						<td style="border-top:2px solid #000; width:10px;border-bottom:2px solid #000; width:10px;"></td>
						<td style="border-left:2px solid #000;"></td>
					</tr>
					<tr>
						<td style="width:200px;"></td>
						<td colspan="6" style="text-align: right;"><div style="border:1px solid #000; padding:5px; margin:10px; margin-right:0px; background-color:yellow; font-weight: bold;"><?php echo number_format($average/1000, 2); ?></div></td>			
					</tr>
				</table>
			</div>

				<?php
				}elseif($set_customer==202900000){ //to invoice ==============
					$db = new Db();
					//current main 1
					$customer_id = $set_customer;
					$reading_time = strtotime(date("$year-$month-15"));

					$sql = "SELECT * FROM r_reading, r_rate, metering_point WHERE rea_date = '$reading_time' AND rea_cus_id = '$customer_id' AND rate_cus_id = '$customer_id' AND rate_reading_id = rea_id AND rea_mp_id = mp_id";
					$select = $db->select($sql);

					$total_advance = 0;

					foreach($select[0] as $row){
					
						extract($row);
						$present_main = $rate_cum_imports;
						$present_main_ex = $rate_cum_exports;
						$mp_location123 = $mp_location;

						//previous main reading
						$sql = "SELECT TOP 1 * FROM r_reading, r_rate, metering_point WHERE rea_date < '$rea_date' AND rea_id = rate_reading_id AND rea_cus_id = '$customer_id' AND rea_mp_id = '$mp_id' AND rate_cus_id = '$customer_id' ORDER BY rea_date DESC";
						$sel = $db->select($sql);
						extract($sel[0][0]);
						$previous_main = $rate_cum_imports;
						$previous_main_ex = $rate_cum_exports;

						$total_advance += (($present_main-$previous_main) - ($present_main_ex-$previous_main_ex))/1000;
					}

					$total_advance;

				}elseif($set_customer==2029){
					?>
					<div class="tg">
					<h3 style="text-align: center;">VISION METER DISPLAY</h3>
					<h3>METER READING POINT: &nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp; TANESCO</h3>
					<h3 style="text-align: center;"><b>BULK SUPPLY METER READING FOR POWER EXPORT TO TANESCO<br/></h3>
					<h3 style="text-align: right123"> MONTH: <span style="border-bottom:2px solid #000; padding:0 10px; "><?php echo month_name($set_month).' - '.$set_year; ?></span></h3>

					<?php

					$db = new Db();
					//current main 1
					$customer_id = $set_customer;
					$reading_time = strtotime(date("$year-$month-15"));

					$sql = "SELECT * FROM r_reading, r_rate, metering_point WHERE rea_date = '$reading_time' AND rea_cus_id = '$customer_id' AND rate_cus_id = '$customer_id' AND rate_reading_id = rea_id AND rea_mp_id = mp_id";
					$select = $db->select($sql);

					$total_advance = 0;

					foreach($select[0] as $row){
					
					extract($row);
					$present_main = $rate_cum_imports;
					$present_main_ex = $rate_cum_exports;
					$mp_location123 = $mp_location;

					//previous main reading
					$sql = "SELECT TOP 1 * FROM r_reading, r_rate, metering_point WHERE rea_date < '$rea_date' AND rea_id = rate_reading_id AND rea_cus_id = '$customer_id' AND rea_mp_id = '$mp_id' AND rate_cus_id = '$customer_id' ORDER BY rea_date DESC";
					$sel = $db->select($sql);
					extract($sel[0][0]);
					$previous_main = $rate_cum_imports;
					$previous_main_ex = $rate_cum_exports;

				?>
				
				<p style="font-weight: bold;">METER: <?php echo $mp_location123; ?></p>
				<table border="1" cellpadding="2" cellspacing="0" style="width:80%; text-align: center; margin-bottom:20px; font-size: 18px;">
					<tr>
						<td style="border-top:2px solid #000;border-right:2px solid #000; border-left:2px solid #000;" colspan="4" align="center">KWH Reading </td>
					</tr>
					<tr>
						<td style="border-left:2px solid #000; "></td>
						<td style="">Present</td>
						<td>Previous</td>
						<td style="border-right:2px solid #000;">Advance</td>
					</tr>
					<tr>
						<td style="border-left:2px solid #000; "><b>Present Imports</b></td>
						<td style=""><b><?php echo number_format($present_main/1000, 2); ?></b></td>
						<td style=""><b><?php echo number_format($previous_main/1000, 2); ?></b></td>
						<td style=" border-right:2px solid #000;"><b><?php echo number_format(($present_main-$previous_main)/1000, 2); ?></b></td>
					</tr>
					<tr>
						<td style="border-left:2px solid #000; "><b>Present Exports</b></td>
						<td style=""><b><?php echo number_format($present_main_ex/1000, 2); ?></b></td>
						<td style=""><b><?php echo number_format($previous_main_ex/1000, 2); ?></b></td>
						<td style=" border-right:2px solid #000;"><b><?php echo number_format(($present_main_ex-$previous_main_ex)/1000, 2); ?></b></td>
					</tr>
					<tr>
						<td style="border-bottom:2px solid #000;border-left:2px solid #000; "><b>Advance</b></td>
						<td style="border-bottom:2px solid #000;"><b><?php echo number_format(($present_main - $present_main_ex)/1000, 2); ?></b></td>
						<td style="border-bottom:2px solid #000;"><b><?php echo number_format(($previous_main - $previous_main_ex)/1000, 2); ?></b></td>
						<td style="border-bottom:2px solid #000; border-right:2px solid #000;"><b><?php echo number_format((($present_main-$previous_main) - ($present_main_ex-$previous_main_ex))/1000, 2); ?></b></td>
					</tr>
				</table>				
			
				<?php

				$total_advance += (($present_main-$previous_main) - ($present_main_ex-$previous_main_ex))/1000;


			}

			?>

			<p style="font-size:18px;">Total Advance = <span style="background-color:yellow; border:1px solid #000;padding:10px;"><?php echo number_format($total_advance, 2); ?></span></p> 
			</div>
			<?php
				}elseif($set_customer==2030){
					?>
					<div class="tg">
					<h3 style="text-align: center;">VISION METER DISPLAY</h3>
					<h3>METER READING POINT: &nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp; KPLC</h3>
					<h3 style="text-align: center;"><b>BULK SUPPLY METER READING FOR POWER EXPORT TO KPLC<br/></h3>
					<h3 style="text-align: right123"> MONTH: <span style="border-bottom:2px solid #000; padding:0 10px; "><?php echo month_name($set_month).' - '.$set_year; ?></span></h3>

					<?php

					$db = new Db();
					//current main 1
					$customer_id = $set_customer;
					$reading_time = strtotime(date("$year-$month-15"));

					$sql = "SELECT * FROM r_reading, r_rate, metering_point WHERE rea_date = '$reading_time' AND rea_cus_id = '$customer_id' AND rate_cus_id = '$customer_id' AND rate_reading_id = rea_id AND rea_mp_id = mp_id";
					$select = $db->select($sql);

					$total_advance = 0;

					foreach($select[0] as $row){
					
					extract($row);
					$present_main = $rate_cum_imports;
					$present_main_ex = $rate_cum_exports;
					$mp_location123 = $mp_location;

					//previous main reading
					$sql = "SELECT TOP 1 * FROM r_reading, r_rate, metering_point WHERE rea_date < '$rea_date' AND rea_id = rate_reading_id AND rea_cus_id = '$customer_id' AND rea_mp_id = '$mp_id' AND rate_cus_id = '$customer_id' ORDER BY rea_date DESC";
					$sel = $db->select($sql);
					extract($sel[0][0]);
					$previous_main = $rate_cum_imports;
					$previous_main_ex = $rate_cum_exports;

				?>
				
				<p style="font-weight: bold;">METER: <?php echo $mp_location123; ?></p>
				<table border="1" cellpadding="2" cellspacing="0" style="width:80%; text-align: center; margin-bottom:20px; font-size: 18px;">
					<tr>
						<td style="border-top:2px solid #000;border-right:2px solid #000; border-left:2px solid #000;" colspan="4" align="center">KWH Reading </td>
					</tr>
					<tr>
						<td style="border-left:2px solid #000; "></td>
						<td style="">Present</td>
						<td>Previous</td>
						<td style="border-right:2px solid #000;">Advance</td>
					</tr>
					<tr>
						<td style="border-left:2px solid #000; "><b>Present Imports</b></td>
						<td style=""><b><?php echo number_format($present_main/1000, 2); ?></b></td>
						<td style=""><b><?php echo number_format($previous_main/1000, 2); ?></b></td>
						<td style=" border-right:2px solid #000;"><b><?php echo number_format(($present_main-$previous_main)/1000, 2); ?></b></td>
					</tr>
					<tr>
						<td style="border-left:2px solid #000; "><b>Present Exports</b></td>
						<td style=""><b><?php echo number_format($present_main_ex/1000, 2); ?></b></td>
						<td style=""><b><?php echo number_format($previous_main_ex/1000, 2); ?></b></td>
						<td style=" border-right:2px solid #000;"><b><?php echo number_format(($present_main_ex-$previous_main_ex)/1000, 2); ?></b></td>
					</tr>
					<tr>
						<td style="border-bottom:2px solid #000;border-left:2px solid #000; "><b>Advance</b></td>
						<td style="border-bottom:2px solid #000;"><b><?php echo number_format(($present_main - $present_main_ex)/1000, 2); ?></b></td>
						<td style="border-bottom:2px solid #000;"><b><?php echo number_format(($previous_main - $previous_main_ex)/1000, 2); ?></b></td>
						<td style="border-bottom:2px solid #000; border-right:2px solid #000;"><b><?php echo number_format((($present_main-$previous_main) - ($present_main_ex-$previous_main_ex))/1000, 2); ?></b></td>
					</tr>
				</table>				
			
				<?php

				echo $total_advance += (($present_main-$previous_main) - ($present_main_ex-$previous_main_ex))/1000;


			}

			?>

			<p style="font-size:18px;">Total Advance = <span style="background-color:yellow; border:1px solid #000;padding:10px;"><?php echo number_format($total_advance, 2); ?></span></p> 
			</div>
			<?php
				}elseif($set_customer==2030){
					//echo '>>>>>';
					echo '<b>';
					$g = "";
					$g .= ''.strtoupper($customer_full_name.' Meter Readings').'';
					$g .= '  ';

					$g .= '(';
					if(1){
						$g .= 'KWh';
					}
					$g .= ')';

					$units = 0.001;

					$g .= ' ';
					$g .= $year = ($this->set_year)?$this->set_year:portion(4);
					$g .= ' - ';
					$g .= month_name($month = ($this->set_month)?$this->set_month:portion(5));

					echo $g;
					echo '</b>';
					$customer_id = $set_customer;
					$reading_time = strtotime(date("$year-$month-15"));

					//current main 1
					$db = new Db();
					$sql = "SELECT * FROM r_reading, r_rate, metering_point WHERE rea_date = '$reading_time' AND rea_cus_id = '$customer_id' AND rate_cus_id = '$customer_id' AND rate_reading_id = rea_id AND rea_mp_id = mp_id AND upper(mp_location) = 'MAIN 1'";
					$select = $db->select($sql);
					extract($select[0][0]);

					for($i=1; $i<=6; $i++){
						if($i<=3)
							${'cm'.$i} = ${'rate_import_wh_'.$i};
						else
							${'cm'.$i} = ${'rate_export_wh_'.$i};
					}

					// //previous main 1
					$db = new Db();
					$sql = "SELECT TOP 1 * FROM r_reading, r_rate, metering_point WHERE rea_date <  '$reading_time' AND rea_id = rate_reading_id AND rea_cus_id = '2030' AND rea_mp_id = mp_id AND rate_cus_id = '2030' AND upper(mp_location) = 'MAIN 1' ORDER BY rea_date DESC";
					$sel = $db->select($sql);

					extract($sel[0][0]);

					for($i=1; $i<=6; $i++){
						if($i<=3)
							${'pm'.$i} = ${'rate_import_wh_'.$i};
						else
							${'pm'.$i} = ${'rate_export_wh_'.$i};
					}

					for($i=1; $i<=6; $i++){
						${'am'.$i} = ${'cm'.$i} - ${'pm'.$i};
					}

					//current main 2
					$select = $db->select("SELECT * FROM r_reading, r_rate, metering_point WHERE rea_date = '$reading_time' AND rea_cus_id = '$customer_id' AND rate_cus_id = '$customer_id' AND rate_reading_id = rea_id AND rea_mp_id = mp_id AND upper(mp_location) = 'MAIN 2'");
					extract($select[0][0]);

					for($i=1; $i<=6; $i++){
						if($i<=3)
							${'cm2'.$i} = ${'rate_import_wh_'.$i};
						else
							${'cm2'.$i} = ${'rate_export_wh_'.$i};
					}

					// //previous main 2
					$sql = "SELECT TOP 1 * FROM r_reading, r_rate, metering_point WHERE rea_date < '$reading_time' AND rea_id = rate_reading_id AND rea_cus_id = '2030' AND rea_mp_id = mp_id AND rate_cus_id = '2030' AND upper(mp_location) = 'MAIN 2' ORDER BY rea_date DESC";
					$sel = $db->select($sql);

					extract($sel[0][0]);

					for($i=1; $i<=6; $i++){
						if($i<=3)
							${'pm2'.$i} = ${'rate_import_wh_'.$i};
						else
							${'pm2'.$i} = ${'rate_export_wh_'.$i};
					}

					for($i=1; $i<=6; $i++){
						${'am2'.$i} = ${'cm2'.$i} - ${'pm2'.$i};
					}

					for($i=1; $i<=6; $i++){
						${'total'.$i} = ${'am'.$i} + ${'am2'.$i};
					}

					echo '<h4>KPLC METERING (MAIN)</h4>';

					echo '<b style="color:red;">Main Meter Export</b><br/>';

					echo '<table border="1" cellpadding="2" cellspacing="0">';
					echo '<tr>';
					echo '<td rowspan="2" style="border:2px solid #000"></td>';
					echo '<td colspan="2" style="border-top:2px solid #000; border-right:2px solid #000; text-align:center; font-weight:bold;">Tariff (I)</td>';
					echo '<td colspan="2" style="border-top:2px solid #000; border-right:2px solid #000; text-align:center; font-weight:bold;">Tariff (II)</td>';
					echo '<td colspan="2" style="border-top:2px solid #000; border-right:2px solid #000; text-align:center; font-weight:bold;">Tariff (III)</td>';
					echo '</tr>';

					echo '<tr>';
					echo '<td style="border-bottom: 2px solid #000; min-width:150px; font-weight:bold; text-align:center; ">Main 1</td>';
					echo '<td style="border-bottom: 2px solid #000; min-width:150px; font-weight:bold; text-align:center; border-right:2px solid #000;">Main 2</td>';
					echo '<td style="border-bottom: 2px solid #000; min-width:150px; font-weight:bold; text-align:center; ">Main 1</td>';
					echo '<td style="border-bottom: 2px solid #000; min-width:150px; font-weight:bold; text-align:center; border-right:2px solid #000;">Main 2</td>';
					echo '<td style="border-bottom: 2px solid #000; min-width:150px; font-weight:bold; text-align:center; ">Main 1</td>';
					echo '<td style="border-bottom: 2px solid #000; min-width:150px; font-weight:bold; text-align:center; border-right:2px solid #000;">Main 2</td>';					
					echo '</tr>';

					echo '<tr>';
					echo '<td style="border-right:2px solid #000; border-left:2px solid #000; font-weight:bold;">Present&nbsp;Readings</td>';
					echo '<td align="right">'.number_format($cm4*$units).'</td>';
					echo '<td align="right" style="border-right:2px solid #000;">'.number_format($cm24*$units).'</td>';
					echo '<td align="right">'.number_format($cm5*$units).'</td>';
					echo '<td align="right" style="border-right:2px solid #000;">'.number_format($cm25*$units).'</td>';
					echo '<td align="right">'.number_format($cm6*$units).'</td>';
					echo '<td align="right" style="border-right:2px solid #000;">'.number_format($cm26*$units).'</td>';					
					echo '</tr>';

					echo '<tr>';
					echo '<td align="right" style="border-right:2px solid #000; border-left:2px solid #000; font-weight:bold;">Previous&nbsp;Readings</td>';
					echo '<td align="right">'.number_format($pm4*$units).'</td>';
					echo '<td align="right" style="border-right:2px solid #000;">'.number_format($pm24*$units).'</td>';
					echo '<td align="right">'.number_format($pm5*$units).'</td>';
					echo '<td align="right" style="border-right:2px solid #000;">'.number_format($pm25*$units).'</td>';
					echo '<td align="right">'.number_format($pm6*$units).'</td>';
					echo '<td align="right" style="border-right:2px solid #000;">'.number_format($pm26*$units).'</td>';					
					echo '</tr>';

					echo '<tr>';
					echo '<td style="border-bottom: 2px solid #000; border-right:2px solid #000; border-left:2px solid #000; font-weight:bold;">Advance</td>';
					echo '<td align="right" colspan="2" style="border-bottom: 2px solid #000; border-right:2px solid #000; font-weight:bold;">'.number_format($total4*$units).'</td>';
					echo '<td align="right" colspan="2" style="font-weight:bold; border-bottom: 2px solid #000; border-right:2px solid #000;">'.number_format($total5*$units).'</td>';
					echo '<td align="right" colspan="2" style="font-weight:bold; border-bottom: 2px solid #000; border-right:2px solid #000;">'.number_format($total6*$units).'</td>';					
					echo '</tr>';

					echo '</table>';
					//echo '<br/><br/>';

					echo '<br/><br/><b style="color:red;">Main Meter Import</b><br/>';

					echo '<table border="1" cellpadding="2" cellspacing="0">';
					echo '<tr>';
					echo '<td rowspan="2" style="border:2px solid #000"></td>';
					echo '<td colspan="2" style="border-top:2px solid #000; border-right:2px solid #000; text-align:center; font-weight:bold;">Tariff (I)</td>';
					echo '<td colspan="2" style="border-top:2px solid #000; border-right:2px solid #000; text-align:center; font-weight:bold;">Tariff (II)</td>';
					echo '<td colspan="2" style="border-top:2px solid #000; border-right:2px solid #000; text-align:center; font-weight:bold;">Tariff (III)</td>';
					echo '</tr>';

					echo '<tr>';
					echo '<td style="border-bottom: 2px solid #000; min-width:150px; font-weight:bold; text-align:center; ">Main 1</td>';
					echo '<td style="border-bottom: 2px solid #000; min-width:150px; font-weight:bold; text-align:center; border-right:2px solid #000;">Main 2</td>';
					echo '<td style="border-bottom: 2px solid #000; min-width:150px; font-weight:bold; text-align:center; ">Main 1</td>';
					echo '<td style="border-bottom: 2px solid #000; min-width:150px; font-weight:bold; text-align:center; border-right:2px solid #000;">Main 2</td>';
					echo '<td style="border-bottom: 2px solid #000; min-width:150px; font-weight:bold; text-align:center; ">Main 1</td>';
					echo '<td style="border-bottom: 2px solid #000; min-width:150px; font-weight:bold; text-align:center; border-right:2px solid #000;">Main 2</td>';					
					echo '</tr>';

					echo '<tr>';
					echo '<td align="left" style="border-right:2px solid #000; border-left:2px solid #000; font-weight:bold;">Present&nbsp;Readings</td>';
					echo '<td align="right">'.number_format($cm1*$units).'</td>';
					echo '<td align="right" style="border-right:2px solid #000;">'.number_format($cm21*$units).'</td>';
					echo '<td align="right">'.number_format($cm2*$units).'</td>';
					echo '<td align="right" style="border-right:2px solid #000;">'.number_format($cm22*$units).'</td>';
					echo '<td align="right">'.number_format($cm3*$units).'</td>';
					echo '<td align="right" style="border-right:2px solid #000;">'.number_format($cm23*$units).'</td>';					
					echo '</tr>';

					echo '<tr>';
					echo '<td align="left" style="border-right:2px solid #000; border-left:2px solid #000; font-weight:bold;">Previous&nbsp;Readings</td>';
					echo '<td align="right">'.number_format($pm1*$units).'</td>';
					echo '<td align="right" style="border-right:2px solid #000;">'.number_format($pm21*$units).'</td>';
					echo '<td align="right">'.number_format($pm2*$units).'</td>';
					echo '<td align="right" style="border-right:2px solid #000;">'.number_format($pm22*$units).'</td>';
					echo '<td align="right">'.number_format($pm3*$units).'</td>';
					echo '<td align="right" style="border-right:2px solid #000;">'.number_format($pm23*$units).'</td>';					
					echo '</tr>';

					echo '<tr>';
					echo '<td align="left" style="border-bottom: 2px solid #000; border-right:2px solid #000; border-left:2px solid #000; font-weight:bold;">Advance</td>';
					echo '<td align="right" colspan="2" style="border-bottom: 2px solid #000; border-right:2px solid #000; font-weight:bold;">'.number_format($total1*$units).'</td>';
					echo '<td align="right" colspan="2" style="font-weight:bold; border-bottom: 2px solid #000; border-right:2px solid #000;">'.number_format($total2*$units).'</td>';
					echo '<td align="right" colspan="2" style="font-weight:bold; border-bottom: 2px solid #000; border-right:2px solid #000;">'.number_format($total3*$units).'</td>';					
					echo '</tr>';

					echo '</table>';
					echo '<br/><br/>';

					////////////////////////////////////////////////

					//current main 1
					$select = $db->select("SELECT * FROM r_reading, r_rate, metering_point WHERE rea_date = '$reading_time' AND rea_cus_id = '$customer_id' AND rate_cus_id = '$customer_id' AND rate_reading_id = rea_id AND rea_mp_id = mp_id AND upper(mp_location) = 'CHECK 1'");
					extract($select[0][0]);

					for($i=1; $i<=6; $i++){
						if($i<=3)
							${'cm'.$i} = ${'rate_import_wh_'.$i};
						else
							${'cm'.$i} = ${'rate_export_wh_'.$i};
					}

					// //previous main 1
					$sql = "SELECT * FROM r_reading, r_rate, metering_point WHERE rea_date < '$reading_time' AND rea_id = rate_reading_id AND rea_cus_id = '2030' AND rea_mp_id = mp_id AND rate_cus_id = '2030' AND upper(mp_location) = 'CHECK 1' ORDER BY rea_date DESC";
					$sel = $db->select($sql);

					extract($sel[0][0]);

					for($i=1; $i<=6; $i++){
						if($i<=3)
							${'pm'.$i} = ${'rate_import_wh_'.$i};
						else
							${'pm'.$i} = ${'rate_export_wh_'.$i};
					}

					for($i=1; $i<=6; $i++){
						${'am'.$i} = ${'cm'.$i} - ${'pm'.$i};
					}

					//current main 2
					$select = $db->select("SELECT * FROM r_reading, r_rate, metering_point WHERE rea_date = '$reading_time' AND rea_cus_id = '$customer_id' AND rate_cus_id = '$customer_id' AND rate_reading_id = rea_id AND rea_mp_id = mp_id AND upper(mp_location) = 'CHECK 2'");
					extract($select[0][0]);

					for($i=1; $i<=6; $i++){
						if($i<=3)
							${'cm2'.$i} = ${'rate_import_wh_'.$i};
						else
							${'cm2'.$i} = ${'rate_export_wh_'.$i};
					}

					// //previous main 2
					$sql = "SELECT * FROM r_reading, r_rate, metering_point WHERE rea_date < '$reading_time' AND rea_id = rate_reading_id AND rea_cus_id = '2030' AND rea_mp_id = mp_id AND rate_cus_id = '2030' AND upper(mp_location) = 'CHECK 2' ORDER BY rea_date DESC";
					$sel = $db->select($sql);

					extract($sel[0][0]);

					for($i=1; $i<=6; $i++){
						if($i<=3)
							${'pm2'.$i} = ${'rate_import_wh_'.$i};
						else
							${'pm2'.$i} = ${'rate_export_wh_'.$i};
					}

					for($i=1; $i<=6; $i++){
						${'am2'.$i} = ${'cm2'.$i} - ${'pm2'.$i};
					}

					for($i=1; $i<=6; $i++){
						${'total'.$i} = ${'am'.$i} + ${'am2'.$i};
					}


					echo '<h4>KPLC METERING (CHECK)</h4>';
					echo '<b style="color:red;">Check Meter Export</b><br/>';

					echo '<table border="1" cellpadding="2" cellspacing="0">';
					echo '<tr>';
					echo '<td rowspan="2" style="border:2px solid #000"></td>';
					echo '<td colspan="2" style="border-top:2px solid #000; border-right:2px solid #000; text-align:center; font-weight:bold;">Tariff (I)</td>';
					echo '<td colspan="2" style="border-top:2px solid #000; border-right:2px solid #000; text-align:center; font-weight:bold;">Tariff (II)</td>';
					echo '<td colspan="2" style="border-top:2px solid #000; border-right:2px solid #000; text-align:center; font-weight:bold;">Tariff (III)</td>';
					echo '</tr>';

					echo '<tr>';
					echo '<td style="border-bottom: 2px solid #000; min-width:150px; font-weight:bold; text-align:center; ">Check 1</td>';
					echo '<td style="border-bottom: 2px solid #000; min-width:150px; font-weight:bold; text-align:center; border-right:2px solid #000;">Check 2</td>';
					echo '<td style="border-bottom: 2px solid #000; min-width:150px; font-weight:bold; text-align:center; ">Check 1</td>';
					echo '<td style="border-bottom: 2px solid #000; min-width:150px; font-weight:bold; text-align:center; border-right:2px solid #000;">Check 2</td>';
					echo '<td style="border-bottom: 2px solid #000; min-width:150px; font-weight:bold; text-align:center; ">Check 1</td>';
					echo '<td style="border-bottom: 2px solid #000; min-width:150px; font-weight:bold; text-align:center; border-right:2px solid #000;">Check 2</td>';					
					echo '</tr>';

					echo '<tr>';
					echo '<td style="border-right:2px solid #000; border-left:2px solid #000; font-weight:bold;">Present Readings</td>';
					echo '<td align="right">'.number_format($cm4*$units).'</td>';
					echo '<td align="right" style="border-right:2px solid #000;">'.number_format($cm24*$units).'</td>';
					echo '<td align="right">'.number_format($cm5*$units).'</td>';
					echo '<td align="right" style="border-right:2px solid #000;">'.number_format($cm25*$units).'</td>';
					echo '<td align="right">'.number_format($cm6*$units).'</td>';
					echo '<td align="right" style="border-right:2px solid #000;">'.number_format($cm26*$units).'</td>';					
					echo '</tr>';

					echo '<tr>';
					echo '<td align="right" style="border-right:2px solid #000; border-left:2px solid #000; font-weight:bold;">Previous Readings</td>';
					echo '<td align="right">'.number_format($pm4*$units).'</td>';
					echo '<td align="right" style="border-right:2px solid #000;">'.number_format($pm24*$units).'</td>';
					echo '<td align="right">'.number_format($pm5*$units).'</td>';
					echo '<td align="right" style="border-right:2px solid #000;">'.number_format($pm25*$units).'</td>';
					echo '<td align="right">'.number_format($pm6*$units).'</td>';
					echo '<td align="right" style="border-right:2px solid #000;">'.number_format($pm26*$units).'</td>';					
					echo '</tr>';

					echo '<tr>';
					echo '<td style="border-bottom: 2px solid #000; border-right:2px solid #000; border-left:2px solid #000; font-weight:bold;">Advance</td>';
					echo '<td align="right" colspan="2" style="border-bottom: 2px solid #000; border-right:2px solid #000; font-weight:bold;">'.number_format($total4*$units).'</td>';
					echo '<td align="right" colspan="2" style="font-weight:bold; border-bottom: 2px solid #000; border-right:2px solid #000;">'.number_format($total5*$units).'</td>';
					echo '<td align="right" colspan="2" style="font-weight:bold; border-bottom: 2px solid #000; border-right:2px solid #000;">'.number_format($total6*$units).'</td>';					
					echo '</tr>';

					echo '</table>';
					//echo '<br/><br/>';

					echo '<br/><br/><b style="color:red;">Check Meter Import</b><br/>';

					echo '<table border="1" cellpadding="2" cellspacing="0">';
					echo '<tr>';
					echo '<td rowspan="2" style="border:2px solid #000"></td>';
					echo '<td colspan="2" style="border-top:2px solid #000; border-right:2px solid #000; text-align:center; font-weight:bold;">Tariff (I)</td>';
					echo '<td colspan="2" style="border-top:2px solid #000; border-right:2px solid #000; text-align:center; font-weight:bold;">Tariff (II)</td>';
					echo '<td colspan="2" style="border-top:2px solid #000; border-right:2px solid #000; text-align:center; font-weight:bold;">Tariff (III)</td>';
					echo '</tr>';

					echo '<tr>';
					echo '<td style="border-bottom: 2px solid #000; min-width:150px; font-weight:bold; text-align:center; ">Check 1</td>';
					echo '<td style="border-bottom: 2px solid #000; min-width:150px; font-weight:bold; text-align:center; border-right:2px solid #000;">Check 2</td>';
					echo '<td style="border-bottom: 2px solid #000; min-width:150px; font-weight:bold; text-align:center; ">Check 1</td>';
					echo '<td style="border-bottom: 2px solid #000; min-width:150px; font-weight:bold; text-align:center; border-right:2px solid #000;">Check 2</td>';
					echo '<td style="border-bottom: 2px solid #000; min-width:150px; font-weight:bold; text-align:center; ">Check 1</td>';
					echo '<td style="border-bottom: 2px solid #000; min-width:150px; font-weight:bold; text-align:center; border-right:2px solid #000;">Check 2</td>';					
					echo '</tr>';

					echo '<tr>';
					echo '<td align="left" style="border-right:2px solid #000; border-left:2px solid #000; font-weight:bold;">Present Readings</td>';
					echo '<td align="right">'.number_format($cm1*$units).'</td>';
					echo '<td align="right" style="border-right:2px solid #000;">'.number_format($cm21*$units).'</td>';
					echo '<td align="right">'.number_format($cm2*$units).'</td>';
					echo '<td align="right" style="border-right:2px solid #000;">'.number_format($cm22*$units).'</td>';
					echo '<td align="right">'.number_format($cm3*$units).'</td>';
					echo '<td align="right" style="border-right:2px solid #000;">'.number_format($cm23*$units).'</td>';					
					echo '</tr>';

					echo '<tr>';
					echo '<td align="left" style="border-right:2px solid #000; border-left:2px solid #000; font-weight:bold;">Previous Readings</td>';
					echo '<td align="right">'.number_format($pm1*$units).'</td>';
					echo '<td align="right" style="border-right:2px solid #000;">'.number_format($pm21*$units).'</td>';
					echo '<td align="right">'.number_format($pm2*$units).'</td>';
					echo '<td align="right" style="border-right:2px solid #000;">'.number_format($pm22*$units).'</td>';
					echo '<td align="right">'.number_format($pm3*$units).'</td>';
					echo '<td align="right" style="border-right:2px solid #000;">'.number_format($pm23*$units).'</td>';					
					echo '</tr>';

					echo '<tr>';
					echo '<td align="left" style="border-bottom: 2px solid #000; border-right:2px solid #000; border-left:2px solid #000; font-weight:bold;">Advance</td>';
					echo '<td align="right" colspan="2" style="border-bottom: 2px solid #000; border-right:2px solid #000; font-weight:bold;">'.number_format($total1*$units).'</td>';
					echo '<td align="right" colspan="2" style="font-weight:bold; border-bottom: 2px solid #000; border-right:2px solid #000;">'.number_format($total2*$units).'</td>';
					echo '<td align="right" colspan="2" style="font-weight:bold; border-bottom: 2px solid #000; border-right:2px solid #000;">'.number_format($total3*$units).'</td>';					
					echo '</tr>';

					echo '</table>';
					echo '<br/><br/>';


					////////////////////////////////////////////////
				}else{
			?>

			<table id="sortSpan" border="1" cellspacing="0" cellpadding="2" class="table schedule" style="font-size: 12px;font-family: arial; background-color: #fff">
				<thead>
				<tr class="tablesorter-ignoreRow" valign="bottom">
					<?php if($customer_id != 2019){ ?>
					<th colspan="22" style="border:2px solid black;">
					<?php }else{ ?>
					<th colspan="12" style="border:2px solid black;">
					<?php } ?>
					<?php
					$g = "";
					$g .= ''.strtoupper($customer_full_name.' Meter Readings').'';
					$g .= '  ';

					$g .= '(';
					if($units == 1){
						$g .= 'Wh';
					}elseif($units == 0.001){
						$g .= 'KWh';
					}elseif($units == 0.000001){
						$g .= 'MWh';
					}
					$g .= ')';

					$g .= ' ';
					$g .= $year = ($this->set_year)?$this->set_year:portion(4);
					$g .= ' - ';
					$g .= month_name($month = ($this->set_month)?$this->set_month:portion(5));

					echo $g;

					?>

					
					
					</th>				
				</tr>
				<tr valign="bottom">
					<th rowspan="3" style="border:2px solid black;" class="sorter-false">No.</th>
					<th rowspan="3" style="border:2px solid black;" class="">Metering Points</th>
					<th colspan="3" style="border:2px solid black;" class="sorter-false">Main Meter - Current Readings 
					
					</th>					
					<th colspan="3" style="border:2px solid black;color:red;" class="sorter-false">Main Meter - Previous Readings </th>
					<?php if($customer_id != 2019 && $customer_id != 2036){ ?>
					<th colspan="3" style="border:2px solid black;" class="sorter-false">Main Meter - Current Readings</th>
					<th colspan="3" style="border:2px solid black;color:red;" class="sorter-false">Main Meter - Previous Readings</th>
					<?php } ?>										
					<?php if($customer_id != 2019 && $customer_id != 2036){ ?>
					<th style="border:2px solid black;" colspan="8" style="border:2px solid black;" class="sorter-false">Advance</th>
					<?php } else { ?>
					<th style="border:2px solid black;" colspan="4" style="border:2px solid black;" class="sorter-false">Advance</th>
					<?php } ?>

				</tr>
				<tr class="tablesorter-ignoreRow">
					<th style="border:2px solid black;" colspan="3">Imports</th>
					<th style="border:2px solid black; color:red;" colspan="3">Imports</th>
					<?php if($customer_id != 2019 && $customer_id != 2036){ ?>
					<th style="border:2px solid black;" colspan="3">Exports</th>
					<th style="border:2px solid black; color:red;" colspan="3">Exports</th>
					<?php } ?>
					<th style="border:2px solid black;" colspan="4">Imports</th>
					<?php if($customer_id != 2019 && $customer_id != 2036){ ?>
					<th style="border:2px solid black;" colspan="4">Exports</th>
					<?php } ?>				
				</tr>
				<tr class="tablesorter-ignoreRow">
					<th style="border-left:2px solid black;border-bottom:2px solid black;">Shoulder</th>
					<th style="border-bottom:2px solid black;">Peak</th>
					<th style="border-bottom:2px solid black;">Off Peak</th>

					<th style="border-left:2px solid black;border-bottom:2px solid black;color:red;">Shoulder</th>
					<th style="border-bottom:2px solid black;color:red;">Peak</th>
					<th style="border-bottom:2px solid black;color:red;">Off Peak</th>

					<?php if($customer_id != 2019 && $customer_id != 2036){ ?>
					<th style="border-left:2px solid black;border-bottom:2px solid black;">Shoulder</th>
					<th style="border-bottom:2px solid black;">Peak</th>
					<th style="border-bottom:2px solid black;">Off Peak</th>
					
					
					<th style="border-left:2px solid black;border-bottom:2px solid black;color:red;">Shoulder</th>
					<th style="border-bottom:2px solid black;color:red;">Peak</th>
					<th style="border-bottom:2px solid black;color:red;">Off Peak</th>

					<?php } ?>
					<th style="border-left:2px solid black;border-bottom:2px solid black;">Shoulder</th>
					<th style="border-bottom:2px solid black;">Peak</th>
					<th style="border-bottom:2px solid black;">Off Peak</th>
					<th style="border-right:2px solid black;border-bottom:2px solid black;">Total</th>
					<?php if($customer_id != 2019 && $customer_id != 2036){ ?>
					<th style="border-left:2px solid black;border-bottom:2px solid black;">Shoulder</th>
					<th style="border-bottom:2px solid black;">Peak</th>
					<th style="border-bottom:2px solid black;">Off Peak</th>
					<th style="border-right:2px solid black;border-bottom:2px solid black;">Total</th>	
					<?php } ?>			
				</tr>
				<?php

				////////////////////REPORT STEP 1////////////////////////
				$db_values = array();
				$db_values[] = array("No.", "Metering Point","Shoulder","Peak", "Off Peak","Shoulder","Peak", "Off Peak","Shoulder","Peak", "Off Peak","Shoulder","Peak", "Off Peak", "Total","Shoulder","Peak", "Off Peak", "Total");
				///////////////////////////////////////////

				$count = 1;
				$db = new Db();

				$reading_time = strtotime(date("$year-$month-15"));

				$select = $db->select("SELECT * FROM metering_point, r_reading WHERE mp_customer_id = '$customer_id' AND rea_cus_id = '$customer_id' AND mp_id = rea_mp_id AND rea_date = '$reading_time' ORDER BY mp_location ASC");

				$total_mp = 0;
				foreach($select[0] as $row){
					extract($row);
					//if(date('Y', $rea_date)==$year && date('m', $rea_date)==$month)
					{
						$total_mp++;
					}
				}
				//$units = 0.001;

				$tc1 = $tc2 = $tc3 = $tc4= $tc5= $tc6= $tc7= $tc8 = 0;

				$co = 0;
				$t = new Db();
				$sel = $t->select("SELECT DISTINCT mp_region_id FROM metering_point, r_reading WHERE mp_customer_id = '$customer_id' AND rea_cus_id = '$customer_id' AND mp_id = rea_mp_id AND mp_region_id IS NOT NULL ORDER BY mp_location ASC");

				$n = 1;
								
				$sel = $t->select("SELECT * FROM meter_region ORDER BY region_order ASC");
				
				$stc1 = $stc2 = $stc3 = $stc4 = $stc5 = $stc6 = $stc7 = $stc8 = 0;
				foreach($sel[0] as $r){ //start regions
				extract($r);
				$tc1 = $tc2 = $tc3 = $tc4= $tc5= $tc6= $tc7= $tc8 = 0;
				if((!$this->isThere("metering_point", ["mp_region_id"=>$region_id]))){
					continue;
				}else{
					$mp_region_id = $region_id;
				}

				$total_mp;
				$db = new Db();

				if($customer_id == 2019|| $customer_id == 2036){
					$select = $db->select("SELECT rea_date, mp_id, rea_id,mp_location FROM metering_point, r_reading WHERE mp_customer_id = '$customer_id' AND rea_cus_id = '$customer_id' AND mp_id = rea_mp_id AND mp_region_id = '$mp_region_id' AND rea_date = '$reading_time' ORDER BY mp_location ASC");
				}else{
					$select = $db->select("SELECT rea_date, mp_id, rea_id,mp_location FROM metering_point, r_reading WHERE mp_customer_id = '$customer_id' AND rea_cus_id = '$customer_id' AND mp_id = rea_mp_id AND mp_region_id = '$mp_region_id' AND rea_date = '$reading_time' ORDER BY mp_location ASC");
				}

				echo '</thead>';
				echo '<tbody>';

				if($customer_id != 2019 && $customer_id != 2036){
					// echo '<tr>
					// <td colspan="19" style="border-left:none; border-right:none; border-top:2px solid #000; border-bottom: 2px solid #000;color:red;"><b>'.strtoupper($this->rgf("meter_region", $mp_region_id, "region_id", "region_name")).'</b></td>
					// </tr>';
				}else{
					echo '<tr>
					<td colspan="12" style="border-left:none; border-right:none; border-top:2px solid #000; border-bottom: 2px solid #000;color:red;"><b>'.strtoupper($this->rgf("meter_region", $mp_region_id, "region_id", "region_name")).'</b></td>
					</tr>';
				}
					$xl=0;
				$db_values[] = array('', '&nbsp;<br/><b>'.strtoupper($this->rgf("meter_region", $mp_region_id, "region_id", "region_name")).'</b>');
				foreach($select[0] as $row){

					extract($row);
					$j++;
					
					if(date('Y', $rea_date)==$set_year && date('m', $rea_date)==$set_month){
						//selecting rates
						
						$db = new Db();
						$sql = "SELECT rate_lfi, rate_import_wh_1,rate_import_wh_2,rate_import_wh_3,rate_export_wh_4,rate_export_wh_5,rate_export_wh_6,tisu, tipu, tiou, tiu, tesu, tepu, teou, teu FROM r_rate WHERE rate_reading_id = '$rea_id'";
						$sel = $db->select($sql);
						@extract($sel[0][0]);
					
						//calling the function
						$cal = rate_calculate($customer_id, ($rea_date), $mp_id);


						$db_values[] = array(
							($n),
							$mp_location,
							number_format(@$rate_import_wh_1*$units,2),
							number_format(@$rate_import_wh_2*$units,2),
							number_format(@$rate_import_wh_3*$units,2),

							number_format(@$cal[12]*$units,2),
							number_format(@$cal[13]*$units,2),
							number_format(@$cal[14]*$units,2),

							number_format(@$rate_export_wh_4*$units,2),
							number_format(@$rate_export_wh_5*$units,2),
							number_format(@$rate_export_wh_6*$units,2),

							number_format($cal[0]*$units,2),
							number_format($cal[1]*$units,2),
							number_format($cal[2]*$units,2),
							number_format($cal[3]*$units,2),
							number_format($cal[4]*$units,2),
							number_format($cal[5]*$units,2),
							number_format($cal[6]*$units,2),
							number_format($cal[7]*$units,2)
							
						);

						if($co%2 == 0){
							$df = "";
						}else{
							$df = " background-color: #e6e6e6; !important;";

						}
						$co++;

						
						if(1){
							echo '<tr>';
							echo '<td style="'.$df.'border-left:2px solid black;">'.($n++).'.</td>';
							echo '<td style="'.$df.'border-left:2px solid black; border-right:2px solid black;">'.($mp_location).'<span id="rt">'.$this->mpAdvance($mp_advance).'</span>';

							// if(!$this->isLocked($customer_id, $year, $month)){
							// echo '<div class="add-hint" title="Edit '.$mp_location.' Readings" id="add-hint'.$n.'" style="color:white;"><i class="fa fa-fw fa-pencil"></i></div>';
							// }


							echo '</td>';

							echo '<td style="'.$df.' text-align:right;">'.number_format(@$rate_import_wh_1*$units,2).'</td>';
							echo '<td style="'.$df.' text-align:right;">'.number_format(@$rate_import_wh_2*$units,2).'</td>';
							echo '<td style="'.$df.'border-right:2px solid black; text-align:right;">'.number_format(@$rate_import_wh_3*$units,2).'</td>';


							echo '<td style="'.$df.' text-align:right;color:red;">'.number_format(@$cal[12]*$units,2).'</td>';
							echo '<td style="'.$df.' text-align:right;color:red;">'.number_format(@$cal[13]*$units,2).'</td>';
							echo '<td style="'.$df.'border-right:2px solid black; text-align:right;color:red;">'.number_format(@$cal[14]*$units,2).'</td>';

							if($customer_id != 2019 && $customer_id != 2036){
								echo '<td style="'.$df.' text-align:right;">'.number_format(@$rate_export_wh_4*$units,2).'</td>';
								echo '<td style="'.$df.' text-align:right;">'.number_format(@$rate_export_wh_5*$units,2).'</td>';
								echo '<td style="'.$df.' text-align:right;">'.number_format(@$rate_export_wh_6*$units,2).'</td>';	
							
								echo '<td style="'.$df.'border-left:2px solid black; text-align:right;color:red;">'.number_format(@$cal[15]*$units,2).'</td>';
								echo '<td style="'.$df.' text-align:right;color:red;">'.number_format(@$cal[16]*$units,2).'</td>';
								echo '<td style="'.$df.'border-right:2px solid black; text-align:right;color:red;">'.number_format(@$cal[17]*$units,2).'</td>';
							
							}

							echo '<td style="'.$df.'border-left:2px solid black; text-align:right;">';
							echo number_format($tisu*$units,2);
							//if(!$this->isLocked($customer_id, $year, $month))
							// if(1){//898989
							// echo '<div class="add-hint formula-space" style="color:white;"><i class="fa fa-fw fa-calculator"></i>
							// 	<div class="formula-message">
							// 		<div class="area">';
							// 			echo $rate_lfi;
							// 		//$this->formulaDisplay($customer_id, $mp_id, $year, $month);
							// 		echo '</div></div>
							// </div>';
							// }
							echo '</td>';
							echo '<td style="'.$df.' text-align:right;">'.number_format($tipu*$units,2).'</td>';
							echo '<td style="'.$df.' text-align:right;">'.number_format($tiou*$units,2).'</td>';
							echo '<td style="'.$df.'border-right:2px solid black; text-align:right;">'.number_format($tiu*$units,2).'</td>';

							if($customer_id != 2019 && $customer_id != 2036){
								echo '<td style="'.$df.'border-left:2px solid black; text-align:right;">'.number_format($tepu*$units,2).'</td>';
								echo '<td style="'.$df.' text-align:right;">'.number_format($tepu*$units,2).'</td>';
								echo '<td style="'.$df.' text-align:right;">'.number_format($teou*$units,2).'</td>';
								echo '<td style="'.$df.'border-right:2px solid black; text-align:right;">'.number_format($teu*$units,2).'</td>';	
							}			
							
							echo '</tr>';

							$tc1 += $tisu;
							$tc2 += $tipu;
							$tc3 += $tiou;
							$tc4 += $tiu;
							$tc5 += $tesu;
							$tc6 += $tepu;
							$tc7 += $teou;
							$tc8 += $teu;

							$stc1 += $tisu;
							$stc2 += $tipu;
							$stc3 += $tiou;
							$stc4 += $tiu;
							$stc5 += $tesu;
							$stc6 += $tepu;
							$stc7 += $teou;
							$stc8 += $teu;
						}else{			

							echo '<tr>';

							echo '<td style="'.$df.'border-left:2px solid black;">'.($n++).'.</td>';
							echo '<td style="'.$df.'border-left:2px solid black; border-bottom:2px solid black; ">'.$this->noSpace($mp_location).'</td>';


							echo '<td style="'.$df.'border-left:2px solid black; border-bottom:2px solid black;  text-align:right;">'.number_format(@$rate_import_wh_1*$units,2).'</td>';
							echo '<td style="'.$df.'border-bottom:2px solid black;  text-align:right;">'.number_format(@$rate_import_wh_2*$units,2).'</td>';
							echo '<td style="'.$df.'border-bottom:2px solid black;  text-align:right;">'.number_format(@$rate_import_wh_3*$units,2).'</td>';



							echo '<td style="'.$df.'border-left:2px solid black; border-bottom:2px solid black;  text-align:right;color:red;">'.number_format(@$cal[12]*$units,2).'</td>';
							echo '<td style="'.$df.'border-bottom:2px solid black;  text-align:right;color:red;">'.number_format(@$cal[13]*$units,2).'</td>';
							echo '<td style="'.$df.'border-bottom:2px solid black;  text-align:right;color:red;">'.number_format(@$cal[14]*$units,2).'</td>';


							if($customer_id != 2019 && $customer_id != 2036){
								echo '<td style="'.$df.'border-left:2px solid black; border-bottom:2px solid black;  text-align:right;">'.number_format(@$rate_export_wh_4*$units,2).'</td>';
								echo '<td style="'.$df.'border-bottom:2px solid black;  text-align:right;">'.number_format(@$rate_export_wh_5*$units,2).'</td>';
								echo '<td style="'.$df.'border-bottom:2px solid black; text-align:right;">'.number_format(@$rate_export_wh_6*$units,2).'</td>';
							}

							echo '<td style="'.$df.'border-left:2px solid black; border-bottom:2px solid black;  text-align:right;">'.number_format($tisu*$units,2).'</td>';
							echo '<td style="'.$df.'border-bottom:2px solid black;  text-align:right;">'.number_format($tipu*$units,2).'</td>';
							echo '<td style="'.$df.'border-bottom:2px solid black;  text-align:right;">'.number_format($tiou*$units,2).'</td>';
							echo '<td style="'.$df.'border-right:2px solid black; border-bottom:2px solid black; text-align:right; ">'.number_format($tiu*$units,2).'</td>';

							if($customer_id != 2019 && $customer_id != 2036){
								echo '<td style="'.$df.'border-left:2px solid black; border-bottom:2px solid black;  text-align:right;">'.number_format($tesu*$units).'</td>';
								echo '<td style="'.$df.'border-bottom:2px solid black;  text-align:right;">'.number_format($tepu*$units).'</td>';
								echo '<td style="'.$df.'border-bottom:2px solid black;  text-align:right;">'.number_format($teou*$units).'</td>';
								echo '<td style="'.$df.'border-right:2px solid black; border-bottom:2px solid black; text-align:right; ">'.number_format($teu*$units).'</td>';
							}

							echo '</tr>';

							$tc1 += $tisu;
							$tc2 += $tipu;
							$tc3 += $tiou;
							$tc4 += $tiu;
							$tc5 += $tesu;
							$tc6 += $tepu;
							$tc7 += $teou;
							$tc8 += $teu;

							$stc1 += $tisu;
							$stc2 += $tipu;
							$stc3 += $tiou;
							$stc4 += $tiu;
							$stc5 += $tesu;
							$stc6 += $tepu;
							$stc7 += $teou;
							$stc8 += $teu;
						}


						$count++;	

					}	
								
				}

				//if($count == $total_mp+1)
				{
					//$l = $this->sumFormulae($customer_id, $year, $month);

					//$tc1 = $l[0];
					//$tc2 = $l[1];
					//$tc3 = $l[2];
					//$tc4 = $l[3];
					//$tc5 = $l[4];
					//$tc6 = $l[5];
					//$tc7 = $l[6];
					//$tc8 = $l[7];
					if($customer_id == 2019 || $customer_id == 2036){
					echo '<tr>';
					echo '<td style="border-top:2px solid black; border-top:2px solid #000;border-bottom:2px solid #000;border-left:2px solid #000;"></td>';

					if($customer_id != 2019 && $customer_id != 2036){
						echo '<td colspan="13" style="border-top:2px solid black; border-left:2px solid black; border-bottom:2px solid black; border-right:2px solid black; text-align:right;font-weight: bold; ">Total</td>';
					}else{
						echo '<td colspan="7" style="border-top:2px solid black; border-left:2px solid black; border-bottom:2px solid black; border-right:2px solid black; text-align:right;font-weight: bold; ">Total</td>';
					}
					
					echo '<td style="border-top:2px solid black; border-bottom:2px solid black;  text-align:right;font-weight: bold;">'.number_format($tc1*$units,2).'</td>';
					echo '<td style="border-top:2px solid black; border-bottom:2px solid black;  text-align:right;font-weight: bold;">'.number_format($tc2*$units,2).'</td>';
					echo '<td style="border-top:2px solid black; border-bottom:2px solid black;  text-align:right;font-weight: bold;">'.number_format($tc3*$units,2).'</td>';

					echo '<td style="border-top:2px solid black; border-right:2px solid black; border-bottom:2px solid black; text-align:right; font-weight: bold; ">'.number_format($tc4*$units,2).'</td>';

					if($customer_id != 2019 && $customer_id != 2036){
						echo '<td style="border-top:2px solid black; border-left:2px solid black; border-bottom:2px solid black;  text-align:right;">'.number_format($tc5*$units,2).'</td>';
						echo '<td style="border-top:2px solid black; border-bottom:2px solid black;  text-align:right;">'.number_format($tc6*$units,2).'</td>';
						echo '<td style="border-top:2px solid black; border-bottom:2px solid black;  text-align:right;">'.number_format($tc7*$units,2).'</td>';
						echo '<td style="border-top:2px solid black; border-right:2px solid black; border-bottom:2px solid black; text-align:right; ">'.number_format($tc8*$units,2).'</td>';
					}

					echo '</tr>';
					}
				}

				}
				if(1){

					$l = array(); //$this->sumFormulae($customer_id, $year, $month);

					$tc1 = $stc1;
					$tc2 = $stc2;
					$tc3 = $stc3;
					$tc4 = $stc4;
					$tc5 = $stc5;
					$tc6 = $stc6;
					$tc7 = $stc7;
					$tc8 = $stc8;
					echo '<tr>';
					echo '<td style="border-top:2px solid black; border-top:2px solid #000;border-bottom:2px solid #000;border-left:2px solid #000;"></td>';

					if($customer_id != 2019 && $customer_id != 2036){
						echo '<td colspan="13" style="border-top:2px solid black; border-left:2px solid black; border-bottom:2px solid black; border-right:2px solid black; text-align:right;font-weight: bold; ">Total</td>';
					}else{
						echo '<td colspan="7" style="border-top:2px solid black; border-left:2px solid black; border-bottom:2px solid black; border-right:2px solid black; text-align:right;font-weight: bold; ">Total</td>';
					}
					
					echo '<td style="border-top:2px solid black; border-bottom:2px solid black;  text-align:right;font-weight: bold;">'.number_format($tc1*$units,2).'</td>';
					echo '<td style="border-top:2px solid black; border-bottom:2px solid black;  text-align:right;font-weight: bold;">'.number_format($tc2*$units,2).'</td>';
					echo '<td style="border-top:2px solid black; border-bottom:2px solid black;  text-align:right;font-weight: bold;">'.number_format($tc3*$units,2).'</td>';

					echo '<td style="border-top:2px solid black; border-right:2px solid black; border-bottom:2px solid black; text-align:right; font-weight: bold; ">'.number_format($tc4*$units,2).'</td>';

					if($customer_id != 2019 && $customer_id != 2036){
						echo '<td style="border-top:2px solid black; border-left:2px solid black; border-bottom:2px solid black;  text-align:right;">'.number_format($tc5*$units,2).'</td>';
						echo '<td style="border-top:2px solid black; border-bottom:2px solid black;  text-align:right;">'.number_format($tc6*$units,2).'</td>';
						echo '<td style="border-top:2px solid black; border-bottom:2px solid black;  text-align:right;">'.number_format($tc7*$units,2).'</td>';
						echo '<td style="border-top:2px solid black; border-right:2px solid black; border-bottom:2px solid black; text-align:right; ">'.number_format($tc8*$units,2).'</td>';
					}

					echo '</tr>';
					}

				
				?>
			</tbody></table>
		<?php } ?>
		<?php 
		$d = new Db();
		$select=$d->select("SELECT * FROM witness WHERE witness_customer_id = '$customer_id' AND witness_year = '$year' AND witness_month = '$month' ");
		extract($select[0][0]);

		if(!empty($witness_firstname1) || !empty($witness_firstname2) || !empty($witness_firstname3) || !empty($witness_othername1) || !empty($witness_othername2) || !empty($witness_othername3)){
			echo '<h4>Witnesses</h4>';
			echo '<table style="border:2px solid #000;" border="1" cellspacing="0" cellpadding="2">';	
			echo '<tr>';
			echo '<td align="center">No.</td>';
			echo '<td style="min-width:150px; font-weight:bold;" align="center">Name</td>';
			echo '<td style="min-width:150px; font-weight:bold;" align="center" colspan="2">Date</td>';
			echo '</tr>';	

			for($i=1; $i<=3; $i++){
				echo '<tr>';
				echo '<td align="center">'.$i.'.</td>';
				echo '<td align="left">'.ucwords(${'witness_firstname'.$i}).' '.ucwords(${'witness_othername'.$i}).'</td>';
				echo '<td align="center">'.(${'witness_date'.$i}).'</td>';
				echo '<td align="center">'.$this->te(${'witness_time'.$i}).'</td>';
				echo '</tr>';
			}

			echo '</table>';
		}
		?>
			<?php $this->showReadingComments(); ?>
		</div>
			<?php 

			$this->addReadingComment(); 

			$t = new TableCreatorSchedule();
			$heading = "$g";
			$t->open($this->full_name(user_id()), $heading);
			//$t->title($customer_short_name.' '.$year.' '.$month);
			$t->thd($db_values);
			$t->close();
			$t->results();

			$e = new Exporter();
			echo $e->getDisplay($heading, $t->results());
			
			?>
	
	</div>
	<?php

	}//closing what is not tanseco
	}

	public function te($time){
		if(!empty($time)){
			$t = explode(':', $time);
			//echo $time;
			if($t[0] <= 12){
				return $time.' AM';
			}else{
				return str_pad(($t[0]-12), 2, '0', STR_PAD_LEFT).':'.$t[1].' PM';
			}
		}
	}
	
	public function ScheduleCreditNote($id, $year, $month){

		$class = ($this->set_class)?$this->set_class:portion(1);
		$method = ($this->set_method)?$this->set_method:portion(2);
		$id = ($this->set_customer)?$this->set_customer:portion(3);
		$year = ($this->set_year)?$this->set_year: portion(4);
		$month = ($this->set_month)?$this->set_month: portion(5);
		$note = ($this->set_note)?$this->set_note: portion(6);


		$db = new Db();
		$select = $db->select("SELECT * FROM customer WHERE customer_id = '$id'");
		extract($select[0][0]);
	
	?>
	<div id="side">
		<?php 
		$units = $this->customerAndSchedule($id, $year, $month);
		$this->energyLinks($id, $year, $month);

		?>

		<div id="printMe">
			<style>table{border:none;}table tr td, table tr th{ height: 10px !important;} table tr th{ text-align: center;} <?php if($id == 2020) { echo '@media print{@page {size: landscape}}'; } ?> </style>
			<?php 

			echo '<title>'.strtoupper($customer_short_name.' Meter Readings').' - '.($month = $month).' '.$year.'</title>';
			?>
			<center class="hide"><h3>UGANDA ELECTRICITY TRANSMISSION COMPANY LTD</h3></center>
			<table border="1" cellspacing="0" cellpadding="2" class="table schedule" style="font-size: 12px;font-family: arial;">
				<thead>
				<tr valign="bottom">
					<?php if($customer_id != 2019){ ?>
					<th colspan="19" style="border:2px solid black;">
					<?php }else{ ?>
					<th colspan="12" style="border:2px solid black;">
					<?php } ?>
					<?php
					if($this->isDNorCN($id, $year, $month) > 0)
						$not = 'DEBIT NOTE';
					else
						$not = 'CREDIT NOTE';

					$g = "";
					$g .= ''.strtoupper($customer_full_name.' Meter Readings').'';
					$g .= '  ';

					$g .= '(';
					if($units == 1){
						$g .= 'Wh';
					}elseif($units == 0.001){
						$g .= 'KWh';
					}elseif($units == 0.000001){
						$g .= 'MWh';
					}
					$g .= ')';

					$g .= ' ';
					$g .= $year = portion(4);
					$g .= ' - ';
					$g .= month_name($month);

					echo $g;
					$g = "$not ".$g;
					?>

					
					
					</th>				
				</tr>
				<tr valign="bottom">
					<th rowspan="3" style="border:2px solid black;">No.</th>
					<th rowspan="3" style="border:2px solid black;">Metering Points</th>
					<th colspan="3" style="border:2px solid black;">Main Meter - Current Readings 
					
					</th>					
					<th colspan="3" style="border:2px solid black;color:red;">Main Meter - Previous Readings</th>
					<?php if($customer_id != 2019){ ?>
					<th colspan="3" style="border:2px solid black;">Main Meter - Current Readings</th>
					<?php } ?>
					<?php if($customer_id != 2019){ ?>
					<th style="border:2px solid black;" colspan="8" style="border:2px solid black;">Advance</th>
					<?php } else { ?>
					<th style="border:2px solid black;" colspan="4" style="border:2px solid black;">Advance</th>
					<?php } ?>

				</tr>
				<tr>
					<th style="border:2px solid black;" colspan="3">Imports</th>
					<th style="border:2px solid black; color:red;" colspan="3">Imports</th>
					<?php if($customer_id != 2019){ ?>
					<th style="border:2px solid black;" colspan="3">Exports</th>
					<?php } ?>
					<th style="border:2px solid black;" colspan="4">Imports</th>
					<?php if($customer_id != 2019){ ?>
					<th style="border:2px solid black;" colspan="4">Exports</th>
					<?php } ?>				
				</tr>
				<tr>
					<th style="border-left:2px solid black;border-bottom:2px solid black;">Shoulder</th>
					<th style="border-bottom:2px solid black;">Peak</th>
					<th style="border-bottom:2px solid black;">Off Peak</th>

					<th style="border-left:2px solid black;border-bottom:2px solid black;color:red;">Shoulder</th>
					<th style="border-bottom:2px solid black;color:red;">Peak</th>
					<th style="border-bottom:2px solid black;color:red;">Off Peak</th>

					<?php if($customer_id != 2019){ ?>
					<th style="border-left:2px solid black;border-bottom:2px solid black;">Shoulder</th>
					<th style="border-bottom:2px solid black;">Peak</th>
					<th style="border-bottom:2px solid black;">Off Peak</th>
					<?php } ?>

					<th style="border-left:2px solid black;border-bottom:2px solid black;">Shoulder</th>
					<th style="border-bottom:2px solid black;">Peak</th>
					<th style="border-bottom:2px solid black;">Off Peak</th>
					<th style="border-right:2px solid black;border-bottom:2px solid black;">Total</th>
					<?php if($customer_id != 2019){ ?>
					<th style="border-left:2px solid black;border-bottom:2px solid black;">Shoulder</th>
					<th style="border-bottom:2px solid black;">Peak</th>
					<th style="border-bottom:2px solid black;">Off Peak</th>
					<th style="border-right:2px solid black;border-bottom:2px solid black;">Total</th>	
					<?php } ?>			
				</tr>
				<?php

				////////////////////REPORT STEP 1////////////////////////
				$db_values = array();
				$db_values[] = array("No.", "Metering Point","Shoulder","Peak", "Off Peak","Shoulder","Peak", "Off Peak","Shoulder","Peak", "Off Peak","Shoulder","Peak", "Off Peak", "Total","Shoulder","Peak", "Off Peak", "Total");
				///////////////////////////////////////////

				$count = 1;
				$db = new Db();
				$select = $db->select("SELECT * FROM metering_point, r_reading_cn WHERE mp_customer_id = '$customer_id' AND n_rea_cus_id = '$customer_id' AND mp_id = n_rea_mp_id");

				$total_mp = 0;
				foreach($select[0] as $row){
					extract($row);
					if(date('Y', $n_rea_date)==$year && date('m', $n_rea_date)==$month){
						$total_mp++;
					}
				}
				//$units = 0.001;

				$tc1 = $tc2 = $tc3 = $tc4= $tc5= $tc6= $tc7= $tc8 = 0;

				$co = 0;
				$t = new Db();
				$sel = $t->select("SELECT DISTINCT mp_region_id FROM metering_point, r_reading_cn WHERE mp_customer_id = '$customer_id' AND n_rea_cus_id = '$customer_id' AND mp_id = n_rea_mp_id AND mp_region_id IS NOT NULL");

				$n = 1;
				foreach($sel[0] as $r){
				extract($r);
				$tc1 = $tc2 = $tc3 = $tc4= $tc5= $tc6= $tc7= $tc8 = 0;
				if(!$t->num_rows()){
					continue;
				}

				$total_mp;
				$db = new Db();

				$select = $db->select("SELECT * FROM metering_point, r_reading_cn WHERE mp_customer_id = '$customer_id' AND n_rea_cus_id = '$customer_id' AND mp_id = n_rea_mp_id AND mp_region_id = '$mp_region_id'");



				echo '</thead>';
				echo '<tbody>';

				if($customer_id != 2019){
					echo '<tr>
					<td colspan="19" style="border-left:none; border-right:none; border-top:2px solid #000; border-bottom: 2px solid #000;color:red;"><b>'.strtoupper($this->rgf("meter_region", $mp_region_id, "region_id", "region_name")).'</b></td>
					</tr>';
				}else{
					echo '<tr>
					<td colspan="12" style="border-left:none; border-right:none; border-top:2px solid #000; border-bottom: 2px solid #000;color:red;"><b>'.strtoupper($this->rgf("meter_region", $mp_region_id, "region_id", "region_name")).'</b></td>
					</tr>';
				}
					
				$db_values[] = array('', '&nbsp;<br/><b>'.strtoupper($this->rgf("meter_region", $mp_region_id, "region_id", "region_name")).'</b>');
				foreach($select[0] as $row){
					extract($row);
					$j++;
					
					if(date('Y', $n_rea_date)==$year && date('m', $n_rea_date)==$month){

						
						
						$db = new Db();
						$sel = $db->select("SELECT * FROM r_rate_cn WHERE n_rate_reading_id = '$n_rea_id'");
						@extract($sel[0][0]);
						
						//calling the function
						$cal = rate_calculate_cn($customer_id, ($n_rea_date), $mp_id);

						$allw = $n_rate_import_wh_1+$n_rate_import_wh_2+$n_rate_import_wh_3+$n_rate_export_wh_4+$n_rate_export_wh_5+$n_rate_export_wh_6;
						//selecting rates
						if($allw > 0){

						$db_values[] = array(
							($n),
							$mp_location,
							number_format(@$n_rate_import_wh_1*$units,2),
							number_format(@$n_rate_import_wh_2*$units,2),
							number_format(@$n_rate_import_wh_3*$units,2),

							number_format(@$cal[12]*$units,2),
							number_format(@$cal[13]*$units,2),
							number_format(@$cal[14]*$units,2),

							number_format(@$n_rate_export_wh_4*$units,2),
							number_format(@$n_rate_export_wh_5*$units,2),
							number_format(@$n_rate_export_wh_6*$units,2),

							number_format($cal[0]*$units,2),
							number_format($cal[1]*$units,2),
							number_format($cal[2]*$units,2),
							number_format($cal[3]*$units,2),
							number_format($cal[4]*$units,2),
							number_format($cal[5]*$units,2),
							number_format($cal[6]*$units,2),
							number_format($cal[7]*$units,2)
							
						);

						if($co%2 == 0){
							$df = "";
						}else{
							$df = " background-color: #e6e6e6; !important;";

						}
						$co++;

						
						if($count != $total_mp){
							echo '<tr>';
							echo '<td style="'.$df.'border-left:2px solid black;">'.($n++).'.</td>';
							echo '<td style="'.$df.'border-left:2px solid black; border-right:2px solid black;">'.$this->noSpace($mp_location).'</td>';

							echo '<td style="'.$df.' text-align:right;">'.number_format(@$n_rate_import_wh_1*$units,2).'</td>';
							echo '<td style="'.$df.' text-align:right;">'.number_format(@$n_rate_import_wh_2*$units,2).'</td>';
							echo '<td style="'.$df.'border-right:2px solid black; text-align:right;">'.number_format(@$n_rate_import_wh_3*$units,2).'</td>';


							echo '<td style="'.$df.' text-align:right;color:red;">'.number_format(@$cal[12]*$units,2).'</td>';
							echo '<td style="'.$df.' text-align:right;color:red;">'.number_format(@$cal[13]*$units,2).'</td>';
							echo '<td style="'.$df.'border-right:2px solid black; text-align:right;color:red;">'.number_format(@$cal[14]*$units,2).'</td>';

							if($customer_id != 2019){
								echo '<td style="'.$df.' text-align:right;">'.number_format(@$n_rate_export_wh_4*$units,2).'</td>';
								echo '<td style="'.$df.' text-align:right;">'.number_format(@$n_rate_export_wh_5*$units,2).'</td>';
								echo '<td style="'.$df.' text-align:right;">'.number_format(@$n_rate_export_wh_6*$units,2).'</td>';
							}


							echo '<td style="'.$df.'border-left:2px solid black; text-align:right;">'.number_format($cal[0]*$units,2).'</td>';
							echo '<td style="'.$df.' text-align:right;">'.number_format($cal[1]*$units,2).'</td>';
							echo '<td style="'.$df.' text-align:right;">'.number_format($cal[2]*$units,2).'</td>';
							echo '<td style="'.$df.'border-right:2px solid black; text-align:right;">'.number_format($cal[3]*$units,2).'</td>';

							if($customer_id != 2019){
								echo '<td style="'.$df.'border-left:2px solid black; text-align:right;">'.number_format($cal[4]*$units,2).'</td>';
								echo '<td style="'.$df.' text-align:right;">'.number_format($cal[5]*$units,2).'</td>';
								echo '<td style="'.$df.' text-align:right;">'.number_format($cal[6]*$units,2).'</td>';
								echo '<td style="'.$df.'border-right:2px solid black; text-align:right;">'.number_format($cal[7]*$units,2).'</td>';	
							}			
							
							echo '</tr>';

							$tc1 += $cal[0];
							$tc2 += $cal[1];
							$tc3 += $cal[2];
							$tc4 += $cal[3];
							$tc5 += $cal[4];
							$tc6 += $cal[5];
							$tc7 += $cal[6];
							$tc8 += $cal[7];
						}else{			

							echo '<tr>';

							echo '<td style="'.$df.'border-left:2px solid black;">'.($n++).'.</td>';
							echo '<td style="'.$df.'border-left:2px solid black; border-bottom:2px solid black; ">'.$this->noSpace($mp_location).'</td>';


							echo '<td style="'.$df.'border-left:2px solid black; border-bottom:2px solid black;  text-align:right;">'.number_format(@$n_rate_import_wh_1*$units,2).'</td>';
							echo '<td style="'.$df.'border-bottom:2px solid black;  text-align:right;">'.number_format(@$rate_import_wh_2*$units,2).'</td>';
							echo '<td style="'.$df.'border-bottom:2px solid black;  text-align:right;">'.number_format(@$rate_import_wh_3*$units,2).'</td>';



							echo '<td style="'.$df.'border-left:2px solid black; border-bottom:2px solid black;  text-align:right;color:red;">'.number_format(@$cal[12]*$units,2).'</td>';
							echo '<td style="'.$df.'border-bottom:2px solid black;  text-align:right;color:red;">'.number_format(@$cal[13]*$units,2).'</td>';
							echo '<td style="'.$df.'border-bottom:2px solid black;  text-align:right;color:red;">'.number_format(@$cal[14]*$units,2).'</td>';


							if($customer_id != 2019){
								echo '<td style="'.$df.'border-left:2px solid black; border-bottom:2px solid black;  text-align:right;">'.number_format(@$n_rate_export_wh_4*$units,2).'</td>';
								echo '<td style="'.$df.'border-bottom:2px solid black;  text-align:right;">'.number_format(@$rate_export_wh_5*$units,2).'</td>';
								echo '<td style="'.$df.'border-bottom:2px solid black; text-align:right;">'.number_format(@$rate_export_wh_6*$units,2).'</td>';
							}

							echo '<td style="'.$df.'border-left:2px solid black; border-bottom:2px solid black;  text-align:right;">'.number_format($cal[0]*$units,2).'</td>';
							echo '<td style="'.$df.'border-bottom:2px solid black;  text-align:right;">'.number_format($cal[1]*$units,2).'</td>';
							echo '<td style="'.$df.'border-bottom:2px solid black;  text-align:right;">'.number_format($cal[2]*$units,2).'</td>';
							echo '<td style="'.$df.'border-right:2px solid black; border-bottom:2px solid black; text-align:right; ">'.number_format($cal[3]*$units,2).'</td>';

							if($customer_id != 2019){
								echo '<td style="'.$df.'border-left:2px solid black; border-bottom:2px solid black;  text-align:right;">'.number_format($cal[4]*$units).'</td>';
								echo '<td style="'.$df.'border-bottom:2px solid black;  text-align:right;">'.number_format($cal[5]*$units).'</td>';
								echo '<td style="'.$df.'border-bottom:2px solid black;  text-align:right;">'.number_format($cal[6]*$units).'</td>';
								echo '<td style="'.$df.'border-right:2px solid black; border-bottom:2px solid black; text-align:right; ">'.number_format($cal[7]*$units).'</td>';
							}

							echo '</tr>';

							$tc1 += $cal[0];
							$tc2 += $cal[1];
							$tc3 += $cal[2];
							$tc4 += $cal[3];
							$tc5 += $cal[4];
							$tc6 += $cal[5];
							$tc7 += $cal[6];
							$tc8 += $cal[7];
						}


						$count++;	

					}
					}
										
				}

				//if($count == $total_mp+1)
				{
					$l = $this->sumFormulae($customer_id, $year, $month);

					//$tc1 = $l[0];
					//$tc2 = $l[1];
					//$tc3 = $l[2];
					//$tc4 = $l[3];
					//$tc5 = $l[4];
					//$tc6 = $l[5];
					//$tc7 = $l[6];
					//$tc8 = $l[7];

					echo '<tr>';
					echo '<td style="border-top:2px solid black; border-top:2px solid #000;border-bottom:2px solid #000;border-left:2px solid #000;"></td>';

					if($customer_id != 2019){
						echo '<td colspan="10" style="border-top:2px solid black; border-left:2px solid black; border-bottom:2px solid black; border-right:2px solid black; text-align:right;font-weight: bold; ">Total</td>';
					}else{
						echo '<td colspan="7" style="border-top:2px solid black; border-left:2px solid black; border-bottom:2px solid black; border-right:2px solid black; text-align:right;font-weight: bold; ">Total</td>';
					}
					
					echo '<td style="border-top:2px solid black; border-bottom:2px solid black;  text-align:right;font-weight: bold;">'.number_format($tc1*$units,2).'</td>';
					echo '<td style="border-top:2px solid black; border-bottom:2px solid black;  text-align:right;font-weight: bold;">'.number_format($tc2*$units,2).'</td>';
					echo '<td style="border-top:2px solid black; border-bottom:2px solid black;  text-align:right;font-weight: bold;">'.number_format($tc3*$units,2).'</td>';

					echo '<td style="border-top:2px solid black; border-right:2px solid black; border-bottom:2px solid black; text-align:right; font-weight: bold; ">'.number_format($tc4*$units,2).'</td>';

					if($customer_id != 2019){
						echo '<td style="border-top:2px solid black; border-left:2px solid black; border-bottom:2px solid black;  text-align:right;">'.number_format($tc5*$units,2).'</td>';
						echo '<td style="border-top:2px solid black; border-bottom:2px solid black;  text-align:right;">'.number_format($tc6*$units,2).'</td>';
						echo '<td style="border-top:2px solid black; border-bottom:2px solid black;  text-align:right;">'.number_format($tc7*$units,2).'</td>';
						echo '<td style="border-top:2px solid black; border-right:2px solid black; border-bottom:2px solid black; text-align:right; ">'.number_format($tc8*$units,2).'</td>';
					}

					echo '</tr>';
				}

				}
				?>
			</tbody></table>
			<?php $this->showReadingComments(); ?>
		</div>
			<?php 
			$this->addReadingComment();

			$t = new TableCreatorSchedule();
			$heading = "$g";
			$t->open($this->full_name(user_id()), $heading);
			//$t->title($customer_short_name.' '.$year.' '.$month);
			$t->thd($db_values);
			$t->close();
			$t->results();

			$e = new Exporter();
			echo $e->getDisplay($heading, $t->results());
			
			?>
	
	</div>
	<?php
	}

	public function mpAdvance($n){
		return "";

		if($n == 2){
			return '<span class="pull-right" style="color:red; font-weight:bold;"><strong>(S)</strong></span>';
		}else{
			return "";
		}
	}

	public function readingsAction(){

		$class = ($this->set_class)?$this->set_class:portion(1);
		$method = ($this->set_method)?$this->set_method:portion(2);
		$id = ($this->set_customer)?$this->set_customer:portion(3);
		$year = ($this->set_year)?$this->set_year: portion(4);
		$month = ($this->set_month)?$this->set_month: portion(5);

		$note =  ($this->set_note)?$this->set_note:portion(6);

		if(empty($year) || empty($month) || empty($id)){
			Feedback::error("Please select Customer, Year and Month");
			$units = $this->customerAndSchedule($id, $year, $month);
			$this->energyLinks($id, $year, $month);
			$units = 0.001;
			echo '<div class="clearfix"></div>';
		}elseif(!$this->readingExist($id, $year, $month)){
			Feedback::error("No readings found");
			$units = $this->customerAndSchedule($id, $year, $month);
			$this->energyLinks($id, $year, $month);
			$units = 0.001;
			echo '<div class="clearfix"></div>';
		}else{
			if($note == "credit-note"){
				$this->readingsCreditNote($id, $year, $month, $note);
			}else{
				$this->readingsNormal($id, $year, $month, $note);
			}
		}

	}


	public function previousReadingsAction(){

		$class = ($this->set_class)?$this->set_class:portion(1);
		$method = ($this->set_method)?$this->set_method:portion(2);
		$id = ($this->set_customer)?$this->set_customer:portion(3);
		$year = ($this->set_year)?$this->set_year: portion(4);
		$month = ($this->set_month)?$this->set_month: portion(5);

		$note =  ($this->set_note)?$this->set_note:portion(6);

		if(empty($year) || empty($month) || empty($id)){
			Feedback::error("Please select Customer, Year and Month");
			$units = $this->customerAndSchedule($id, $year, $month);
			$this->energyLinks($id, $year, $month);
			$units = 0.001;
			echo '<div class="clearfix"></div>';
		}elseif(!$this->readingExist($id, $year, $month)){
			Feedback::error("No readings found");
			$units = $this->customerAndSchedule($id, $year, $month);
			$this->energyLinks($id, $year, $month);
			$units = 0.001;
			echo '<div class="clearfix"></div>';
		}else{
			if($note == "credit-note"){
				$this->readingsCreditNotePrevious($id, $year, $month, $note);
			}else{
				$this->readingsCreditNotePrevious($id, $year, $month, $note);
			}
		}

	}

	
	public function missingPoint(){

		$class = ($this->set_class)?$this->set_class:portion(1);
		$method = ($this->set_method)?$this->set_method:portion(2);
		$customer_id = ($this->set_customer)?$this->set_customer:portion(3);
		$year = ($this->set_year)?$this->set_year: portion(4);
		$month = ($this->set_month)?$this->set_month: portion(5);

		$db = new Db();
		$select = $db->select("SELECT mp_id FROM metering_point WHERE mp_customer_id = '$customer_id'");
		$mp_ids = array();
		foreach($select[0] as $row){
			extract($row);
			$mp_ids[] = $mp_id;
		}

		//$sel = $db->select("SELECT TOP 1 rea_id FROM r_reading WHERE rea_cus_id = $cus_id  AND rea_mp_id = '$metering_point' AND rea_date < $rea_date ORDER BY rea_date DESC"); 
		$reading_time = strtotime(date("$year-$month-15"));
		$select = $db->select("SELECT rea_mp_id FROM r_reading WHERE rea_cus_id = '$customer_id' AND rea_date = '$reading_time'; ");
		$mp_ids_1 = array();

		foreach($select[0] as $row){
			extract($row);
			$mp_ids_1[] = $rea_mp_id;
		}

		$missing = array();

		for($i=0; $i<count($mp_ids); $i++){

			if(!in_array($mp_ids[$i], $mp_ids_1)){
				$missing[] = $mp_ids[$i];
			}
		}



		$customer = $customer_id;

		echo '<a href="'.return_url().'services/readings/'.$customer_id.'/'.$year.'/'.$month.'">Back</a>';

		echo '<h3>Attach Metering Points</h3>';

		if(isset($_POST['add'])){
			$mp = $_POST['mp'];
			print_r($mp);
			if(1){
				$n = new Db();
				foreach($mp as $pnt){
					extract($row);
					$db = new Db();

					$insert = $db->insert("r_reading",[
						"rea_number"=>'',
						"rea_date"=>$reading_time,
						"rea_cus_id"=>$customer,
						"rea_added_by"=>user_id(),
						"rea_date_added"=>time(),
						"rea_mp_id"=>$pnt,
					]);					
					
					$db = new Db();
					$select = $db->select("SELECT TOP 1 rea_id FROM r_reading ORDER BY rea_id DESC");
					extract($select[0][0]);
					$rrr = $rea_id;
					
					{ //imports as advance
						$select = $db->insert("r_rate",[
							"rate_import_wh_1"=>0,
							"rate_import_wh_2"=>0,
							"rate_import_wh_3"=>0,
							"rate_export_wh_4"=>0,
							"rate_export_wh_5"=>0,
							"rate_export_wh_6"=>0,
							"rate_reading_id"=>$rrr,
							"rate_added_by"=>user_id(),
							"rate_date_added"=>time(),
							"rate_cus_id"=>$customer
						]);
					}						

				}
			}

			FeedBack::redirect(return_url().'services/readings/'.$customer_id.'/'.$year.'/'.$month);
		}

		echo '<form action="" method="post">';

		echo '<b>Customer:</b> '.$this->rgf("customer", $customer_id, "customer_id", "customer_short_name").'<br/>';
		echo '<b>Year:</b> '.$year.'<br/>';
		echo '<b>Month:</b> '.month_name($month).'<br/><br/>';

		foreach($missing as $m){
			echo '<input id="id'.$m.'" name="mp[]" type="checkbox" value="'.$m.'"/><label for="id'.$m.'" >'.$this->rgf('metering_point', $m, "mp_id", "mp_location").'</label>';
			echo '<br/>';
		}

		if(empty($missing))
			echo '<h5 class="text-danger">No. Missing Points</h5>';
		else{
			echo '<br/><button name="add" class="btn btn-success"><i class="fa fa-fw fa-plus"></i> Add </button>';
		}

		echo '</form>';

	}

	public function readingsNormal($id, $year, $month, $note){
		//echo "@@@@@@@@@@@@@@@";
		$db = new Db();
		$select = $db->select("SELECT * FROM customer WHERE customer_id = '$id'");
		extract($select[0][0]);
	
	?>
	<div id="side">
		<br/>
		<div class="">		
		<?php 
		$units = $this->customerAndSchedule($id, $year, $month);
		$this->energyLinks($id, $year, $month);
		$units = 0.001;

		echo '<div class="clearfix"></div>';

		echo '<h4>Readings in: <b>KWh</b>';
		echo '</h4>';

		if($this->isLocked($id, $year, $month)){
			$uu = " readonly ";
		}else{
			$uu = "";
		}
		
		if($id == 1212012012001201201){ // tanseco
			
		}else{

		$peroid = strtotime($year.'-'.$month.'-15');
		echo '<a class="eagle-load" href="'.return_url().'services/missing-point/'.$id.'/'.$year.'/'.$month.'"><i class="fa fa-fw fa-plus"></i> Attach Missing Metering Point</a>';
		echo '&nbsp;&nbsp;&nbsp;';
		echo '<a title="Download CSV" href="'.return_url().'classes/download-csv-with-readings.php?cus='.$id.'&peroid='.$peroid.'"><i class="fa fa-fw fa-download"></i> Download readings for '.$year.' '.month_name($month).' '.$customer_short_name.'</a> ';

		$tou = $this->tou($id, $month, $year);

		?>
		<div id="printMe">
			<?php
			echo '<div style="margin-bottom:-70px; z-index:100;">';
			?>
			<style>table tr td, table tr th{ height: 10px !important;} table tr th{ text-align: center;} table tr:hover td{ background-color: #efefff; } table tr td input{background-color: inherit;} table tr td input:hover, table tr td input:active, table tr td input:focus{background-color: #ccc;}</style>
			<input type="text" style="padding:0 10px; margin-bottom: 10px;" name="search" autocomplete="off" placeholder="Search Metering Point" id="searchTerm" onkeyup="doSearch()" onClick="this.select();"> &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp;			
			<?php 
			$d = date('Y-m-d', $tou[4]); 
			
			if($id != 202912 && $id != 201712){
			?>
			<span <?php if($d < date('Y-m-d')){ echo ' class="expired text-danger font-500" id="expiryDateStatus" '; }?> >Shoulder</span> <input title="<?php echo $tou[1]; ?>" <?php echo $uu; ?> type="text" id="shoulder" value="<?php echo $tou[1]; ?>" style="text-align:right; padding:0 2px; width:100px; margin-bottom: 10px;" name="search" autocomplete="off" class="number"> &nbsp; 
			<span <?php if($d < date('Y-m-d')){ echo ' class="expired text-danger font-500" id="expiryDateStatus" '; }?> >Peak</span> <input title="<?php echo $tou[0]; ?>" <?php echo $uu; ?> type="text" id="peak" value="<?php echo $tou[0]; ?>" style="text-align:right; padding:0 2px; width:100px; margin-bottom: 10px;" name="search" autocomplete="off" class="number"> &nbsp;
			<span <?php if($d < date('Y-m-d')){ echo ' class="expired text-danger font-500" id="expiryDateStatus" '; }?> >Off Peak</span> <input title="<?php echo $tou[2]; ?>" <?php echo $uu; ?> type="text" id="offPeak" value="<?php echo $tou[2]; ?>" style="text-align:right; padding:0 2px; width:100px; margin-bottom: 10px;" name="search" autocomplete="off" class="number"> 
			&nbsp; &nbsp; &nbsp; &nbsp;
			<?php 
			$d = date('Y-m-d', $tou[4]); 
			if($d < date('Y-m-d')){
			?>
			<span id="expiryDateStatus"><span class="text-danger expired" style="font-weight:bold">Expired Date</span></span> <input style="font-weight:bold; color:red;" title="<?php echo $d; ?>" <?php echo $uu; ?> type="date" id="expiryDate" value="<?php echo $d; ?>" style="text-align:right; padding:0 2px; width:130px; margin-bottom: 10px;" name="" autocomplete="off" class="">
			<?php }else{?>
			
			Expiry Date <input title="<?php echo $d; ?>" <?php echo $uu; ?> type="date" id="expiryDate" value="<?php echo $d; ?>" style="text-align:right; padding:0 2px; width:130px; margin-bottom: 10px;" name="search" autocomplete="off" class="">
			<?php } ?>
			 &nbsp; &nbsp; 
			 &nbsp;&nbsp;
			 &nbsp;
			 <?php 
			 if(!$this->isLocked($id, $year, $month)){
				echo '<span id="saveID" onclick="return saveBtn(\''.return_url().'\');" style="margin-bottom: 10px;" class="btn-sm btn-primary"><i class="fa fa-fw fa-save"></i> Save</span>';
			}else{
				echo '<span class="" style="background-color:red;color:#FFF;padding: 5px;font-size:1em;"><i class="fa fa-lock fa-fx"></i> Locked</span>';
			}
			
			}//ending if not TANESCO or SNEL
			echo '</div>';
			?>
			<div style="z-index: 1" class="attach-content" style="position:relative; height:250px;">
			<table id="sortSpan" border="1" cellspacing="0" cellpadding="2" class="table schedule" style="font-size: 12px; width:300px !important;z-index: 0">
				<thead>	
				<tr style="height:70px;"><th colspan="12" style="height:120px;border:1px solid white;">&nbsp;</th></tr>						
				<tr style="background-color: white;">
					<th style="width:20px;border-top:2px solid black;  border-left:2px solid black;border-bottom:2px solid black;">No.</th>
					<th style="border-top:2px solid black; border-left:2px solid black;border-bottom:2px solid black;">Metering Point</th>
					<th class="imports" style="min-width:80px; border-top:2px solid black; border-left:2px solid black;border-bottom:2px solid black;">Imports<br/>Shoulder <br/>( Rate 1) </th>
					<th class="imports" style="min-width:80px; border-top:2px solid black; border-bottom:2px solid black;">Imports<br/>Peak <br/>( Rate 2) </th>
					<th class="imports" style="min-width:80px; border-top:2px solid black; border-bottom:2px solid black;">Imports<br/>Off Peak <br/>( Rate 3) </th>

					<th style="min-width:80px; border-top:2px solid black; border-left:2px solid black;border-bottom:2px solid black;color:black;">Exports<br/>Shoulder <br/>( Rate 4) </th>
					<th style="min-width:80px; border-top:2px solid black; border-bottom:2px solid black;color:black;">Exports<br/>Peak <br/>( Rate 5) </th>
					<th style="min-width:80px; border-top:2px solid black; border-right:2px solid black; border-bottom:2px solid black;color:black;">Exports<br/>Off Peak <br/>( Rate 6) </th>
					<th style="min-width:80px; border-top:2px solid black; border-right:2px solid black; border-bottom:2px solid black;color:black;" class="yellow">Cummulative<br/> Imports<br/> </th>
					<th style="min-width:80px; border-top:2px solid black; border-right:2px solid black; border-bottom:2px solid black;color:black;" class="yellow">Cummulative<br/> Exports<br/> </th>
					<th style="min-width:80px; border-top:2px solid black; border-right:2px solid black; border-bottom:2px solid black;color:black;">Reading Date </th>
					<th style="min-width:140px;border-top:2px solid black; border-right:2px solid black; border-bottom:2px solid black;color:black;">
					<?php 
						if(!$this->isLocked($id, $year, $month)){
							echo '<span class="btn-danger btn-xs clear-all" onclick="return clearAll();">CA </span>';
							
						}else{
							echo '<i class="fa fa-fx fa-lock" style="color:red"></i>';
						}
					?>
					</th>
				</tr>
			</thead>
			<tbody>
				<style>
					.yellow{background-color:#dedcf7b5;}
					.imports{background-color:#dedcf7b5;}
				</style>
				<?php 
				$count = 1;
				$db = new Db();
				$select = $db->select("SELECT * FROM metering_point, r_reading WHERE mp_customer_id = '$customer_id' AND rea_cus_id = '$customer_id' AND mp_id = rea_mp_id ORDER BY mp_location ASC");

				$total_mp = 0;
				foreach($select[0] as $row){
					extract($row);
					if(date('Y', $rea_date)==$year && date('m', $rea_date)==$month){
						$total_mp++;
					}
				}
				//$units = 0.001;

				$tc1 = $tc2 = $tc3 = $tc4= $tc5= $tc6= $tc7= $tc8 = 0;

				$total_mp;
				$db = new Db();
				$select = $db->select("SELECT * FROM metering_point, r_reading WHERE mp_customer_id = '$customer_id' AND rea_cus_id = '$customer_id' AND mp_id = rea_mp_id ORDER BY mp_location ASC");
				$j = 0;

				echo '<input type="hidden" id="customerID" value="'.$customer_id.'"/>';
				echo '<input id="year" type="hidden" value="'.$year.'"/>';
				echo '<input id="month" type="hidden" value="'.$month.'"/>';
				echo '<input id="userID" type="hidden" value="'.user_id().'"/>';
				echo '<input type="hidden" value="11" id="lastValueHolder"/>';
				echo '<input type="hidden" value="11" id="lastValueHolder2"/>';
				echo '<input type="hidden" value="11" id="toSave" style="width:500px;"/>';
				$tn = $db->num_rows();
				foreach($select[0] as $row){
					extract($row);
					$v = 1;
					if(date('Y', $rea_date)==$year && date('m', $rea_date)==$month){
						//selecting rates
						
						$db = new Db();
						$sel = $db->select("SELECT * FROM r_rate WHERE rate_reading_id = '$rea_id'");
						@extract($sel[0][0]);
				if($j==0)
				//print_r($sel[0][0]);
						//calling the function
						$cal = rate_calculate($customer_id, ($rea_date), $mp_id);

						$j++;

						if($j != $total_mp){							


							echo '<tr>';

							echo '<td style="border-left:2px solid black; ">'.$count.'.</td>';
							echo '<td style="border-left:2px solid black; border-right:2px solid black;">'.$this->noSpace(strtoupper(strtolower($mp_location))).$this->mpAdvance($mp_advance).'<input type="hidden" id="mp'.$j.'" value="'.$mp_location.'"/></td>';
							
							if($j==1){
								echo '<td class="imports" style=" text-align:right;"><input '.$uu.' id="id'.($j).($v).'" onClick="this.select();" onfocus="return saveReading(\''.return_url().'\',\''.($j).($v++).'\');" type="text" class="number" value="'.$this->decimalFixer(@$rate_import_wh_1*$units).'" style="text-align:right; width:100%; padding:0 2px; margin:0;border:none;" /></td>';
							}else{
								echo '<td class="imports" style=" text-align:right;"><input '.$uu.' id="id'.($j).($v).'" onClick="this.select();" onfocus="return saveReading(\''.return_url().'\',\''.($j).($v++).'\');" type="text" class="number" value="'.$this->decimalFixer(@$rate_import_wh_1*$units).'" style="text-align:right; width:100%; padding:0 2px; margin:0;border:none;" /></td>';					
							}

							echo '<td class="imports" style=" text-align:right;"><input '.$uu.' id="id'.($j).($v).'" onClick="this.select();" onfocus="return saveReading(\''.return_url().'\',\''.($j).($v++).'\');" type="text" class="number" value="'.$this->decimalFixer(@$rate_import_wh_2*$units).'" style="text-align:right; width:100%; padding:0 2px; margin:0;border:none;" /></td>';

							echo '<td class="imports" style="border-right:2px solid black; text-align:right;"><input '.$uu.' id="id'.($j).($v).'" onClick="this.select();" onfocus="return saveReading(\''.return_url().'\',\''.($j).($v++).'\');" type="text" class="number" value="'.$this->decimalFixer(@$rate_import_wh_3*$units).'" style="text-align:right; width:100%; padding:0 2px; margin:0;border:none;" /></td>';

							echo '<td style=" text-align:right;"><input '.$uu.' id="id'.($j).($v).'" onClick="this.select();" onfocus="return saveReading(\''.return_url().'\',\''.($j).($v++).'\');" type="text" class="number" value="'.$this->decimalFixer(@$rate_export_wh_4*$units).'" style="text-align:right; width:100%; padding:0 2px; margin:0;border:none;" /></td>';
							echo '<td style=" text-align:right;"><input '.$uu.' id="id'.($j).($v).'" onClick="this.select();" onfocus="return saveReading(\''.return_url().'\',\''.($j).($v++).'\');" type="text" class="number" value="'.$this->decimalFixer(@$rate_export_wh_5*$units).'" style="text-align:right; width:100%; padding:0 2px; margin:0;border:none;" /></td>';
							echo '<td style="border-right:2px solid black; text-align:right;"><input id="id'.($j).($v).'" onClick="this.select();" onfocus="return saveReading(\''.return_url().'\',\''.($j).($v++).'\');" type="text" class="number" value="'.$this->decimalFixer(@$rate_export_wh_6*$units).'" style="text-align:right; width:100%; padding:0 2px; margin:0;border:none;" /></td>';

							echo '<td style="border-right:2px solid black; text-align:right;" class="yellow"><input id="id'.($j).($v).'" onClick="this.select();" onfocus="return saveReading(\''.return_url().'\',\''.($j).($v++).'\');" type="text" class="number" value="'.$this->decimalFixer(@$rate_cum_imports*$units).'" style="text-align:right; width:100%; padding:0 2px; margin:0;border:none;" /></td>';
							echo '<td style="border-right:2px solid black; text-align:right;" class="yellow"><input id="id'.($j).($v).'" onClick="this.select();" onfocus="return saveReading(\''.return_url().'\',\''.($j).($v++).'\');" type="text" class="number" value="'.$this->decimalFixer(@$rate_cum_exports*$units).'" style="text-align:right; width:100%; padding:0 2px; margin:0;border:none;" /></td>';

							if($j==1){
								echo '<td style="border-right:2px solid black; text-align:right;"><input id="id'.($j).($v).'" onClick="this.select();" onfocus="return saveReading(\''.return_url().'\',\''.($j).($v++).'\');" type="date" class="dateFirst dateRow'.$j.'" value="'.(@$rate_date_read).'" style="text-align:right; width:100%; padding:0 2px; margin:0;border:none;" /></td>';
							}else{
								echo '<td style="border-right:2px solid black; text-align:right;"><input id="id'.($j).($v).'" onClick="this.select();" onchange="return saveReading(\''.return_url().'\',\''.($j).($v++).'\');" onfocus="return saveReading(\''.return_url().'\',\''.($j).($v++).'\');" type="date"  class="dateRow'.$j.'" value="'.(@$rate_date_taken).'" style="text-align:right; width:100%; padding:0 2px; margin:0;border:none;" /></td>';
							}

							if(!$this->isLocked($id, $year, $month)){
								echo '<td style="border-right:2px solid black;">';
								echo '<span title="Clear Values" class="btn-xs btn-danger clear-row" onclick="clearRow(\''.$j.'\')">CV</span>';
								echo '&nbsp;&nbsp;';
								echo '<button style="font-size:10px;" type="button" id="delete_readings" rate-reading-id="'.$rate_reading_id.'" year-id="'.$year.'"  cust-id="'.$id.'" month-id="'.$month.'" class="btn btn-xs btn-danger" title="Delete reading"><i style="font-size:10px;" class="fa fa-trash"></i></button>';
								//echo "@".$rate_reading_id;
								echo '&nbsp;&nbsp;';
								$p = strtotime($year.'-'.$month.'-15');
								echo '<a href="'.return_url().'services/remove-mp-from-list/'.$mp_id.'/'.$p.'" title="Remove Metering Point ('.$mp_location.') from List" class="btn-xs btn-primary clear-row" onclick="return confirm(\'You really want to remove '.$mp_location.' from '.$year.'-'.month_name($month).' list?\n The metering point will be removed from the list and its readings.\')">Remove</a>';
								echo '</td>';							
							}else{
								echo '<td style="text-align:center; border-right:2px solid black;"><i class="fa fa-fx fa-lock" style="color:red"></i></td>';
							}
											
							echo '</tr>';
						}else{		

							echo '<tr>';

							echo '<td style="border-bottom:2px solid black; border-left:2px solid black; ">'.$count.'.</td>';
							echo '<td style="border-bottom:2px solid black; border-left:2px solid black; border-right:2px solid black;">'.strtoupper(strtolower($mp_location)).$this->mpAdvance($mp_advance).'<input type="hidden" id="mp'.$j.'" value="'.$mp_location.'"/></td>';
							
							if($j==1){
								echo '<td class="imports" style="border-bottom:2px solid black;  text-align:right;"><input '.$uu.' id="id'.($j).($v).'" onClick="this.select();" onfocus="return saveReading(\''.return_url().'\',\''.($j).($v++).'\');" type="text" class="number" value="'.$this->decimalFixer(@$rate_import_wh_1*$units).'" style="text-align:right; width:100%; padding:0 2px; margin:0;border:none;" /></td>';
							}else{
								echo '<td class="imports" style="border-bottom:2px solid black;  text-align:right;"><input '.$uu.' id="id'.($j).($v).'" onClick="this.select();" onfocus="return saveReading(\''.return_url().'\',\''.($j).($v++).'\');" type="text" class="number" value="'.$this->decimalFixer(@$rate_import_wh_1*$units).'" style="text-align:right; width:100%; padding:0 2px; margin:0;border:none;" /></td>';			
							}

							echo '<td class="imports" style="border-bottom:2px solid black;  text-align:right;"><input '.$uu.' id="id'.($j).($v).'" onClick="this.select();" onfocus="return saveReading(\''.return_url().'\',\''.($j).($v++).'\');" type="text" class="number" value="'.$this->decimalFixer(@$rate_import_wh_2*$units).'" style="text-align:right; width:100%; padding:0 2px; margin:0;border:none;" /></td>';

							echo '<td class="imports" style="border-bottom:2px solid black; border-right:2px solid black; text-align:right;"><input '.$uu.' id="id'.($j).($v).'" onClick="this.select();" onfocus="return saveReading(\''.return_url().'\',\''.($j).($v++).'\');" type="text" class="number" value="'.$this->decimalFixer(@$rate_import_wh_3*$units).'" style="text-align:right; width:100%; padding:0 2px; margin:0;border:none;" /></td>';

							echo '<td style="border-bottom:2px solid black;  text-align:right;"><input '.$uu.' id="id'.($j).($v).'" onClick="this.select();" onfocus="return saveReading(\''.return_url().'\',\''.($j).($v++).'\');" type="text" class="number" value="'.$this->decimalFixer(@$rate_export_wh_4*$units).'" style="text-align:right; width:100%; padding:0 2px; margin:0;border:none;" /></td>';
							echo '<td style="border-bottom:2px solid black;  text-align:right;"><input '.$uu.' id="id'.($j).($v).'" onClick="this.select();" onfocus="return saveReading(\''.return_url().'\',\''.($j).($v++).'\');" type="text" class="number" value="'.$this->decimalFixer(@$rate_export_wh_5*$units).'" style="text-align:right; width:100%; padding:0 2px; margin:0;border:none;" /></td>';
							echo '<td style="border-bottom:2px solid black; border-right:2px solid black; text-align:right;"><input '.$uu.' id="id'.($j).($v).'" onClick="this.select();" onfocus="return saveReading(\''.return_url().'\',\''.($j).($v++).'\');" type="text" class="number" value="'.$this->decimalFixer(@$rate_export_wh_6*$units).'" style="text-align:right; width:100%; padding:0 2px; margin:0;border:none;" /></td>';


							echo '<td class="yellow" style="border-bottom:2px solid black; border-right:2px solid black; text-align:right;"><input '.$uu.' id="id'.($j).($v).'" onClick="this.select();" onfocus="return saveReading(\''.return_url().'\',\''.($j).($v++).'\');" type="text" class="number" value="'.$this->decimalFixer(@$rate_cum_imports*$units).'" style="text-align:right; width:100%; padding:0 2px; margin:0;border:none;" /></td>';
							echo '<td class="yellow" style="border-bottom:2px solid black; border-right:2px solid black; text-align:right;"><input '.$uu.' id="id'.($j).($v).'" onClick="this.select();" onfocus="return saveReading(\''.return_url().'\',\''.($j).($v++).'\');" type="text" class="number" value="'.$this->decimalFixer(@$rate_cum_exports*$units).'" style="text-align:right; width:100%; padding:0 2px; margin:0;border:none;" /></td>';
							echo '<td class="" style="border-bottom:2px solid black; border-right:2px solid black; text-align:right;"><input '.$uu.' id="id'.($j).($v).'" onClick="this.select();" onfocus="return saveReading(\''.return_url().'\',\''.($j).($v++).'\');" onchange="return saveReading(\''.return_url().'\',\''.($j).($v++).'\');" type="date" class="dateRow'.$j.'" value="'.($rate_date_read).'" style="text-align:right; width:100%; padding:0 2px; margin:0;border:none;" /></td>';

							if(!$this->isLocked($id, $year, $month)){
								echo '<td style="border-bottom:2px solid black; border-right:2px solid black;"><span class="btn-danger btn-xs clear-row" onclick="clearRow(\''.$j.'\')">CV</span>';

								echo '&nbsp;&nbsp;';
								echo '<button style="font-size:10px;" type="button" id="delete_readings" rate-reading-id="'.$rate_reading_id.'" year-id="'.$year.'"  cust-id="'.$id.'" month-id="'.$month.'" class="btn btn-xs btn-danger" title="Delete reading"><i style="font-size:10px;" class="fa fa-trash"></i></button>';
								//echo "@".$rate_reading_id;
								echo '&nbsp;&nbsp;';
								$p = strtotime($year.'-'.$month.'-15');
								echo '<a href="'.return_url().'services/remove-mp-from-list/'.$mp_id.'/'.$p.'" title="Remove Metering Point ('.$mp_location.') from List" class="btn-xs btn-primary clear-row" onclick="return confirm(\'You really want to remove '.$mp_location.' from '.$year.'-'.month_name($month).' list?\n The metering point will be removed from the list and its readings.\')">Remove</a>';
								echo '</td>';
							}else{
								echo '<td style="text-align:center; border-bottom:2px solid black; border-right:2px solid black;"><i class="fa fa-fx fa-lock" style="color:red"></i></td>';
							}
											
							echo '</tr>';							

						}


						$count++;	

					}					
				}	

				echo '<input type="hidden" value="'.$j.'" id="allRows"/>';

				$select = $db->select("SELECT * FROM witness WHERE witness_customer_id = '$customer_id' AND witness_year = '$year' AND witness_month = '$month'");
				extract($select[0][0]);		
				?>
			</tbody></table>
		</div>
		<div id="firstname12"></div>

			<table border="0" cellspacing="5" cellpadding="2">
				<tr>
					<td style="font-weight: bold;" colspan="5"><span style=""><u> WITNESS </u></span></td>
				</tr>
				<tr>
					<td style="font-weight: bold; text-align: center;"></td>
					<td style="font-weight: bold; text-align: center;">Firstname</td>
					<td style="font-weight: bold; text-align: center;">Othername</td>
					<td style="font-weight: bold; text-align: center;">Date</td>
					<td style="font-weight: bold; text-align: center;">Time</td>
				</tr>
				<tr>
					<td style="font-weight: bold;">1. </td>
					<td><input <?php echo $uu; ?> type="text" id="firstname1" value="<?php echo $witness_firstname1; ?>"/></td>
					<td><input <?php echo $uu; ?> type="text" id="othername1" value="<?php echo $witness_othername1; ?>"/></td>
					<td><input <?php echo $uu; ?> id="date1" type="date" value="<?php echo $witness_date1; ?>"/></td>
					<td><input <?php echo $uu; ?> type="time" id="time1" value="<?php echo $witness_time1; ?>"/></td>
				</tr>
				<tr>
					<td style="font-weight: bold;">2. </td>
					<td><input <?php echo $uu; ?> type="text" id="firstname2" value="<?php echo $witness_firstname2; ?>"/></td>
					<td><input <?php echo $uu; ?> type="text" id="othername2" value="<?php echo $witness_othername2; ?>"/></td>
					<td><input <?php echo $uu; ?>  id="date2" type="date" value="<?php echo $witness_date2; ?>"/></td>
					<td><input <?php echo $uu; ?> type="time" id="time2" value="<?php echo $witness_time2; ?>"/></td>
				</tr>
				<tr>
					<td style="font-weight: bold;">3. </td>
					<td><input <?php echo $uu; ?> type="text" id="firstname3" value="<?php echo $witness_firstname3; ?>"/></td>
					<td><input  <?php echo $uu; ?>type="text" id="othername3" value="<?php echo $witness_othername3; ?>"/></td>
					<td><input <?php echo $uu; ?>  id="date3" type="date" value="<?php echo $witness_date3; ?>"/></td>
					<td><input <?php echo $uu; ?> type="time" id="time3" value="<?php echo $witness_time3; ?>"/></td>
				</tr>
			</table>

		</div>
		<script type="text/javascript">

			$('.dateFirst').change(function(){
				var dates = $('.date');
				// for(var i=1; i <= $('#allRows').val(); i++){ 
				// 	$('.dateRow'+i).val($('.dateFirst').val());
				// }
			});

			$('#date1').change(function(){
				if($('#date2').val()=="" || $('#date2').val()==null){
					$('#date2').val($('#date1').val());
				}
				if($('#date3').val()=="" || $('#date3').val()==null){
					$('#date3').val($('#date1').val());
				}
			});
			$('#time1').change(function(){
				if($('#time2').val()=="" || $('#time2').val()==null){
					$('#time2').val($('#time1').val());
				}
				if($('#time3').val()=="" || $('#time3').val()==null){
					$('#time3').val($('#time1').val());
				}
			});
		</script>
	</div>
	<?php

	}//close what is not tanseco
	}

	public function removeMpFromList(){
		$mp = portion(3);
		$period = portion(4);
		$db = new Db();
		$select = $db->select("SELECT rea_id,rea_cus_id FROM r_reading WHERE rea_mp_id = '$mp' AND rea_date = '$period'");
		extract($select[0][0]);
		
		$select = $db->select("SELECT rate_id FROM r_rate WHERE rate_reading_id = '$rea_id'");
		if($db->num_rows()){
			extract($select[0][0]);
			$delete = $db->query("DELETE FROM r_rate WHERE rate_id = '$rate_id'");
		}
		$year = date('Y',$period);
		$month = date('m',$period);


		$delete = $db->query("DELETE FROM r_reading WHERE rea_id = '$rea_id'");

		Feedback::redirect(return_url().'services/readings/'.$rea_cus_id.'/'.$year.'/'.$month);

	}

	public function readingsCreditNote($id, $year, $month, $note){

		$db = new Db();
		$select = $db->select("SELECT * FROM customer WHERE customer_id = '$id'");
		extract($select[0][0]);
	
	?>
	<div id="side">
		<br/>
		<div class="">		
		<?php 
		$units = $this->customerAndSchedule($id, $year, $month);
		$this->energyLinks($id, $year, $month);
		$units = 0.001;

		echo '<div class="clearfix"></div>';

		echo '<h4>Readings in: <b>KWh</b>';
		echo '</h4>';

		if($this->isLocked($id, $year, $month)){
			$uu = " readonly ";
		}else{
			$uu = "";
		}
		
		if($id == 1212012012001201201){ // tanseco
			
		}else{

		$peroid = strtotime($year.'-'.$month.'-15');
		echo '<a class="eagle-load" href="'.return_url().'services/missing-point/'.$id.'/'.$year.'/'.$month.'"><i class="fa fa-fw fa-plus"></i> Attach Missing Metering Point</a>';
		echo '&nbsp;&nbsp;&nbsp;';
		echo '<a title="Download CSV" href="'.return_url().'classes/download-csv-with-readings.php?cus='.$id.'&peroid='.$peroid.'"><i class="fa fa-fw fa-download"></i> Download readings for '.$year.' '.month_name($month).' '.$customer_short_name.'</a> ';

		$tou = $this->tou($id, $month, $year, 1);

		//print_r($tou);
		
		?>
		<div id="printMe">
			<?php
			echo '<div style="margin-bottom:-70px; z-index:100;">';
			?>
			<style>table tr td, table tr th{ height: 10px !important;} table tr th{ text-align: center;} table tr:hover td{ background-color: #efefff; } table tr td input{background-color: inherit;} table tr td input:hover, table tr td input:active, table tr td input:focus{background-color: #ccc;}</style>
			<input type="text" style="padding:0 10px; margin-bottom: 10px;" name="search" autocomplete="off" placeholder="Search Metering Point" id="searchTerm" onkeyup="doSearch()" onClick="this.select();"> &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp;			
			<?php 
			$d = date('Y-m-d', $tou[4]); 
			
			if($id != 202912 && $id != 201712){
			?>
			<br/>Enter CN/DN Narration
			<?php 
			if($tou[5]) $narration = $tou[5];
			else $narration = 'Credit Note/Debit Note for '.month_name($month).$year;
			?>
			<textarea style="width:100%;" id="narration" placeholder="Enter CN/DN Narration"><?php echo $narration ?></textarea>
			<span <?php if($d < date('Y-m-d')){ echo ' class="expired text-danger font-500" id="expiryDateStatus" '; }?> >Shoulder</span> <input title="<?php echo $tou[1]; ?>" <?php echo $uu; ?> type="text" id="shoulder" value="<?php echo $tou[1]; ?>" style="text-align:right; padding:0 2px; width:100px; margin-bottom: 10px;" name="search" autocomplete="off" class="number"> &nbsp; 
			<span <?php if($d < date('Y-m-d')){ echo ' class="expired text-danger font-500" id="expiryDateStatus" '; }?> >Peak</span> <input title="<?php echo $tou[0]; ?>" <?php echo $uu; ?> type="text" id="peak" value="<?php echo $tou[0]; ?>" style="text-align:right; padding:0 2px; width:100px; margin-bottom: 10px;" name="search" autocomplete="off" class="number"> &nbsp;
			<span <?php if($d < date('Y-m-d')){ echo ' class="expired text-danger font-500" id="expiryDateStatus" '; }?> >Off Peak</span> <input title="<?php echo $tou[2]; ?>" <?php echo $uu; ?> type="text" id="offPeak" value="<?php echo $tou[2]; ?>" style="text-align:right; padding:0 2px; width:100px; margin-bottom: 10px;" name="search" autocomplete="off" class="number"> 
			&nbsp; &nbsp; &nbsp; &nbsp;
			<?php 
			$d = date('Y-m-d', $tou[4]); 
			if($d < date('Y-m-d')){
			?>
			<span id="expiryDateStatus"><span class="text-danger expired" style="font-weight:bold">Expired Date</span></span> <input style="font-weight:bold; color:red;" title="<?php echo $d; ?>" <?php echo $uu; ?> type="date" id="expiryDate" value="<?php echo $d; ?>" style="text-align:right; padding:0 2px; width:130px; margin-bottom: 10px;" name="" autocomplete="off" class="">
			<?php }else{?>
			
			Expiry Date <input title="<?php echo $d; ?>" <?php echo $uu; ?> type="date" id="expiryDate" value="<?php echo $d; ?>" style="text-align:right; padding:0 2px; width:130px; margin-bottom: 10px;" name="search" autocomplete="off" class="">
			<?php } ?>
			 &nbsp; &nbsp; 
			 &nbsp;&nbsp;
			 &nbsp;
			 <?php 
			 if(!$this->isLocked($id, $year, $month)){
				echo '<span id="saveID" onclick="return saveBtnCN(\''.return_url().'\');" style="margin-bottom: 10px;" class="btn-sm btn-primary"><i class="fa fa-fw fa-save"></i> Save</span>';
			}else{
				echo '<span class="" style="background-color:red;color:#FFF;padding: 5px;font-size:1em;"><i class="fa fa-lock fa-fx"></i> Locked</span>';
			}
			
			}//ending if not TANESCO or SNEL
			echo '</div>';
			?>
			<div style="z-index: 1" class="attach-content" style="position:relative; height:250px;">
			<table id="sortSpan" border="1" cellspacing="0" cellpadding="2" class="table schedule" style="font-size: 12px; width:300px !important;z-index: 0">
				<thead>	
				<tr style="height:70px;"><th colspan="12" style="height:120px;border:1px solid white;">&nbsp;</th></tr>						
				<tr style="background-color: white;">
					<th style="width:20px;border-top:2px solid black;  border-left:2px solid black;border-bottom:2px solid black;">No.</th>
					<th style="border-top:2px solid black; border-left:2px solid black;border-bottom:2px solid black;">Metering Point</th>
					<th class="imports" style="min-width:80px; border-top:2px solid black; border-left:2px solid black;border-bottom:2px solid black;">Imports<br/>Shoulder <br/>( Rate 1) </th>
					<th class="imports" style="min-width:80px; border-top:2px solid black; border-bottom:2px solid black;">Imports<br/>Peak <br/>( Rate 2) </th>
					<th class="imports" style="min-width:80px; border-top:2px solid black; border-bottom:2px solid black;">Imports<br/>Off Peak <br/>( Rate 3) </th>

					<th style="min-width:80px; border-top:2px solid black; border-left:2px solid black;border-bottom:2px solid black;color:black;">Exports<br/>Shoulder <br/>( Rate 4) </th>
					<th style="min-width:80px; border-top:2px solid black; border-bottom:2px solid black;color:black;">Exports<br/>Peak <br/>( Rate 5) </th>
					<th style="min-width:80px; border-top:2px solid black; border-right:2px solid black; border-bottom:2px solid black;color:black;">Exports<br/>Off Peak <br/>( Rate 6) </th>
					<th style="min-width:80px; border-top:2px solid black; border-right:2px solid black; border-bottom:2px solid black;color:black;" class="yellow">Cummulative<br/> Imports<br/> </th>
					<th style="min-width:80px; border-top:2px solid black; border-right:2px solid black; border-bottom:2px solid black;color:black;" class="yellow">Cummulative<br/> Exports<br/> </th>
					<th style="min-width:80px; border-top:2px solid black; border-right:2px solid black; border-bottom:2px solid black;color:black;">Reading Date </th>
					<th style="border-top:2px solid black; border-right:2px solid black; border-bottom:2px solid black;color:black;">
					<?php 
						if(!$this->isLocked($id, $year, $month)){
							echo '<span class="btn-danger btn-xs clear-all" onclick="return clearAll();">CA</span>';
						}else{
							echo '<i class="fa fa-fx fa-lock" style="color:red"></i>';
						}
					?>
					</th>
				</tr>
			</thead>
			<tbody>
				<style>
					.yellow{background-color:#dedcf7b5;}
					.imports{background-color:#dedcf7b5;}
				</style>
				<?php 
				$count = 1;
				$db = new Db();
				$select = $db->select("SELECT * FROM metering_point, r_reading WHERE mp_customer_id = '$customer_id' AND rea_cus_id = '$customer_id' AND mp_id = rea_mp_id ORDER BY mp_location ASC");

				$total_mp = 0;
				foreach($select[0] as $row){
					extract($row);
					if(date('Y', $rea_date)==$year && date('m', $rea_date)==$month){
						$total_mp++;
					}
				}
				//$units = 0.001;

				$tc1 = $tc2 = $tc3 = $tc4= $tc5= $tc6= $tc7= $tc8 = 0;

				$total_mp;
				$db = new Db();
				$select = $db->select("SELECT * FROM metering_point, r_reading WHERE mp_customer_id = '$customer_id' AND rea_cus_id = '$customer_id' AND mp_id = rea_mp_id ORDER BY mp_location ASC");
				$j = 0;

				echo '<input type="hidden" id="customerID" value="'.$customer_id.'"/>';
				echo '<input id="year" type="hidden" value="'.$year.'"/>';
				echo '<input id="month" type="hidden" value="'.$month.'"/>';
				echo '<input id="userID" type="hidden" value="'.user_id().'"/>';
				echo '<input type="hidden" value="11" id="lastValueHolder"/>';
				echo '<input type="hidden" value="11" id="lastValueHolder2"/>';
				echo '<input type="hidden" value="11" id="toSave" style="width:500px;"/>';
				$tn = $db->num_rows();
				foreach($select[0] as $row){
					extract($row);
					$v = 1;
					if(date('Y', $rea_date)==$year && date('m', $rea_date)==$month){
						//selecting rates
						$time_readings = strtotime($year.'-'.$month.'-15');
						$db = new Db();
						$sql = "SELECT * FROM r_rate_cn,r_reading_cn WHERE n_rea_cus_id = '$customer_id' AND n_rate_reading_id = n_rea_id AND n_rea_date = '$time_readings' AND n_rea_mp_id = '$mp_id'";
						$sel = $db->select($sql);
						@extract($sel[0][0]);

						if(!$db->num_rows()){
							// $db = new Db();
							// $sql = "SELECT rate_import_wh_1 as n_rate_import_wh_1,rate_import_wh_2 as n_rate_import_wh_2,rate_import_wh_3 as n_rate_import_wh_3,rate_export_wh_4 as n_rate_export_wh_4, rate_export_wh_5 as n_rate_export_wh_5 , rate_export_wh_6 as n_rate_export_wh_6 FROM r_rate,r_reading WHERE rea_cus_id = '$customer_id' AND rate_reading_id = rea_id AND rea_date = '$time_readings' AND rea_mp_id = '$mp_id'";
							// $sel = $db->select($sql);
							// @extract($sel[0][0]);
						}
						
						// $db = new Db();
						// $sel = $db->select("SELECT * FROM r_rate_cn WHERE n_rate_reading_id = '$rea_id'");
						// @extract($sel[0][0]);

				if($j==0)
				//print_r($sel[0][0]);
						//calling the function
						$cal = rate_calculate($customer_id, ($rea_date), $mp_id);

						$j++;

						if($j != $total_mp){							


							echo '<tr>';

							echo '<td style="border-left:2px solid black; ">'.$count.'.</td>';
							echo '<td style="border-left:2px solid black; border-right:2px solid black;">'.$this->noSpace(strtoupper(strtolower($mp_location))).$this->mpAdvance($mp_advance).'<input type="hidden" id="mp'.$j.'" value="'.$mp_location.'"/></td>';
							
							if($j==1){
								echo '<td class="imports" style=" text-align:right;"><input '.$uu.' id="id'.($j).($v).'" onClick="this.select();" onfocus="return saveReadingCN(\''.return_url().'\',\''.($j).($v++).'\');" type="text" class="number" value="'.$this->decimalFixer(@$n_rate_import_wh_1*$units).'" style="text-align:right; width:100%; padding:0 2px; margin:0;border:none;" /></td>';
							}else{
								echo '<td class="imports" style=" text-align:right;"><input '.$uu.' id="id'.($j).($v).'" onClick="this.select();" onfocus="return saveReadingCN(\''.return_url().'\',\''.($j).($v++).'\');" type="text" class="number" value="'.$this->decimalFixer(@$n_rate_import_wh_1*$units).'" style="text-align:right; width:100%; padding:0 2px; margin:0;border:none;" /></td>';					
							}

							echo '<td class="imports" style=" text-align:right;"><input '.$uu.' id="id'.($j).($v).'" onClick="this.select();" onfocus="return saveReadingCN(\''.return_url().'\',\''.($j).($v++).'\');" type="text" class="number" value="'.$this->decimalFixer(@$n_rate_import_wh_2*$units).'" style="text-align:right; width:100%; padding:0 2px; margin:0;border:none;" /></td>';

							echo '<td class="imports" style="border-right:2px solid black; text-align:right;"><input '.$uu.' id="id'.($j).($v).'" onClick="this.select();" onfocus="return saveReadingCN(\''.return_url().'\',\''.($j).($v++).'\');" type="text" class="number" value="'.$this->decimalFixer(@$n_rate_import_wh_3*$units).'" style="text-align:right; width:100%; padding:0 2px; margin:0;border:none;" /></td>';

							echo '<td style=" text-align:right;"><input '.$uu.' id="id'.($j).($v).'" onClick="this.select();" onfocus="return saveReadingCN(\''.return_url().'\',\''.($j).($v++).'\');" type="text" class="number" value="'.$this->decimalFixer(@$n_rate_export_wh_4*$units).'" style="text-align:right; width:100%; padding:0 2px; margin:0;border:none;" /></td>';
							echo '<td style=" text-align:right;"><input '.$uu.' id="id'.($j).($v).'" onClick="this.select();" onfocus="return saveReadingCN(\''.return_url().'\',\''.($j).($v++).'\');" type="text" class="number" value="'.$this->decimalFixer(@$n_rate_export_wh_5*$units).'" style="text-align:right; width:100%; padding:0 2px; margin:0;border:none;" /></td>';
							echo '<td style="border-right:2px solid black; text-align:right;"><input id="id'.($j).($v).'" onClick="this.select();" onfocus="return saveReadingCN(\''.return_url().'\',\''.($j).($v++).'\');" type="text" class="number" value="'.$this->decimalFixer(@$n_rate_export_wh_6*$units).'" style="text-align:right; width:100%; padding:0 2px; margin:0;border:none;" /></td>';

							echo '<td style="border-right:2px solid black; text-align:right;" class="yellow"><input id="id'.($j).($v).'" onClick="this.select();" onfocus="return saveReadingCN(\''.return_url().'\',\''.($j).($v++).'\');" type="text" class="number" value="'.$this->decimalFixer(@$n_rate_cum_imports*$units).'" style="text-align:right; width:100%; padding:0 2px; margin:0;border:none;" /></td>';
							echo '<td style="border-right:2px solid black; text-align:right;" class="yellow"><input id="id'.($j).($v).'" onClick="this.select();" onfocus="return saveReadingCN(\''.return_url().'\',\''.($j).($v++).'\');" type="text" class="number" value="'.$this->decimalFixer(@$n_rate_cum_exports*$units).'" style="text-align:right; width:100%; padding:0 2px; margin:0;border:none;" /></td>';

							if($j==1){
								echo '<td style="border-right:2px solid black; text-align:right;"><input id="id'.($j).($v).'" onClick="this.select();" onfocus="return saveReadingCN(\''.return_url().'\',\''.($j).($v++).'\');" type="date" class="dateFirst dateRow'.$j.'" value="'.(@$rate_date_read).'" style="text-align:right; width:100%; padding:0 2px; margin:0;border:none;" /></td>';
							}else{
								echo '<td style="border-right:2px solid black; text-align:right;"><input id="id'.($j).($v).'" onClick="this.select();" onchange="return saveReadingCN(\''.return_url().'\',\''.($j).($v++).'\');" onfocus="return saveReadingCN(\''.return_url().'\',\''.($j).($v++).'\');" type="date"  class="dateRow'.$j.'" value="'.(@$rate_date_taken).'" style="text-align:right; width:100%; padding:0 2px; margin:0;border:none;" /></td>';
							}

							if(!$this->isLocked($id, $year, $month)){
								echo '<td style="border-right:2px solid black;">';
								echo '<span title="Clear Values" class="btn-xs btn-danger clear-row" onclick="clearRow(\''.$j.'\')">CV</span>';
								echo '&nbsp;&nbsp;';
								$p = strtotime($year.'-'.$month.'-15');
								echo '<a href="'.return_url().'services/remove-mp-from-list/'.$mp_id.'/'.$p.'" title="Remove Metering Point ('.$mp_location.') from List" class="btn-xs btn-primary clear-row" onclick="return confirm(\'You really want to remove '.$mp_location.' from '.$year.'-'.month_name($month).' list?\n The metering point will be removed from the list and its readings.\')">Remove</a>';
								echo '</td>';							
							}else{
								echo '<td style="text-align:center; border-right:2px solid black;"><i class="fa fa-fx fa-lock" style="color:red"></i></td>';
							}
											
							echo '</tr>';
						}else{		

							echo '<tr>';

							echo '<td style="border-bottom:2px solid black; border-left:2px solid black; ">'.$count.'.</td>';
							echo '<td style="border-bottom:2px solid black; border-left:2px solid black; border-right:2px solid black;">'.strtoupper(strtolower($mp_location)).$this->mpAdvance($mp_advance).'<input type="hidden" id="mp'.$j.'" value="'.$mp_location.'"/></td>';
							
							if($j==1){
								echo '<td class="imports" style="border-bottom:2px solid black;  text-align:right;"><input '.$uu.' id="id'.($j).($v).'" onClick="this.select();" onfocus="return saveReadingCN(\''.return_url().'\',\''.($j).($v++).'\');" type="text" class="number" value="'.$this->decimalFixer(@$n_rate_import_wh_1*$units).'" style="text-align:right; width:100%; padding:0 2px; margin:0;border:none;" /></td>';
							}else{
								echo '<td class="imports" style="border-bottom:2px solid black;  text-align:right;"><input '.$uu.' id="id'.($j).($v).'" onClick="this.select();" onfocus="return saveReadingCN(\''.return_url().'\',\''.($j).($v++).'\');" type="text" class="number" value="'.$this->decimalFixer(@$n_rate_import_wh_1*$units).'" style="text-align:right; width:100%; padding:0 2px; margin:0;border:none;" /></td>';			
							}

							echo '<td class="imports" style="border-bottom:2px solid black;  text-align:right;"><input '.$uu.' id="id'.($j).($v).'" onClick="this.select();" onfocus="return saveReadingCN(\''.return_url().'\',\''.($j).($v++).'\');" type="text" class="number" value="'.$this->decimalFixer(@$n_rate_import_wh_2*$units).'" style="text-align:right; width:100%; padding:0 2px; margin:0;border:none;" /></td>';

							echo '<td class="imports" style="border-bottom:2px solid black; border-right:2px solid black; text-align:right;"><input '.$uu.' id="id'.($j).($v).'" onClick="this.select();" onfocus="return saveReadingCN(\''.return_url().'\',\''.($j).($v++).'\');" type="text" class="number" value="'.$this->decimalFixer(@$n_rate_import_wh_3*$units).'" style="text-align:right; width:100%; padding:0 2px; margin:0;border:none;" /></td>';

							echo '<td style="border-bottom:2px solid black;  text-align:right;"><input '.$uu.' id="id'.($j).($v).'" onClick="this.select();" onfocus="return saveReadingCN(\''.return_url().'\',\''.($j).($v++).'\');" type="text" class="number" value="'.$this->decimalFixer(@$n_rate_export_wh_4*$units).'" style="text-align:right; width:100%; padding:0 2px; margin:0;border:none;" /></td>';
							echo '<td style="border-bottom:2px solid black;  text-align:right;"><input '.$uu.' id="id'.($j).($v).'" onClick="this.select();" onfocus="return saveReadingCN(\''.return_url().'\',\''.($j).($v++).'\');" type="text" class="number" value="'.$this->decimalFixer(@$n_rate_export_wh_5*$units).'" style="text-align:right; width:100%; padding:0 2px; margin:0;border:none;" /></td>';
							echo '<td style="border-bottom:2px solid black; border-right:2px solid black; text-align:right;"><input '.$uu.' id="id'.($j).($v).'" onClick="this.select();" onfocus="return saveReadingCN(\''.return_url().'\',\''.($j).($v++).'\');" type="text" class="number" value="'.$this->decimalFixer(@$n_rate_export_wh_6*$units).'" style="text-align:right; width:100%; padding:0 2px; margin:0;border:none;" /></td>';


							echo '<td class="yellow" style="border-bottom:2px solid black; border-right:2px solid black; text-align:right;"><input '.$uu.' id="id'.($j).($v).'" onClick="this.select();" onfocus="return saveReadingCN(\''.return_url().'\',\''.($j).($v++).'\');" type="text" class="number" value="'.$this->decimalFixer(@$n_rate_cum_imports*$units).'" style="text-align:right; width:100%; padding:0 2px; margin:0;border:none;" /></td>';
							echo '<td class="yellow" style="border-bottom:2px solid black; border-right:2px solid black; text-align:right;"><input '.$uu.' id="id'.($j).($v).'" onClick="this.select();" onfocus="return saveReadingCN(\''.return_url().'\',\''.($j).($v++).'\');" type="text" class="number" value="'.$this->decimalFixer(@$n_rate_cum_exports*$units).'" style="text-align:right; width:100%; padding:0 2px; margin:0;border:none;" /></td>';
							echo '<td class="" style="border-bottom:2px solid black; border-right:2px solid black; text-align:right;"><input '.$uu.' id="id'.($j).($v).'" onClick="this.select();" onfocus="return saveReadingCN(\''.return_url().'\',\''.($j).($v++).'\');" onchange="return saveReadingCN(\''.return_url().'\',\''.($j).($v++).'\');" type="date" class="dateRow'.$j.'" value="'.($n_rate_date_read).'" style="text-align:right; width:100%; padding:0 2px; margin:0;border:none;" /></td>';

							if(!$this->isLocked($id, $year, $month)){
								echo '<td style="border-bottom:2px solid black; border-right:2px solid black;"><span class="btn-danger btn-xs clear-row" onclick="clearRow(\''.$j.'\')">CV</span></td>';

							}else{
								echo '<td style="text-align:center; border-bottom:2px solid black; border-right:2px solid black;"><i class="fa fa-fx fa-lock" style="color:red"></i></td>';
							}
											
							echo '</tr>';							

						}


						$count++;	

					}					
				}	

				echo '<input type="hidden" value="'.$j.'" id="allRows"/>';

				$select = $db->select("SELECT * FROM witness WHERE witness_customer_id = '$customer_id' AND witness_year = '$year' AND witness_month = '$month'");
				extract($select[0][0]);		
				?>
			</tbody></table>
		</div>

			<table border="0" cellspacing="5" cellpadding="2">
				<tr>
					<td style="font-weight: bold;" colspan="5"><span style=""><u> WITNESS </u></span></td>
				</tr>
				<tr>
					<td style="font-weight: bold; text-align: center;"></td>
					<td style="font-weight: bold; text-align: center;">Firstname</td>
					<td style="font-weight: bold; text-align: center;">Othername</td>
					<td style="font-weight: bold; text-align: center;">Date</td>
					<td style="font-weight: bold; text-align: center;">Time</td>
				</tr>
				<tr>
					<td style="font-weight: bold;">1. </td>
					<td><input <?php echo $uu; ?> type="text" id="firstname1" value="<?php echo $witness_firstname1; ?>"/></td>
					<td><input <?php echo $uu; ?> type="text" id="othername1" value="<?php echo $witness_othername1; ?>"/></td>
					<td><input <?php echo $uu; ?> id="date1" type="date" value="<?php echo $witness_date1; ?>"/></td>
					<td><input <?php echo $uu; ?> type="time" id="time1" value="<?php echo $witness_time1; ?>"/></td>
				</tr>
				<tr>
					<td style="font-weight: bold;">2. </td>
					<td><input <?php echo $uu; ?> type="text" id="firstname2" value="<?php echo $witness_firstname2; ?>"/></td>
					<td><input <?php echo $uu; ?> type="text" id="othername2" value="<?php echo $witness_othername2; ?>"/></td>
					<td><input <?php echo $uu; ?>  id="date2" type="date" value="<?php echo $witness_date2; ?>"/></td>
					<td><input <?php echo $uu; ?> type="time" id="time2" value="<?php echo $witness_time2; ?>"/></td>
				</tr>
				<tr>
					<td style="font-weight: bold;">3. </td>
					<td><input <?php echo $uu; ?> type="text" id="firstname3" value="<?php echo $witness_firstname3; ?>"/></td>
					<td><input  <?php echo $uu; ?>type="text" id="othername3" value="<?php echo $witness_othername3; ?>"/></td>
					<td><input <?php echo $uu; ?>  id="date3" type="date" value="<?php echo $witness_date3; ?>"/></td>
					<td><input <?php echo $uu; ?> type="time" id="time3" value="<?php echo $witness_time3; ?>"/></td>
				</tr>
			</table>

		</div>
		<div id="xtext"></div>
		<script type="text/javascript">

			$('.dateFirst').change(function(){
				var dates = $('.date');
				// for(var i=1; i <= $('#allRows').val(); i++){ 
				// 	$('.dateRow'+i).val($('.dateFirst').val());
				// }
			});

			$('#date1').change(function(){
				if($('#date2').val()=="" || $('#date2').val()==null){
					$('#date2').val($('#date1').val());
				}
				if($('#date3').val()=="" || $('#date3').val()==null){
					$('#date3').val($('#date1').val());
				}
			});
			$('#time1').change(function(){
				if($('#time2').val()=="" || $('#time2').val()==null){
					$('#time2').val($('#time1').val());
				}
				if($('#time3').val()=="" || $('#time3').val()==null){
					$('#time3').val($('#time1').val());
				}
			});
		</script>
	</div>
	<?php

	}//close what is not tanseco
	}

	

	public function readingsCreditNotePrevious($id, $year, $month, $note){

		$db = new Db();
		$select = $db->select("SELECT * FROM customer WHERE customer_id = '$id'");
		extract($select[0][0]);
	
	?>
	<div id="side">
		<br/>
		<div class="">		
		<?php 
		$units = $this->customerAndSchedule($id, $year, $month);
		$this->energyLinks($id, $year, $month);
		$units = 0.001;

		echo '<div class="clearfix"></div>';

		echo '<h4>Readings in: <b>KWh</b>';
		echo '</h4>';

		if($this->isLocked($id, $year, $month)){
			$uu = " readonly ";
		}else{
			$uu = "";
		}
		
		if($id == 1212012012001201201){ // tanseco
			
		}else{

		$peroid = strtotime($year.'-'.$month.'-15');
		echo '<a class="eagle-load" href="'.return_url().'services/missing-point/'.$id.'/'.$year.'/'.$month.'"><i class="fa fa-fw fa-plus"></i> Attach Missing Metering Point</a>';
		echo '&nbsp;&nbsp;&nbsp;';
		echo '<a title="Download CSV" href="'.return_url().'classes/download-csv-with-readings.php?cus='.$id.'&peroid='.$peroid.'"><i class="fa fa-fw fa-download"></i> Download readings for '.$year.' '.month_name($month).' '.$customer_short_name.'</a> ';

		$tou = $this->tou($id, $month, $year, 1);

		//print_r($tou);
		
		?>
		<div id="printMe">
			<?php
			echo '<div style="margin-bottom:-70px; z-index:100;">';
			?>
			<style>table tr td, table tr th{ height: 10px !important;} table tr th{ text-align: center;} table tr:hover td{ background-color: #efefff; } table tr td input{background-color: inherit;} table tr td input:hover, table tr td input:active, table tr td input:focus{background-color: #ccc;}</style>
			<input type="text" style="padding:0 10px; margin-bottom: 10px;" name="search" autocomplete="off" placeholder="Search Metering Point" id="searchTerm" onkeyup="doSearch()" onClick="this.select();"> &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp;			
			<?php 
			$d = date('Y-m-d', $tou[4]); 
			
			if($id != 202912 && $id != 201712){
			?>
			<span <?php if($d < date('Y-m-d')){ echo ' class="expired text-danger font-500" id="expiryDateStatus" '; }?> >Shoulder</span> <input title="<?php echo $tou[1]; ?>" <?php echo $uu; ?> type="text" id="shoulder" value="<?php echo $tou[1]; ?>" style="text-align:right; padding:0 2px; width:100px; margin-bottom: 10px;" name="search" autocomplete="off" class="number"> &nbsp; 
			<span <?php if($d < date('Y-m-d')){ echo ' class="expired text-danger font-500" id="expiryDateStatus" '; }?> >Peak</span> <input title="<?php echo $tou[0]; ?>" <?php echo $uu; ?> type="text" id="peak" value="<?php echo $tou[0]; ?>" style="text-align:right; padding:0 2px; width:100px; margin-bottom: 10px;" name="search" autocomplete="off" class="number"> &nbsp;
			<span <?php if($d < date('Y-m-d')){ echo ' class="expired text-danger font-500" id="expiryDateStatus" '; }?> >Off Peak</span> <input title="<?php echo $tou[2]; ?>" <?php echo $uu; ?> type="text" id="offPeak" value="<?php echo $tou[2]; ?>" style="text-align:right; padding:0 2px; width:100px; margin-bottom: 10px;" name="search" autocomplete="off" class="number"> 
			&nbsp; &nbsp; &nbsp; &nbsp;
			<?php 
			$d = date('Y-m-d', $tou[4]); 
			if($d < date('Y-m-d')){
			?>
			<span id="expiryDateStatus"><span class="text-danger expired" style="font-weight:bold">Expired Date</span></span> <input style="font-weight:bold; color:red;" title="<?php echo $d; ?>" <?php echo $uu; ?> type="date" id="expiryDate" value="<?php echo $d; ?>" style="text-align:right; padding:0 2px; width:130px; margin-bottom: 10px;" name="" autocomplete="off" class="">
			<?php }else{?>
			
			Expiry Date <input title="<?php echo $d; ?>" <?php echo $uu; ?> type="date" id="expiryDate" value="<?php echo $d; ?>" style="text-align:right; padding:0 2px; width:130px; margin-bottom: 10px;" name="search" autocomplete="off" class="">
			<?php } ?>
			 &nbsp; &nbsp; 
			 &nbsp;&nbsp;
			 &nbsp;
			 <?php 
			 if(!$this->isLocked($id, $year, $month)){
				echo '<span id="saveID" onclick="return saveBtnCNPrevious(\''.return_url().'\');" style="margin-bottom: 10px;" class="btn-sm btn-primary"><i class="fa fa-fw fa-save"></i> Save</span>';
			}else{
				echo '<span class="" style="background-color:red;color:#FFF;padding: 5px;font-size:1em;"><i class="fa fa-lock fa-fx"></i> Locked</span>';
			}
			
			}//ending if not TANESCO or SNEL
			echo '</div>';
			?>
			<div style="z-index: 1" class="attach-content" style="position:relative; height:250px;">
			<table id="sortSpan" border="1" cellspacing="0" cellpadding="2" class="table schedule" style="font-size: 12px; width:300px !important;z-index: 0">
				<thead>	
				<tr style="height:70px;"><th colspan="12" style="height:120px;border:1px solid white;">&nbsp;</th></tr>						
				<tr style="background-color: white;">
					<th style="width:20px;border-top:2px solid black;  border-left:2px solid black;border-bottom:2px solid black;">No.</th>
					<th style="border-top:2px solid black; border-left:2px solid black;border-bottom:2px solid black;">Metering Point</th>
					<th class="imports" style="min-width:80px; border-top:2px solid black; border-left:2px solid black;border-bottom:2px solid black;">Imports<br/>Shoulder <br/>( Rate 1) </th>
					<th class="imports" style="min-width:80px; border-top:2px solid black; border-bottom:2px solid black;">Imports<br/>Peak <br/>( Rate 2) </th>
					<th class="imports" style="min-width:80px; border-top:2px solid black; border-bottom:2px solid black;">Imports<br/>Off Peak <br/>( Rate 3) </th>

					<th style="min-width:80px; border-top:2px solid black; border-left:2px solid black;border-bottom:2px solid black;color:black;">Exports<br/>Shoulder <br/>( Rate 4) </th>
					<th style="min-width:80px; border-top:2px solid black; border-bottom:2px solid black;color:black;">Exports<br/>Peak <br/>( Rate 5) </th>
					<th style="min-width:80px; border-top:2px solid black; border-right:2px solid black; border-bottom:2px solid black;color:black;">Exports<br/>Off Peak <br/>( Rate 6) </th>
					<th style="min-width:80px; border-top:2px solid black; border-right:2px solid black; border-bottom:2px solid black;color:black;" class="yellow">Cummulative<br/> Imports<br/> </th>
					<th style="min-width:80px; border-top:2px solid black; border-right:2px solid black; border-bottom:2px solid black;color:black;" class="yellow">Cummulative<br/> Exports<br/> </th>
					<th style="min-width:80px; border-top:2px solid black; border-right:2px solid black; border-bottom:2px solid black;color:black;">Reading Date </th>
					<th style="border-top:2px solid black; border-right:2px solid black; border-bottom:2px solid black;color:black;">
					<?php 
						if(!$this->isLocked($id, $year, $month)){
							echo '<span class="btn-danger btn-xs clear-all" onclick="return clearAll();">CA</span>';
						}else{
							echo '<i class="fa fa-fx fa-lock" style="color:red"></i>';
						}
					?>
					</th>
				</tr>
			</thead>
			<tbody>
				<style>
					.yellow{background-color:#dedcf7b5;}
					.imports{background-color:#dedcf7b5;}
				</style>
				<?php 
				$count = 1;
				$db = new Db();
				$select = $db->select("SELECT * FROM metering_point, r_reading WHERE mp_customer_id = '$customer_id' AND rea_cus_id = '$customer_id' AND mp_id = rea_mp_id ORDER BY mp_location ASC");

				$total_mp = 0;
				foreach($select[0] as $row){
					extract($row);
					if(date('Y', $rea_date)==$year && date('m', $rea_date)==$month){
						$total_mp++;
					}
				}
				//$units = 0.001;

				$tc1 = $tc2 = $tc3 = $tc4= $tc5= $tc6= $tc7= $tc8 = 0;

				$total_mp;
				$db = new Db();
				$select = $db->select("SELECT * FROM metering_point, r_reading WHERE mp_customer_id = '$customer_id' AND rea_cus_id = '$customer_id' AND mp_id = rea_mp_id ORDER BY mp_location ASC");
				$j = 0;

				echo '<input type="hidden" id="customerID" value="'.$customer_id.'"/>';
				echo '<input id="year" type="hidden" value="'.$year.'"/>';
				echo '<input id="month" type="hidden" value="'.$month.'"/>';
				echo '<input id="userID" type="hidden" value="'.user_id().'"/>';
				echo '<input type="hidden" value="11" id="lastValueHolder"/>';
				echo '<input type="hidden" value="11" id="lastValueHolder2"/>';
				echo '<input type="hidden" value="11" id="toSave" style="width:500px;"/>';
				$tn = $db->num_rows();
				foreach($select[0] as $row){
					extract($row);
					$v = 1;
					if(date('Y', $rea_date)==$year && date('m', $rea_date)==$month){
						//selecting rates
						$time_readings = strtotime($year.'-'.$month.'-15');
						$db = new Db();
						$sql = "SELECT * FROM r_rate_cn_previous,r_reading_cn_previous WHERE n_rea_cus_id = '$customer_id' AND n_rate_reading_id = n_rea_id AND n_rea_date = '$time_readings' AND n_rea_mp_id = '$mp_id'";
						$sel = $db->select($sql);
						@extract($sel[0][0]);

						if(!$db->num_rows()){
							// $db = new Db();
							// $sql = "SELECT rate_import_wh_1 as n_rate_import_wh_1,rate_import_wh_2 as n_rate_import_wh_2,rate_import_wh_3 as n_rate_import_wh_3,rate_export_wh_4 as n_rate_export_wh_4, rate_export_wh_5 as n_rate_export_wh_5 , rate_export_wh_6 as n_rate_export_wh_6 FROM r_rate,r_reading WHERE rea_cus_id = '$customer_id' AND rate_reading_id = rea_id AND rea_date = '$time_readings' AND rea_mp_id = '$mp_id'";
							// $sel = $db->select($sql);
							// @extract($sel[0][0]);
						}
						
						// $db = new Db();
						// $sel = $db->select("SELECT * FROM r_rate_cn WHERE n_rate_reading_id = '$rea_id'");
						// @extract($sel[0][0]);

				if($j==0)
				//print_r($sel[0][0]);
						//calling the function
						$cal = rate_calculate($customer_id, ($rea_date), $mp_id);

						$j++;

						if($j != $total_mp){							


							echo '<tr>';

							echo '<td style="border-left:2px solid black; ">'.$count.'.</td>';
							echo '<td style="border-left:2px solid black; border-right:2px solid black;">'.$this->noSpace(strtoupper(strtolower($mp_location))).$this->mpAdvance($mp_advance).'<input type="hidden" id="mp'.$j.'" value="'.$mp_location.'"/></td>';
							
							if($j==1){
								echo '<td class="imports" style=" text-align:right;"><input '.$uu.' id="id'.($j).($v).'" onClick="this.select();" onfocus="return saveReadingCNPrevious(\''.return_url().'\',\''.($j).($v++).'\');" type="text" class="number" value="'.$this->decimalFixer(@$n_rate_import_wh_1*$units).'" style="text-align:right; width:100%; padding:0 2px; margin:0;border:none;" /></td>';
							}else{
								echo '<td class="imports" style=" text-align:right;"><input '.$uu.' id="id'.($j).($v).'" onClick="this.select();" onfocus="return saveReadingCNPrevious(\''.return_url().'\',\''.($j).($v++).'\');" type="text" class="number" value="'.$this->decimalFixer(@$n_rate_import_wh_1*$units).'" style="text-align:right; width:100%; padding:0 2px; margin:0;border:none;" /></td>';					
							}

							echo '<td class="imports" style=" text-align:right;"><input '.$uu.' id="id'.($j).($v).'" onClick="this.select();" onfocus="return saveReadingCNPrevious(\''.return_url().'\',\''.($j).($v++).'\');" type="text" class="number" value="'.$this->decimalFixer(@$n_rate_import_wh_2*$units).'" style="text-align:right; width:100%; padding:0 2px; margin:0;border:none;" /></td>';

							echo '<td class="imports" style="border-right:2px solid black; text-align:right;"><input '.$uu.' id="id'.($j).($v).'" onClick="this.select();" onfocus="return saveReadingCNPrevious(\''.return_url().'\',\''.($j).($v++).'\');" type="text" class="number" value="'.$this->decimalFixer(@$n_rate_import_wh_3*$units).'" style="text-align:right; width:100%; padding:0 2px; margin:0;border:none;" /></td>';

							echo '<td style=" text-align:right;"><input '.$uu.' id="id'.($j).($v).'" onClick="this.select();" onfocus="return saveReadingCNPrevious(\''.return_url().'\',\''.($j).($v++).'\');" type="text" class="number" value="'.$this->decimalFixer(@$n_rate_export_wh_4*$units).'" style="text-align:right; width:100%; padding:0 2px; margin:0;border:none;" /></td>';
							echo '<td style=" text-align:right;"><input '.$uu.' id="id'.($j).($v).'" onClick="this.select();" onfocus="return saveReadingCNPrevious(\''.return_url().'\',\''.($j).($v++).'\');" type="text" class="number" value="'.$this->decimalFixer(@$n_rate_export_wh_5*$units).'" style="text-align:right; width:100%; padding:0 2px; margin:0;border:none;" /></td>';
							echo '<td style="border-right:2px solid black; text-align:right;"><input id="id'.($j).($v).'" onClick="this.select();" onfocus="return saveReadingCNPrevious(\''.return_url().'\',\''.($j).($v++).'\');" type="text" class="number" value="'.$this->decimalFixer(@$n_rate_export_wh_6*$units).'" style="text-align:right; width:100%; padding:0 2px; margin:0;border:none;" /></td>';

							echo '<td style="border-right:2px solid black; text-align:right;" class="yellow"><input id="id'.($j).($v).'" onClick="this.select();" onfocus="return saveReadingCNPrevious(\''.return_url().'\',\''.($j).($v++).'\');" type="text" class="number" value="'.$this->decimalFixer(@$n_rate_cum_imports*$units).'" style="text-align:right; width:100%; padding:0 2px; margin:0;border:none;" /></td>';
							echo '<td style="border-right:2px solid black; text-align:right;" class="yellow"><input id="id'.($j).($v).'" onClick="this.select();" onfocus="return saveReadingCNPrevious(\''.return_url().'\',\''.($j).($v++).'\');" type="text" class="number" value="'.$this->decimalFixer(@$n_rate_cum_exports*$units).'" style="text-align:right; width:100%; padding:0 2px; margin:0;border:none;" /></td>';

							if($j==1){
								echo '<td style="border-right:2px solid black; text-align:right;"><input id="id'.($j).($v).'" onClick="this.select();" onfocus="return saveReadingCNPrevious(\''.return_url().'\',\''.($j).($v++).'\');" type="date" class="dateFirst dateRow'.$j.'" value="'.(@$rate_date_read).'" style="text-align:right; width:100%; padding:0 2px; margin:0;border:none;" /></td>';
							}else{
								echo '<td style="border-right:2px solid black; text-align:right;"><input id="id'.($j).($v).'" onClick="this.select();" onchange="return saveReadingCNPrevious(\''.return_url().'\',\''.($j).($v++).'\');" onfocus="return saveReadingCNPrevious(\''.return_url().'\',\''.($j).($v++).'\');" type="date"  class="dateRow'.$j.'" value="'.(@$rate_date_taken).'" style="text-align:right; width:100%; padding:0 2px; margin:0;border:none;" /></td>';
							}

							if(!$this->isLocked($id, $year, $month)){
								echo '<td style="border-right:2px solid black;">';
								echo '<span title="Clear Values" class="btn-xs btn-danger clear-row" onclick="clearRow(\''.$j.'\')">CV</span>';
								echo '&nbsp;&nbsp;';
								$p = strtotime($year.'-'.$month.'-15');
								echo '<a href="'.return_url().'services/remove-mp-from-list/'.$mp_id.'/'.$p.'" title="Remove Metering Point ('.$mp_location.') from List" class="btn-xs btn-primary clear-row" onclick="return confirm(\'You really want to remove '.$mp_location.' from '.$year.'-'.month_name($month).' list?\n The metering point will be removed from the list and its readings.\')">Remove</a>';
								echo '</td>';							
							}else{
								echo '<td style="text-align:center; border-right:2px solid black;"><i class="fa fa-fx fa-lock" style="color:red"></i></td>';
							}
											
							echo '</tr>';
						}else{		

							echo '<tr>';

							echo '<td style="border-bottom:2px solid black; border-left:2px solid black; ">'.$count.'.</td>';
							echo '<td style="border-bottom:2px solid black; border-left:2px solid black; border-right:2px solid black;">'.strtoupper(strtolower($mp_location)).$this->mpAdvance($mp_advance).'<input type="hidden" id="mp'.$j.'" value="'.$mp_location.'"/></td>';
							
							if($j==1){
								echo '<td class="imports" style="border-bottom:2px solid black;  text-align:right;"><input '.$uu.' id="id'.($j).($v).'" onClick="this.select();" onfocus="return saveReadingCNPrevious(\''.return_url().'\',\''.($j).($v++).'\');" type="text" class="number" value="'.$this->decimalFixer(@$n_rate_import_wh_1*$units).'" style="text-align:right; width:100%; padding:0 2px; margin:0;border:none;" /></td>';
							}else{
								echo '<td class="imports" style="border-bottom:2px solid black;  text-align:right;"><input '.$uu.' id="id'.($j).($v).'" onClick="this.select();" onfocus="return saveReadingCNPrevious(\''.return_url().'\',\''.($j).($v++).'\');" type="text" class="number" value="'.$this->decimalFixer(@$n_rate_import_wh_1*$units).'" style="text-align:right; width:100%; padding:0 2px; margin:0;border:none;" /></td>';			
							}

							echo '<td class="imports" style="border-bottom:2px solid black;  text-align:right;"><input '.$uu.' id="id'.($j).($v).'" onClick="this.select();" onfocus="return saveReadingCNPrevious(\''.return_url().'\',\''.($j).($v++).'\');" type="text" class="number" value="'.$this->decimalFixer(@$n_rate_import_wh_2*$units).'" style="text-align:right; width:100%; padding:0 2px; margin:0;border:none;" /></td>';

							echo '<td class="imports" style="border-bottom:2px solid black; border-right:2px solid black; text-align:right;"><input '.$uu.' id="id'.($j).($v).'" onClick="this.select();" onfocus="return saveReadingCNPrevious(\''.return_url().'\',\''.($j).($v++).'\');" type="text" class="number" value="'.$this->decimalFixer(@$n_rate_import_wh_3*$units).'" style="text-align:right; width:100%; padding:0 2px; margin:0;border:none;" /></td>';

							echo '<td style="border-bottom:2px solid black;  text-align:right;"><input '.$uu.' id="id'.($j).($v).'" onClick="this.select();" onfocus="return saveReadingCNPrevious(\''.return_url().'\',\''.($j).($v++).'\');" type="text" class="number" value="'.$this->decimalFixer(@$n_rate_export_wh_4*$units).'" style="text-align:right; width:100%; padding:0 2px; margin:0;border:none;" /></td>';
							echo '<td style="border-bottom:2px solid black;  text-align:right;"><input '.$uu.' id="id'.($j).($v).'" onClick="this.select();" onfocus="return saveReadingCNPrevious(\''.return_url().'\',\''.($j).($v++).'\');" type="text" class="number" value="'.$this->decimalFixer(@$n_rate_export_wh_5*$units).'" style="text-align:right; width:100%; padding:0 2px; margin:0;border:none;" /></td>';
							echo '<td style="border-bottom:2px solid black; border-right:2px solid black; text-align:right;"><input '.$uu.' id="id'.($j).($v).'" onClick="this.select();" onfocus="return saveReadingCNPrevious(\''.return_url().'\',\''.($j).($v++).'\');" type="text" class="number" value="'.$this->decimalFixer(@$n_rate_export_wh_6*$units).'" style="text-align:right; width:100%; padding:0 2px; margin:0;border:none;" /></td>';


							echo '<td class="yellow" style="border-bottom:2px solid black; border-right:2px solid black; text-align:right;"><input '.$uu.' id="id'.($j).($v).'" onClick="this.select();" onfocus="return saveReadingCNPrevious(\''.return_url().'\',\''.($j).($v++).'\');" type="text" class="number" value="'.$this->decimalFixer(@$n_rate_cum_imports*$units).'" style="text-align:right; width:100%; padding:0 2px; margin:0;border:none;" /></td>';
							echo '<td class="yellow" style="border-bottom:2px solid black; border-right:2px solid black; text-align:right;"><input '.$uu.' id="id'.($j).($v).'" onClick="this.select();" onfocus="return saveReadingCNPrevious(\''.return_url().'\',\''.($j).($v++).'\');" type="text" class="number" value="'.$this->decimalFixer(@$n_rate_cum_exports*$units).'" style="text-align:right; width:100%; padding:0 2px; margin:0;border:none;" /></td>';
							echo '<td class="" style="border-bottom:2px solid black; border-right:2px solid black; text-align:right;"><input '.$uu.' id="id'.($j).($v).'" onClick="this.select();" onfocus="return saveReadingCNPrevious(\''.return_url().'\',\''.($j).($v++).'\');" onchange="return saveReadingCNPrevious(\''.return_url().'\',\''.($j).($v++).'\');" type="date" class="dateRow'.$j.'" value="'.($n_rate_date_read).'" style="text-align:right; width:100%; padding:0 2px; margin:0;border:none;" /></td>';

							if(!$this->isLocked($id, $year, $month)){
								echo '<td style="border-bottom:2px solid black; border-right:2px solid black;"><span class="btn-danger btn-xs clear-row" onclick="clearRow(\''.$j.'\')">CV</span></td>';
							}else{
								echo '<td style="text-align:center; border-bottom:2px solid black; border-right:2px solid black;"><i class="fa fa-fx fa-lock" style="color:red"></i></td>';
							}
											
							echo '</tr>';							

						}


						$count++;	

					}					
				}	

				echo '<input type="hidden" value="'.$j.'" id="allRows"/>';

				$select = $db->select("SELECT * FROM witness WHERE witness_customer_id = '$customer_id' AND witness_year = '$year' AND witness_month = '$month'");
				extract($select[0][0]);		
				?>
			</tbody></table>
		</div>

		<div id="xtext"></div>

			<table border="0" cellspacing="5" cellpadding="2">
				<tr>
					<td style="font-weight: bold;" colspan="5"><span style=""><u> WITNESS </u></span></td>
				</tr>
				<tr>
					<td style="font-weight: bold; text-align: center;"></td>
					<td style="font-weight: bold; text-align: center;">Firstname</td>
					<td style="font-weight: bold; text-align: center;">Othername</td>
					<td style="font-weight: bold; text-align: center;">Date</td>
					<td style="font-weight: bold; text-align: center;">Time</td>
				</tr>
				<tr>
					<td style="font-weight: bold;">1. </td>
					<td><input <?php echo $uu; ?> type="text" id="firstname1" value="<?php echo $witness_firstname1; ?>"/></td>
					<td><input <?php echo $uu; ?> type="text" id="othername1" value="<?php echo $witness_othername1; ?>"/></td>
					<td><input <?php echo $uu; ?> id="date1" type="date" value="<?php echo $witness_date1; ?>"/></td>
					<td><input <?php echo $uu; ?> type="time" id="time1" value="<?php echo $witness_time1; ?>"/></td>
				</tr>
				<tr>
					<td style="font-weight: bold;">2. </td>
					<td><input <?php echo $uu; ?> type="text" id="firstname2" value="<?php echo $witness_firstname2; ?>"/></td>
					<td><input <?php echo $uu; ?> type="text" id="othername2" value="<?php echo $witness_othername2; ?>"/></td>
					<td><input <?php echo $uu; ?>  id="date2" type="date" value="<?php echo $witness_date2; ?>"/></td>
					<td><input <?php echo $uu; ?> type="time" id="time2" value="<?php echo $witness_time2; ?>"/></td>
				</tr>
				<tr>
					<td style="font-weight: bold;">3. </td>
					<td><input <?php echo $uu; ?> type="text" id="firstname3" value="<?php echo $witness_firstname3; ?>"/></td>
					<td><input  <?php echo $uu; ?>type="text" id="othername3" value="<?php echo $witness_othername3; ?>"/></td>
					<td><input <?php echo $uu; ?>  id="date3" type="date" value="<?php echo $witness_date3; ?>"/></td>
					<td><input <?php echo $uu; ?> type="time" id="time3" value="<?php echo $witness_time3; ?>"/></td>
				</tr>
			</table>

		</div>
		<script type="text/javascript">

			$('.dateFirst').change(function(){
				var dates = $('.date');
				// for(var i=1; i <= $('#allRows').val(); i++){ 
				// 	$('.dateRow'+i).val($('.dateFirst').val());
				// }
			});

			$('#date1').change(function(){
				if($('#date2').val()=="" || $('#date2').val()==null){
					$('#date2').val($('#date1').val());
				}
				if($('#date3').val()=="" || $('#date3').val()==null){
					$('#date3').val($('#date1').val());
				}
			});
			$('#time1').change(function(){
				if($('#time2').val()=="" || $('#time2').val()==null){
					$('#time2').val($('#time1').val());
				}
				if($('#time3').val()=="" || $('#time3').val()==null){
					$('#time3').val($('#time1').val());
				}
			});
		</script>
	</div>
	<?php

	}//close what is not tanseco
	}

	

	public function oldeditingModeAction(){


		$id = portion(3);
		$year = portion(4);
		$month = portion(5);

		$db = new Db();
		$select = $db->select("SELECT * FROM customer WHERE customer_id = '$id'");
		extract($select[0][0]);
	
	?>
	<div id="side">
		<?php 
		$units = $this->customerAndSchedule($id, $year, $month);
		$this->energyLinks($id, $year, $month);

		?>
		
		<div id="printMe">
			<style>table tr td, table tr th{ height: 10px !important;} table tr th{ text-align: center;} table tr:hover td{ background-color: #efefff; }</style>
			<center class="hide"><h3>UGANDA ELECTRICITY TRANSMISSION COMPANY LTD</h3></center>
			<table border="1" cellspacing="0" cellpadding="2" class="table schedule" style="font-size: 12px; margin:auto;">
				<tbody>
				<tr valign="bottom">
					<th colspan="19" style="border:2px solid black;">
					<?php
					echo strtoupper($customer_full_name.' Meter Readings');
					echo '<br/>';
					echo $year = portion(4);
					echo ' - ';
					echo month_name($month = portion(5));

					?>
					</th>				
				</tr>
				<tr valign="bottom">
					<th rowspan="3" style="border-left:2px solid black;border-bottom:2px solid black;">No.</th>
					<th rowspan="3" style="border:2px solid black;">Metering Points</th>
					<th colspan="3" style="border:2px solid black;">Main Meter - Current Readings 
					<?php
					echo '(';
					if($units == 1){
						echo 'Wh';
					}elseif($units == 0.001){
						echo 'KWh';
					}elseif($units == 0.000001){
						echo 'MWh';
					}
					echo ')';
					?>
					</th>					
					<th colspan="3" style="border:2px solid black;color:red;">Main Meter - Previous Readings</th>
					<th colspan="3" style="border:2px solid black;">Main Meter - Current Readings</th>
					<th style="border:2px solid black;" colspan="8" style="border:2px solid black;">Advance</th>				
				</tr>
				<tr>
					<th style="border:2px solid black;" colspan="3">Imports</th>
					<th style="border:2px solid black; color:red;" colspan="3">Imports</th>
					<th style="border:2px solid black;" colspan="3">Exports</th>
					<th style="border:2px solid black;" colspan="4">Imports</th>
					<th style="border:2px solid black;" colspan="4">Exports</th>				
				</tr>
				<tr>
					<th style="border-left:2px solid black;border-bottom:2px solid black;">Shoulder</th>
					<th style="border-bottom:2px solid black;">Peak</th>
					<th style="border-bottom:2px solid black;">Off Peak</th>

					<th style="border-left:2px solid black;border-bottom:2px solid black;color:red;">Shoulder</th>
					<th style="border-bottom:2px solid black;color:red;">Peak</th>
					<th style="border-bottom:2px solid black;color:red;">Off Peak</th>

					<th style="border-left:2px solid black;border-bottom:2px solid black;">Shoulder</th>
					<th style="border-bottom:2px solid black;">Peak</th>
					<th style="border-bottom:2px solid black;">Off Peak</th>
					<th style="border-left:2px solid black;border-bottom:2px solid black;">Shoulder</th>
					<th style="border-bottom:2px solid black;">Peak</th>
					<th style="border-bottom:2px solid black;">Off Peak</th>
					<th style="border-right:2px solid black;border-bottom:2px solid black;">Total</th>
					<th style="border-left:2px solid black;border-bottom:2px solid black;">Shoulder</th>
					<th style="border-bottom:2px solid black;">Peak</th>
					<th style="border-bottom:2px solid black;">Off Peak</th>
					<th style="border-right:2px solid black;border-bottom:2px solid black;">Total</th>				
				</tr>
				<?php 
				$count = 1;
				$db = new Db();
				$select = $db->select("SELECT * FROM metering_point, r_reading WHERE mp_customer_id = '$customer_id' AND rea_cus_id = '$customer_id' AND mp_id = rea_mp_id");

				$total_mp = 0;
				foreach($select[0] as $row){
					extract($row);
					if(date('Y', $rea_date)==$year && date('m', $rea_date)==$month){
						$total_mp++;
					}
				}
				//$units = 0.001;

				$tc1 = $tc2 = $tc3 = $tc4= $tc5= $tc6= $tc7= $tc8 = 0;

				$total_mp;
				$db = new Db();
				$select = $db->select("SELECT * FROM metering_point, r_reading WHERE mp_customer_id = '$customer_id' AND rea_cus_id = '$customer_id' AND mp_id = rea_mp_id");
				
				foreach($select[0] as $row){
					extract($row);

					if(date('Y', $rea_date)==$year && date('m', $rea_date)==$month){
						//selecting rates
						
						$db = new Db();
						$sel = $db->select("SELECT * FROM r_rate WHERE rate_reading_id = '$rea_id'");
						@extract($sel[0][0]);
						
						//calling the function
						$cal = rate_calculate($customer_id, ($rea_date), $mp_id);
						
						if($count != $total_mp){
							echo '<tr>';

							echo '<td style="border-left:2px solid black; ">'.$count.'</td>';
							echo '<td style="border-left:2px solid black; border-right:2px solid black;">'.$mp_location.' <br/><a href="'.return_url().'services/swap/'.$mp_id.'/'.$customer_id.'/'.$year.'/'.$month.'">Swap</a></td>';

							echo '<td style=" text-align:right;">'.number_format(@$rate_import_wh_1*$units,2).'<br/><a href="'.return_url().'services/edit-invoice-reading/'.$mp_id.'/'.$customer_id.'/'.$year.'/'.$month.'/1">Edit</a></td>';
							echo '<td style=" text-align:right;">'.number_format(@$rate_import_wh_2*$units,2).'<br/><a href="'.return_url().'services/edit-invoice-reading/'.$mp_id.'/'.$customer_id.'/'.$year.'/'.$month.'/2">Edit</a></td>';
							echo '<td style="border-right:2px solid black; text-align:right;">'.number_format(@$rate_import_wh_3*$units,2).'<br/><a href="'.return_url().'services/edit-invoice-reading/'.$mp_id.'/'.$customer_id.'/'.$year.'/'.$month.'/3">Edit</a></td>';

							if($month == 12){
								$y = $year-1;
								$m = 1;
							}else{
								$m = $month-1;
								$y = $year;
							}

							echo '<td style=" text-align:right;color:red;">'.number_format(@$cal[12]*$units,2).'<br/><a href="'.return_url().'services/swap/'.$mp_id.'/'.$customer_id.'/'.$y.'/'.$m.'/'.$month.'">Swap</a></td>';
							echo '<td style=" text-align:right;color:red;">'.number_format(@$cal[13]*$units,2).'</td>';
							echo '<td style="border-right:2px solid black; text-align:right;color:red;">'.number_format(@$cal[14]*$units,2).'</td>';


							echo '<td style=" text-align:right;">'.number_format(@$rate_export_wh_4*$units,2).'<br/><a href="'.return_url().'services/edit-invoice-reading/'.$mp_id.'/'.$customer_id.'/'.$year.'/'.$month.'/4">Edit</a></td>';
							echo '<td style=" text-align:right;">'.number_format(@$rate_export_wh_5*$units,2).'<br/><a href="'.return_url().'services/edit-invoice-reading/'.$mp_id.'/'.$customer_id.'/'.$year.'/'.$month.'/5">Edit</a></td>';
							echo '<td style=" text-align:right;">'.number_format(@$rate_export_wh_6*$units,2).'<br/><a href="'.return_url().'services/edit-invoice-reading/'.$mp_id.'/'.$customer_id.'/'.$year.'/'.$month.'/6">Edit</a></td>';
							echo '<td style="border-left:2px solid black; text-align:right;">'.number_format($cal[0]*$units,2).'</td>';
							echo '<td style=" text-align:right;">'.number_format($cal[1]*$units,2).'</td>';
							echo '<td style=" text-align:right;">'.number_format($cal[2]*$units,2).'</td>';
							echo '<td style="border-right:2px solid black; text-align:right;">'.number_format($cal[3]*$units,2).'</td>';

							echo '<td style="border-left:2px solid black; text-align:right;">'.number_format($cal[4]*$units,2).'</td>';
							echo '<td style=" text-align:right;">'.number_format($cal[5]*$units,2).'</td>';
							echo '<td style=" text-align:right;">'.number_format($cal[6]*$units,2).'</td>';
							echo '<td style="border-right:2px solid black; text-align:right;">'.number_format($cal[7]*$units,2).'</td>';				
							echo '</tr>';

							$tc1 += $cal[0];
							$tc2 += $cal[1];
							$tc3 += $cal[2];
							$tc4 += $cal[3];
							$tc5 += $cal[4];
							$tc6 += $cal[5];
							$tc7 += $cal[6];
							$tc8 += $cal[7];
						}else{			

							echo '<tr>';
							echo '<td style="border-left:2px solid black; border-bottom:2px solid black; ">'.$count.'</td>';
							echo '<td style="border-left:2px solid black; border-bottom:2px solid black; ">'.$mp_location.'<br/><a href="'.return_url().'services/swap/'.$mp_id.'/'.$customer_id.'/'.$year.'/'.$month.'">Swap</a></td>';


							echo '<td style="border-left:2px solid black; border-bottom:2px solid black;  text-align:right;">'.number_format(@$rate_import_wh_1*$units,2).'<br/><a href="'.return_url().'services/edit-invoice-reading/'.$mp_id.'/'.$customer_id.'/'.$year.'/'.$month.'/1">Edit</a></td>';
							echo '<td style="border-bottom:2px solid black;  text-align:right;">'.number_format(@$rate_import_wh_2*$units,2).'<br/><a href="'.return_url().'services/edit-invoice-reading/'.$mp_id.'/'.$customer_id.'/'.$year.'/'.$month.'/2">Edit</a></td>';
							echo '<td style="border-bottom:2px solid black;  text-align:right;">'.number_format(@$rate_import_wh_3*$units,2).'<br/><a href="'.return_url().'services/edit-invoice-reading/'.$mp_id.'/'.$customer_id.'/'.$year.'/'.$month.'/3">Edit</a></td>';



							echo '<td style="border-left:2px solid black; border-bottom:2px solid black;  text-align:right;color:red;">'.number_format(@$cal[12]*$units,2).'<br/><a href="'.return_url().'services/swap/'.$mp_id.'/'.$customer_id.'/'.$y.'/'.$m.'/'.$month.'">Swap</a></td>';
							echo '<td style="border-bottom:2px solid black;  text-align:right;color:red;">'.number_format(@$cal[13]*$units,2).'</td>';
							echo '<td style="border-bottom:2px solid black;  text-align:right;color:red;">'.number_format(@$cal[14]*$units,2).'</td>';



							echo '<td style="border-left:2px solid black; border-bottom:2px solid black;  text-align:right;">'.number_format(@$rate_export_wh_4*$units,2).'<br/><a href="'.return_url().'services/edit-invoice-reading/'.$mp_id.'/'.$customer_id.'/'.$year.'/'.$month.'/4">Edit</a></td>';
							echo '<td style="border-bottom:2px solid black;  text-align:right;">'.number_format(@$rate_export_wh_5*$units,2).'<br/><a href="'.return_url().'services/edit-invoice-reading/'.$mp_id.'/'.$customer_id.'/'.$year.'/'.$month.'/5">Edit</a></td>';
							echo '<td style="border-bottom:2px solid black; text-align:right;">'.number_format(@$rate_export_wh_6*$units,2).'<br/><a href="'.return_url().'services/edit-invoice-reading/'.$mp_id.'/'.$customer_id.'/'.$year.'/'.$month.'/6">Edit</a></td>';


							echo '<td style="border-left:2px solid black; border-bottom:2px solid black;  text-align:right;">'.number_format($cal[0]*$units,2).'</td>';
							echo '<td style="border-bottom:2px solid black;  text-align:right;">'.number_format($cal[1]*$units,2).'</td>';
							echo '<td style="border-bottom:2px solid black;  text-align:right;">'.number_format($cal[2]*$units,2).'</td>';
							echo '<td style="border-right:2px solid black; border-bottom:2px solid black; text-align:right; ">'.number_format($cal[3]*$units,2).'</td>';


							echo '<td style="border-left:2px solid black; border-bottom:2px solid black;  text-align:right;">'.number_format($cal[4]*$units).'</td>';
							echo '<td style="border-bottom:2px solid black;  text-align:right;">'.number_format($cal[5]*$units).'</td>';
							echo '<td style="border-bottom:2px solid black;  text-align:right;">'.number_format($cal[6]*$units).'</td>';
							echo '<td style="border-right:2px solid black; border-bottom:2px solid black; text-align:right; ">'.number_format($cal[7]*$units).'</td>';

							echo '</tr>';

							$tc1 += $cal[0];
							$tc2 += $cal[1];
							$tc3 += $cal[2];
							$tc4 += $cal[3];
							$tc5 += $cal[4];
							$tc6 += $cal[5];
							$tc7 += $cal[6];
							$tc8 += $cal[7];
						}


						$count++;	

					}					
				}

				echo '<tr>';
				echo '<td colspan="10" style="border-left:2px solid black; border-bottom:2px solid black; border-right:2px solid black; text-align:right;font-weight: bold; ">Total</td>';
				
				echo '<td style="border-bottom:2px solid black;  text-align:right;font-weight: bold;">'.number_format($tc1*$units,2).'</td>';
				echo '<td style="border-bottom:2px solid black;  text-align:right;font-weight: bold;">'.number_format($tc2*$units,2).'</td>';
				echo '<td style="border-bottom:2px solid black;  text-align:right;font-weight: bold;">'.number_format($tc3*$units,2).'</td>';
				echo '<td style="border-right:2px solid black; border-bottom:2px solid black; text-align:right; font-weight: bold; ">'.number_format($tc4*$units,2).'</td>';


				echo '<td style="border-left:2px solid black; border-bottom:2px solid black;  text-align:right;">'.number_format($tc5*$units,2).'</td>';
				echo '<td style="border-bottom:2px solid black;  text-align:right;">'.number_format($tc6*$units,2).'</td>';
				echo '<td style="border-bottom:2px solid black;  text-align:right;">'.number_format($tc7*$units,2).'</td>';
				echo '<td style="border-right:2px solid black; border-bottom:2px solid black; text-align:right; ">'.number_format($tc8*$units,2).'</td>';

				echo '</tr>';
				?>
			</tbody></table>

			<?php 
			/*
			echo '<pre>';
				print_r($this->sumFormulae($customer_id, $year, $month));
			echo '</pre>';
			*/
			?>

		</div>
	
	</div>
	<?php
	}

	public function debitNoteAction(){
		$id = portion(3);
		$month = portion(4);
		$year = portion(5);

		echo "<h4>Debit Note for:<span style='color:red;'> ".$this->rgf("customer", $id, "customer_id", "customer_full_name")."</span>";
		echo "<br/><br/>Invoice: <span style='color:red;'>".month_name($month)." $year</span></h4>";

		$id = portion(3);
		$db = new Db();
		$select = $db->select("SELECT TOP 1 * FROM time_band WHERE tb_customer_id = '$id' ORDER BY tb_id DESC");
		extract($select[0][0]);

		$peak = $tb_peak;
		$off_peak = $tb_off_peak;
		$shoulder = $tb_shoulder;

		if(isset($_POST['submit'])){
			$peak = $_POST['peak'];
			$off_peak = $_POST['off_peak'];
			$shoulder = $_POST['shoulder'];
			$debit_peak = $_POST['debit_peak'];
			$debit_off_peak = $_POST['debit_off_peak'];
			$debit_shoulder = $_POST['debit_shoulder'];
			$wheeling_charge = $_POST['wheeling_charge'];
			$details = $_POST['details'];
			$shoulder = $_POST['shoulder'];

			$errors = array();

			if(empty($peak)){
				$errors[] = "Enter Peak Rate";
			}
			if(empty($off_peak)){
				$errors[] = "Enter Off Peak Rate";
			}
			if(empty($shoulder)){
				$errors[] = "Enter Shoulder Rate";
			}

			if(empty($debit_peak)){
				$errors[] = "Enter Peak Debit Units";
			}
			if(empty($debit_off_peak)){
				$errors[] = "Enter Off Peak Debit Units";
			}
			if(empty($debit_shoulder)){
				$errors[] = "Enter Shoulder Debit Units";
			}

			if(empty($wheeling_charge)){
				$errors[] = "Enter Wheeling Charge";
			}

			if(empty($details)){
				$errors[] = "Enter Details";
			}

			if(empty($errors)){
				$db = new Db();
				$select = $db->insert("debit_note", [
					"dn_customer_id"=>$id,
					"dn_year"=>$year,
					"dn_month"=>$month,
					"dn_peak"=>$peak,
					"dn_off_peak"=>$off_peak,
					"dn_shoulder"=>$shoulder,
					"dn_bit_peak"=>$debit_peak,
					"dn_bit_off_peak"=>$debit_off_peak,
					"dn_bit_shoulder"=>$debit_shoulder,
					"dn_wheeling_charge"=>$wheeling_charge,
					"dn_details"=>$details,
					"dn_added_by"=>user_id(),
					"dn_date_added"=>time(),
				]);

				if(empty($db->error())){
					FeedBack::success();
					FeedBack::refresh(3, return_url().'services/view-invoice/'.$id.'/'.$month.'/'.$year);
				}else{
					FeedBack::error("Error: ".$db->error());
				}
			}else{
				FeedBack::errors($errors);
			}
		}

		?>
		
		<form action="" method="post">
			<div class="col-lg-6">
				<div class="col-lg-12">
					<h5 style="color:blue;">Rates</h3>
				</div>
				<div class="col-lg-12">
					<div class="row">
						
						<div class="col-lg-4">
							<div class="form-group">
								<label>Peak<span class="must">*</span></label>
								<div class="form-line">
									<input type="number" min = "0" step="any" value="<?php echo $peak; ?>" class="form-control" name="peak">
								</div>
							</div>
						</div>
						<div class="col-lg-4">
							<div class="form-group">
								<label>Shoulder<span class="must">*</span></label>
								<div class="form-line">
									<input type="number" min = "0" step="any" value="<?php echo $shoulder; ?>" class="form-control" name="shoulder">
								</div>
							</div>
						</div>
						<div class="col-lg-4">
							<div class="form-group">
								<label>Off Peak<span class="must">*</span></label>
								<div class="form-line">
									<input type="number" min = "0" step="any" value="<?php echo $off_peak; ?>" class="form-control" name="off_peak">
								</div>
							</div>
						</div>
					</div>
				</div>

				<div class="col-lg-12">
					<h5 style="color:blue;">Debit Units</h3>
				</div>
				<div class="col-lg-12">
					<div class="row">					
						<div class="col-lg-4">
							<div class="form-group">
								<label>Peak<span class="must">*</span></label>
								<div class="form-line">
									<input type="number" min = "0" step="any" value="<?php echo $debit_peak; ?>" class="form-control" name="debit_peak">
								</div>
							</div>
						</div>
						<div class="col-lg-4">
							<div class="form-group">
								<label>Shoulder<span class="must">*</span></label>
								<div class="form-line">
									<input type="number" min = "0" step="any" value="<?php echo $debit_shoulder; ?>" class="form-control" name="debit_shoulder">
								</div>
							</div>
						</div>
						<div class="col-lg-4">
							<div class="form-group">
								<label>Off Peak<span class="must">*</span></label>
								<div class="form-line">
									<input type="number" min = "0" step="any" value="<?php echo $debit_off_peak; ?>" class="form-control" name="debit_off_peak">
								</div>
							</div>
						</div>
					</div>
				</div>
			</div>
			<div class="col-lg-6">
				<br/>
				<div class="col-lg-12">
					<h5 style="color:blue;">Wheeling Charge(%)</h3>
				</div>
				<div class="col-lg-12">
					<input type="number" min = "0" step="any" value="<?php echo $wheeling_charge; ?>" name="wheeling_charge">
				</div>
				<div class="col-lg-12">
					<BR/>
					<h5 style="color:blue;">Details</h3>
				</div>
				<div class="col-lg-12">
					<textarea name="details" style="width:100%; height: 100px;"><?php echo $details; ?></textarea>
				</div>
			</div>

			<div class="col-lg-12">
				<button onclick = "" type="submit" name="submit" class="btn btn-primary"><i class="fa fa-fw fa-save"></i> Save</button>
			</div>
		</form>

		<?php
	}

	
	public function creditNoteOldAction(){
		$id = portion(3);
		$month = portion(4);
		$year = portion(5);

		echo "<h4>Credit Note for:<span style='color:red;'> ".$this->rgf("customer", $id, "customer_id", "customer_full_name")."</span>";
		echo "<br/><br/>Invoice: <span style='color:red;'>".month_name($month)." $year</span></h4>";

		$id = portion(3);
		$db = new Db();
		$select = $db->select("SELECT TOP 1 * FROM time_band WHERE tb_customer_id = '$id' ORDER BY tb_id DESC");
		extract($select[0][0]);

		$peak = $tb_peak;
		$off_peak = $tb_off_peak;
		$shoulder = $tb_shoulder;

		if(isset($_POST['submit'])){
			$peak = $_POST['peak'];
			$off_peak = $_POST['off_peak'];
			$shoulder = $_POST['shoulder'];
			$debit_peak = $_POST['debit_peak'];
			$debit_off_peak = $_POST['debit_off_peak'];
			$debit_shoulder = $_POST['debit_shoulder'];
			$wheeling_charge = $_POST['wheeling_charge'];
			$details = $_POST['details'];
			$shoulder = $_POST['shoulder'];

			$errors = array();

			if(empty($peak)){
				$errors[] = "Enter Peak Rate";
			}
			if(empty($off_peak)){
				$errors[] = "Enter Off Peak Rate";
			}
			if(empty($shoulder)){
				$errors[] = "Enter Shoulder Rate";
			}

			if(empty($debit_peak)){
				$errors[] = "Enter Peak Credit Units";
			}
			if(empty($debit_off_peak)){
				$errors[] = "Enter Off Peak Credit Units";
			}
			if(empty($debit_shoulder)){
				$errors[] = "Enter Shoulder Credit Units";
			}

			if(empty($wheeling_charge)){
				$errors[] = "Enter Wheeling Charge";
			}

			if(empty($details)){
				$errors[] = "Enter Details";
			}

			if(empty($errors)){
				$db = new Db();
				$select = $db->insert("credit_note", [
					"cn_customer_id"=>$id,
					"cn_year"=>$year,
					"cn_month"=>$month,
					"cn_peak"=>$peak,
					"cn_off_peak"=>$off_peak,
					"cn_shoulder"=>$shoulder,
					"cn_bit_peak"=>$debit_peak,
					"cn_bit_off_peak"=>$debit_off_peak,
					"cn_bit_shoulder"=>$debit_shoulder,
					"cn_wheeling_charge"=>$wheeling_charge,
					"cn_details"=>$details,
					"cn_added_by"=>user_id(),
					"cn_date_added"=>time(),
				]);

				if(empty($db->error())){
					FeedBack::success();
					FeedBack::refresh(3, return_url().'services/view-invoice/'.$id.'/'.$month.'/'.$year);
				}else{
					FeedBack::error("Error: ".$db->error());
				}
			}else{
				FeedBack::errors($errors);
			}
		}

		?>
		
		<form action="" method="post">
			<div class="col-lg-6">
				<div class="col-lg-12">
					<h5 style="color:blue;">Rates</h3>
				</div>
				<div class="col-lg-12">
					<div class="row">
						
						<div class="col-lg-4">
							<div class="form-group">
								<label>Peak<span class="must">*</span></label>
								<div class="form-line">
									<input type="number" min = "0" step="any" value="<?php echo $peak; ?>" class="form-control" name="peak">
								</div>
							</div>
						</div>
						<div class="col-lg-4">
							<div class="form-group">
								<label>Shoulder<span class="must">*</span></label>
								<div class="form-line">
									<input type="number" min = "0" step="any" value="<?php echo $shoulder; ?>" class="form-control" name="shoulder">
								</div>
							</div>
						</div>
						<div class="col-lg-4">
							<div class="form-group">
								<label>Off Peak<span class="must">*</span></label>
								<div class="form-line">
									<input type="number" min = "0" step="any" value="<?php echo $off_peak; ?>" class="form-control" name="off_peak">
								</div>
							</div>
						</div>
					</div>
				</div>

				<div class="col-lg-12">
					<h5 style="color:blue;">Credit Units</h3>
				</div>
				<div class="col-lg-12">
					<div class="row">					
						<div class="col-lg-4">
							<div class="form-group">
								<label>Peak<span class="must">*</span></label>
								<div class="form-line">
									<input type="number" min = "0" step="any" value="<?php echo $debit_peak; ?>" class="form-control" name="debit_peak">
								</div>
							</div>
						</div>
						<div class="col-lg-4">
							<div class="form-group">
								<label>Shoulder<span class="must">*</span></label>
								<div class="form-line">
									<input type="number" min = "0" step="any" value="<?php echo $debit_shoulder; ?>" class="form-control" name="debit_shoulder">
								</div>
							</div>
						</div>
						<div class="col-lg-4">
							<div class="form-group">
								<label>Off Peak<span class="must">*</span></label>
								<div class="form-line">
									<input type="number" min = "0" step="any" value="<?php echo $debit_off_peak; ?>" class="form-control" name="debit_off_peak">
								</div>
							</div>
						</div>
					</div>
				</div>
			</div>
			<div class="col-lg-6">
				<br/>
				<div class="col-lg-12">
					<h5 style="color:blue;">Wheeling Charge(%)</h3>
				</div>
				<div class="col-lg-12">
					<input type="number" min = "0" step="any" value="<?php echo $wheeling_charge; ?>" name="wheeling_charge">
				</div>
				<div class="col-lg-12">
					<BR/>
					<h5 style="color:blue;">Details</h3>
				</div>
				<div class="col-lg-12">
					<textarea name="details" style="width:100%; height: 100px;"><?php echo $details; ?></textarea>
				</div>
			</div>

			<div class="col-lg-12">
				<button onclick = "" type="submit" name="submit" class="btn btn-primary"><i class="fa fa-fw fa-save"></i> Save</button>
			</div>
		</form>

		<?php
	}

	public function debitNoteDetailsAction(){
		$id = portion(3);
		$year = portion(5);
		$month = portion(4);
		$db = new Db();
		$select = $db->select("SELECT * FROM debit_note WHERE dn_id = '$id'");
		extract($select[0][0]);

		$customer_id = $dn_customer_id;
		$year = $dn_year;
		$month = $dn_month;

		echo '<Br/><form action="" method="post" class="pull-left">';
		if(isset($_POST['change'])){
			$units = $_POST['rate'];
		}else{
			$units = 0.001;
		}
		echo 'Change Units:';
		echo '<select name="rate">';
		echo '<option value="1">Wh</option>';
		echo '<option value="0.001">KWh</option>';
		echo '<option value="0.000001">MWh</option>';
		echo '</select>';
		echo ' &nbsp; <button name="change">Change</button>';
		echo '</form>';

		$db = new Db();
		$select = $db->select("SELECT dn_id FROM debit_note WHERE dn_year='$year' AND dn_month='$month' AND dn_customer_id='$customer_id'");

		echo '<div class="nav files pull-right">';
		if($db->num_rows()){
			extract($select[0][0]);
			echo '<b>Debit Note:</b> <a href="'.return_url().'services/debit-note-details/'.$dn_id.'">';
			echo $this->rgf("customer", $customer_id, "customer_id", "customer_short_name");
			echo '/DN/'.str_pad($dn_id, 3, '0', STR_PAD_LEFT).'/'.$year;
			echo '</a>';
			echo '&nbsp;  &nbsp;'; 

		}else{
			
			echo '<a class="btn btn-warning btn-xs" href="'.return_url().'services/view-invoice/'.$customer_id.'/'.$month.'/'.$year.'"><i class="fa fa-level-up fa-fw"></i> Raise Debit Note</a>';
		}
		echo '&nbsp;  &nbsp;'; 

		$db = new Db();
		$select = $db->select("SELECT cn_id FROM credit_note WHERE cn_year='$year' AND cn_month='$month' AND cn_customer_id='$customer_id'");

		
		if($db->num_rows()){
			extract($select[0][0]);
			echo '<b>Credit Note:</b> <a href="'.return_url().'services/credit-note-details/'.$cn_id.'">';
			echo $this->rgf("customer", $customer_id, "customer_id", "customer_short_name");
			echo '/DN/'.str_pad($cn_id, 3, '0', STR_PAD_LEFT).'/'.$year;
			echo '</a>';
		}else{
			
			echo '<a class="btn btn-warning btn-xs" href="'.return_url().'services/credit-note/'.$customer_id.'/'.$month.'/'.$year.'"><i class="fa fa-level-up fa-fw"></i> Raise Credit Note</a>';
		}	
		echo '&nbsp;  &nbsp;';
		echo '<a class="btn btn-warning btn-xs" href="'.return_url().'services/view-invoice/'.$customer_id.'/'.$month.'/'.$year.'"><i class="fa fa-table fa-fw"></i> Invoice</a>';
		echo '&nbsp;  &nbsp;';
		echo '<a class="btn btn-success btn-xs" href="" onfocus="return print(\'printMe\');"><i class="fa fa-print fa-fw"></i> Print Debit Note</a>	
		</div>';
		echo '<div class="clearfix"></div> ';
		echo '<br/><br/>';
		?>
		<style>
		table tr th{ background-color:#ccc; color:black; }
		
		.table tr:hover{
			background-color:none;
			background:none !important;
		}
		#y>tbody>tr>td{ padding:5px; background-color:#fbfbfb; border:1px solid #000; }
		</style>
		<div id="printMe" style="width:900px;margin:auto;">
			<table border="0" cellspacing="0" cellpadding="2" id="y">
				<tbody>
				<tr valign="bottom">
					<td colspan="11">
					<img src="<?php display_url();?>images/uetcl_header.png" width="100%">
					<center><h2 style="color:blue; margin: 5px 0 0 0; padding:0">TAX INVOICE</h2></center>
						<!-- <hr style="border:1px solid black;"/> -->
						
						<p align="right" style="font-weight: bold; color:red;">Debit Note:
							<?php  
							echo $this->rgf("customer", $customer_id, "customer_id", "customer_short_name");
			echo '/DN/'.str_pad($dn_id, 3, '0', STR_PAD_LEFT).'/'.$year;
							?></p>
						<table style="" class="" cellspacing="10">
							
							<tr>
								<td>Date:</td>
								<td><?php echo date('dS F Y h:i:s a'); ?></td>
							</tr>
							<tr valign="top">
								<td width="35%">Name of Customer:</td>
								<td>
								<?php
									$select = $db->select("SELECT * FROM customer WHERE customer_id = '$customer_id'");
									extract($select[0][0]);
									echo '<b>'.$customer_full_name.'</b>';
									echo '<br/>';
									echo '<span style="font-size: 12px;">';
									if(empty($customer_street)){
										echo nl2br($customer_address);
									}else{
										echo nl2br($customer_street).'<br/>';
										echo nl2br($customer_postal_code).'<br/>';
										echo nl2br($customer_tel).'<br/>';
										echo nl2br($customer_email).'<br/>';
									}
									echo '</span>';
									echo '<br/><br/>';
								?>								
								</td>
							</tr>
							<tr>
								<td>Detail:</td>
								<td><u><b>Debit Note - <?php 
								echo month_name($month);
								echo ' ';
								echo $year;
								?></b></u><br/>
								<?php echo nl2br($dn_details); ?>
							</td>
							</tr>
						</table>
						
						<?php
										

						$t1 = $t2 = $t3 = $t4 = 0;							
							
						$t1 += $dn_bit_peak*$units;
						$t2 += $dn_bit_shoulder*$units;
						$t3 += $dn_bit_off_peak*$units;
						$t4 += ($dn_bit_peak+$dn_bit_shoulder+$dn_bit_off_peak)*$units;	
						

						$ttu = $tta = 0;
						?>
						<style>
							#x tr td, #x tr th{
								padding:10px;
							}
						</style>
						<table cellpadding="5" cellspacing="0" border="1" width="100%" id="x">
							<tr>
								<th>TIME BAND</th>
								<th>RATE</th>
								<th>QTY - 
									<?php
									echo '(';
									if($units == 1){
										echo 'Wh';
									}elseif($units == 0.001){
										echo 'KWh';
									}elseif($units == 0.000001){
										echo 'MWh';
									}
									echo ')';
									?>
								</th>
								<th>AMOUNT</th>
							</tr>
							<tr>
								<?php 
								$o = $dn_peak;
								$ttu += $t1;
								$tta += $o*$t1;
								?>
								<td>Peak</td>
								<td align="right"><?php echo number_format($o); ?></td>
								<td align="right"><?php echo number_format($t1); ?></td>
								<td align="right"><?php echo number_format($o*$t1); ?></td>
							</tr>
							<tr>
								<?php 
								$o = $dn_shoulder;
								$ttu += $t2;
								$tta += $o*$t2;
								?>
								<td>Shoulder</td>
								<td align="right"><?php echo number_format($o); ?></td>
								<td align="right"><?php echo number_format($t2); ?></td>
								<td align="right"><?php echo number_format($o*$t2); ?></td>
							</tr>
							<tr>
								<td>Off Peak</td>
								<?php 
								$o = $dn_off_peak;
								$ttu += $t3;
								$tta += $o*$t3;
								?>
								<td align="right"><?php echo number_format($o); ?></td>
								<td align="right"><?php echo number_format($t3); ?></td>
								<td align="right"><?php echo number_format($o*$t3); ?></td>
							</tr>
							<tr>
								<td>Total</td>
								<td align="right"><?php echo number_format($ttu); ?></td>
								<td></td>
								<td align="right"><?php echo number_format($tta); ?></td>
							</tr>
							<tr>
								<td colspan="3">Wheeling Charges(<?php echo $dn_wheeling_charge; ?>%)</td>
								<td align="right"><?php echo number_format($tta*$dn_wheeling_charge/100); ?></td>
							</tr>
							<?php $tta += $tta*$dn_wheeling_charge/100; ?>
							<tr>
								<td colspan="3">Total (VAT Exclusive)</td>
								<td align="right"><?php echo number_format($tta); ?></td>
							</tr>							
							<tr>
								<td colspan="3">Add 18% VAT</td>
								<?php $tot = 18/100*$tta; ?>
								<td align="right"><?php echo number_format($tot); ?></td>
							</tr>							
							<tr>
								<td colspan="3">Total Amount (VAT Incl)</td>
								<?php $tot = 18/100*$tta; ?>
								<td align="right"><?php echo number_format($tta*(18/100+1)); ?></td>							
							<tr>
								<td colspan="4">
								<?php $tot = $tta*(18/100+1); if($tot<200000000000){?>
								Total in words:<br/><?php echo '<b>'.number_to_words($tta*(18/100+1)).'</b>'; ?></td>
								<?php } ?>
							</tr>
							<?php if($oi_exchange_rate && $currency =='Dollars'){?>
							<tr>
							<td colspan="4">
								Exhange Rate:
								<?php 
								//echo ;
									echo number_format($oi_exchange_rate,2);
								echo " $currency".''; 
								?>
								</td>
							</tr>
						<?php }?>
						</table>
						<span style="color:red; font-weight: bold; font-size: 12px; text-align: center; ">Accounts are due within 45days of receipt of this invoice</span>				

						<br/><br/>
						<br/><br/>
						Prepared by: ......................... 
						<?php for($i=0; $i<20; $i++) echo ' &nbsp ';  ?>
						Approved By: .........................
					</td>				
				</tr>
				
			</tbody></table>
		</div>
	
	</div>
	<?php
	}
	
	

	public function creditNoteDetailsAction(){
		$id = portion(3);
		$db = new Db();
		$select = $db->select("SELECT * FROM credit_note WHERE cn_id = '$id'");
		extract($select[0][0]);

		$customer_id = $cn_customer_id;
		$year = $cn_year;
		$month = $cn_month;

		echo '<Br/><form action="" method="post" class="pull-left">';
		if(isset($_POST['change'])){
			$units = $_POST['rate'];
		}else{
			$units = 1;
		}
		echo 'Change Units:';
		echo '<select name="rate">';
		echo '<option value="1">Wh</option>';
		echo '<option value="0.001">KWh</option>';
		echo '<option value="0.000001">MWh</option>';
		echo '</select>';
		echo ' &nbsp; <button name="change">Change</button>';
		echo '</form>';

		$db = new Db();
		$select = $db->select("SELECT dn_id FROM debit_note WHERE dn_year='$year' AND dn_month='$month' AND dn_customer_id='$customer_id'");

		echo '<div class="nav files pull-right">';
		if($db->num_rows()){
			extract($select[0][0]);
			echo '<b>Debit Note:</b> <a href="'.return_url().'services/debit-note-details/'.$dn_id.'">';
			echo $this->rgf("customer", $customer_id, "customer_id", "customer_short_name");
			echo '/DN/'.str_pad($dn_id, 3, '0', STR_PAD_LEFT).'/'.$year;
			echo '</a>';
			echo '&nbsp;  &nbsp;'; 

		}else{
			
			echo '<a class="btn btn-warning" href="'.return_url().'services/view-invoice/'.$customer_id.'/'.$month.'/'.$year.'"><i class="fa fa-level-up fa-fw"></i> Raise Debit Note</a>';
		}
		echo '&nbsp;  &nbsp;'; 

		$db = new Db();
		$select = $db->select("SELECT cn_id FROM credit_note WHERE cn_year='$year' AND cn_month='$month' AND cn_customer_id='$customer_id'");

		
		if($db->num_rows()){
			extract($select[0][0]);
			echo '<b>Credit Note:</b> <a href="'.return_url().'services/credit-note-details/'.$cn_id.'">';
			echo $this->rgf("customer", $customer_id, "customer_id", "customer_short_name");
			echo '/DN/'.str_pad($cn_id, 3, '0', STR_PAD_LEFT).'/'.$year;
			echo '</a>';
		}else{
			
			echo '<a class="btn btn-warning" href="'.return_url().'services/credit-note/'.$customer_id.'/'.$month.'/'.$year.'"><i class="fa fa-level-up fa-fw"></i> Raise Credit Note</a>';
		}	
		echo '&nbsp;  &nbsp;';
		echo '<a class="btn btn-warning" href="'.return_url().'services/view-invoice/'.$customer_id.'/'.$month.'/'.$year.'"><i class="fa fa-table fa-fw"></i> Invoice</a>';
		echo '&nbsp;  &nbsp;';
		echo '<a class="btn btn-success" href="" onfocus="return print(\'printMe\');"><i class="fa fa-print fa-fw"></i> Print Credit Note</a>	
		</div>';
		echo '<div class="clearfix"></div> ';
		echo '<br/><br/>';
		?>
		<style>
		table tr th{ background-color:#ccc; color:black; }
		.hide{display:none;}
		.table tr:hover{
			background-color:none;
			background:none !important;
		}
		#y>tbody>tr>td{ padding:5px; background-color:#fbfbfb; border:1px solid #000; }
		</style>
		<div id="printMe" style="width:900px;margin:auto;">
			<table border="0" cellspacing="0" cellpadding="2" id="y">
				<tbody>
				<tr valign="bottom">
					<td colspan="11">
					<img src="<?php display_url();?>images/uetcl_header.png" width="100%">
					<center><h2 style="color:blue;margin: 5px 0 0 0; padding:0">TAX INVOICE</h2></center>
						<!-- <hr style="border:1px solid black;"/> -->
						
						<p align="right" style="font-weight: bold; color:red;">Credit Note:
							<?php  
							echo $this->rgf("customer", $customer_id, "customer_id", "customer_short_name");
			echo '/CN/'.str_pad($cn_id, 3, '0', STR_PAD_LEFT).'/'.$year;
							?></p>
						<table style="" class="" cellspacing="10">
							
							<tr>
								<td>Date:</td>
								<td><?php echo date('dS F Y h:i:s a'); ?></td>
							</tr>
							<tr valign="top">
								<td width="35%"><br/>Name of Customer:</td>
								<td>
								<?php
									$select = $db->select("SELECT * FROM customer WHERE customer_id = '$customer_id'");
									extract($select[0][0]);
									echo '<Br/><b>'.$customer_full_name.'</b>';
									echo '<br/>';
									echo '<span style="font-size: 12px;">';
									if(empty($customer_street)){
										echo nl2br($customer_address);
									}else{
										echo nl2br($customer_street).'<br/>';
										echo nl2br($customer_postal_code).'<br/>';
										echo nl2br($customer_tel).'<br/>';
										echo nl2br($customer_email).'<br/>';
									}
									echo '</span>';
									echo '<br/><br/>';
								?>								
								</td>
							</tr>
							<tr>
								<td>Detail:</td>
								<td><u><b>Credit Note - <?php 
								echo month_name($month);
								echo ' ';
								echo $year;
								?></b></u><br/>
								<?php echo nl2br($dn_details); ?>
							</td>
							</tr>
						</table>
						<?php
										

						$t1 = $t2 = $t3 = $t4 = 0;							
							
						$t1 += $cn_bit_peak*$units;
						$t2 += $cn_bit_shoulder*$units;
						$t3 += $cn_bit_off_peak*$units;
						$t4 += ($cn_bit_peak+$cn_bit_shoulder+$cn_bit_off_peak)*$units;	
						

						$ttu = $tta = 0;
						?>
						<style>
							#x tr td, #x tr th{
								padding:2px 10px;
							}
						</style>
						<table cellpadding="5" cellspacing="0" border="1" width="100%" id="x">
							<tr>
								<th>TIME BAND</th>
								<th>RATE</th>
								<th>QTY - 
									<?php
									echo '(';
									if($units == 1){
										echo 'Wh';
									}elseif($units == 0.001){
										echo 'KWh';
									}elseif($units == 0.000001){
										echo 'MWh';
									}
									echo ')';
									?>
								</th>
								<th>AMOUNT</th>
							</tr>
							<tr>
								<?php 
								$o = $cn_peak;
								$ttu += $t1;
								$tta += $o*$t1;
								?>
								<td>Peak</td>
								<td align="right"><?php echo number_format($o); ?></td>
								<td align="right"><?php echo number_format($t1); ?></td>
								<td align="right"><?php echo number_format($o*$t1); ?></td>
							</tr>
							<tr>
								<?php 
								$o = $cn_shoulder;
								$ttu += $t2;
								$tta += $o*$t2;
								?>
								<td>Shoulder</td>
								<td align="right"><?php echo number_format($o); ?></td>
								<td align="right"><?php echo number_format($t2); ?></td>
								<td align="right"><?php echo number_format($o*$t2); ?></td>
							</tr>
							<tr>
								<td>Off Peak</td>
								<?php 
								$o = $cn_off_peak;
								$ttu += $t3;
								$tta += $o*$t3;
								?>
								<td align="right"><?php echo number_format($o); ?></td>
								<td align="right"><?php echo number_format($t3); ?></td>
								<td align="right"><?php echo number_format($o*$t3); ?></td>
							</tr>
							<tr>
								<td>Total</td>
								<td align="right"><?php echo number_format($ttu); ?></td>
								<td></td>
								<td align="right"><?php echo number_format($tta); ?></td>
							</tr>
							<tr>
								<td colspan="3">Wheeling Charges(<?php echo $cn_wheeling_charge; ?>%)</td>
								<td align="right"><?php echo number_format($tta*$cn_wheeling_charge/100); ?></td>
							</tr>
							<?php $tta += $tta*$cn_wheeling_charge/100; ?>
							<tr>
								<td colspan="3">Total (VAT Exclusive)</td>
								<td align="right"><?php echo number_format($tta); ?></td>
							</tr>							
							<tr>
								<td colspan="3">Add 18% VAT</td>
								<?php $tot = 18/100*$tta; ?>
								<td align="right"><?php echo number_format($tot); ?></td>
							</tr>							
							<tr>
								<td colspan="3">Total Amount (VAT Incl)</td>
								<?php $tot = 18/100*$tta; ?>
								<td align="right"><?php echo number_format($tta*(18/100+1)); ?></td>							
							<tr>
								<td colspan="4">
								<?php $tot = $tta*(18/100+1); if($tot<200000000000){?>
								Total in words:<br/><?php echo '<b>'.number_to_words($tta*(18/100+1)).'</b>'; ?></td>
								<?php } ?>
							</tr>
							<?php if($oi_exchange_rate && $currency =='Dollars'){?>
							<tr>
							<td colspan="4">
								Exhange Rate:
								<?php 
								//echo ;
									echo number_format($oi_exchange_rate,2);
								echo " $currency".''; 
								?>
								</td>
							</tr>
						<?php }?>
						</table>
						<span style="color:red; font-weight: bold; font-size: 12px; text-align: center; ">Accounts are due within 45days of receipt of this invoice</span>				

						<br/><br/>
						<br/><br/>
						Prepared by: ......................... 
						<?php for($i=0; $i<20; $i++) echo ' &nbsp ';  ?>
						Approved By: .........................
					</td>				
				</tr>
				
			</tbody></table>
		</div>
	
	</div>
	<?php
	}

	public function energyApprovals($user_id, $year=0, $month=0, $invoice=0, $ms="", $customer=0){
		//echo $this->rgf("sysuser", $user_id, "user_id", "user_designation");
		$db = new Db();
		$select = $db->select("SELECT app_role_id FROM approval_order ORDER BY app_id ASC");
		
		echo '<div style="font-size: 12px;">';
		$tc=1;
		echo '<h2 style="border-bottom:1px solid #000; margin-top:15px; font-size: 16px;">Approvals:</h2>';
		echo '<ol>';
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
			if($tc == 1){
				$sel = $d->select("SELECT * FROM done_levels WHERE dl_year = '$year' AND dl_month = '$month' AND dl_app$tc = 1 AND dl_customer = '$customer'");
				
			}else{
				$tcn = $tc-1;
				$sql = "SELECT * FROM done_levels WHERE dl_year = '$year' AND dl_month = '$month' AND dl_app$tc = 1 AND dl_app$tcn = 1 AND dl_customer = '$customer'";
				$sel = $d->select($sql);
			}

			if($d->num_rows()){
				$dd = new Db();
				
				if($invoice){

					//echo '<br/><br/>................................<br/>';

					$sele = $dd->select("SELECT arc_added_by FROM all_readings_comments WHERE arc_year = '$year' AND arc_month = '$month' AND arc_part = '$tc' AND arc_customer = '$customer' ");

					if($dd->num_rows()){
						extract($sele[0][0]);
					}
					if($d->num_rows()){
						extract($sel[0][0]);
					}

					//echo '<b style="font-size:8pt">'.ucwords(strtolower(''.$this->rgf("designation", $this->rgf("sysuser", $arc_added_by, "user_id", "user_designation"), 'designation_id', 'designation_name').'</b>'));
					echo '<b style="font-size:8pt">'.ucwords(strtolower(''.$this->rgf("designation", ${"dl_selected".$tc}, 'designation_id', 'designation_name').'</b>'));
					//echo '<br/><span style="font-size:8pt">'.$this->full_name($arc_added_by).'</span>';//$this->full_name($arc_added_by); //'Approved by:'.					
					$tc_num = $tc;
				}else{
					if($d->num_rows()){
						extract($sel[0][0]);
					}
				
					$sele = $dd->select("SELECT arc_added_by FROM all_readings_comments WHERE arc_year = '$year' AND arc_month = '$month' AND arc_part = '$tc' AND arc_customer = '$customer' ");

					if($dd->num_rows()){
						extract($sele[0][0]);
					}

					echo '<b style="font-size:8pt">'.ucwords(strtolower(''.$this->rgf("designation", ${"dl_selected".$tc}, 'designation_id', 'designation_name').'</b>'));


					//echo '<b>'.strtoupper(''.$this->rgf("designation", $this->rgf("sysuser", $arc_added_by, "user_id", "user_designation"), 'designation_id', 'designation_name').'</b><br/>');

					extract($sel[0][0]);
					
					//echo ''.$this->full_name($dl_added_by).'	';
					echo ''.$this->full_name(${'dl_selected'.$tc}).'	';
					
					$sel = $d->select("SELECT * FROM all_readings_comments WHERE arc_year = '$year' AND arc_month = '$month' AND arc_part = '$tc' AND arc_customer = '$customer' ");
					if($d->num_rows()){
						echo '<br/><b>Comment: </b><br/>';
					}
					
					foreach($sel[0] as $row){
						extract($row);
						//$k++;
						echo nl2br($arc_description);
						echo '<br/><b>Date: </b>'.FeedBack::date_s(time()).'<br/>';
					}
				}
			}else{

				if($tc == 1){
					$tot = 1;
				}else{
					$tcn = $tc-1;
					$sel = $d->select("SELECT * FROM done_levels WHERE dl_year = '$year' AND dl_month = '$month' AND dl_customer = '$customer' AND dl_app$tcn IS NOT NULL");
					$tot = $d->num_rows();
				}

				if($tot){
					extract($sel[0][0]);
					//7 == amanda
					//if($invoice == 0 && (($tc == 1 && user_id() == 7)|| ${"dl_selected".$tc} == $this->rgf("sysuser", user_id(), "user_id", "user_designation"))){	
					if($invoice == 0 && (($tc == 1)|| ${"dl_selected".$tc} == $this->rgf("sysuser", user_id(), "user_id", "user_designation"))){									
				
						echo '<form action="" method="post" style="background-color: #fbe2e2;padding:10px; border:1px solid #000; ">';
						

						if(isset($_POST['approve']) && $_POST['part']== $tc){
							echo '';
							$ind = array();
							$comment = $_POST['comment'];
							$part = $_POST['part'];
							$parc_id = $_POST['parc_id'];

							$customername = $this->rgf("customer", $customer, "customer_id", "customer_short_name");

							if($tc == 1){

								$this->assignInvoiceNumber($year, $month, $customer);

								$approveOne = $_POST["approveOne"];
								$approveTwo = $_POST["approveTwo"];

								$ind['dl_year'] = $year;
								$ind['dl_month'] = $month;
								$ind['dl_date_added'] = time();
								$ind['dl_added_by'] = user_id();
								$ind['dl_selected1'] = $this->rgf("sysuser", user_id(), "user_id", "user_designation");
								$ind['dl_selected2'] = $approveOne;
								$ind['dl_selected3'] = $approveTwo;
								$ind['dl_customer'] = $customer;
							}
								
							$ind["dl_app$tc"] = 1;
							
							$com = array();
							$com["arc_year"] = $year;
							$com["arc_month"] = $month;
							$com["arc_description"] = $comment;
							$com["arc_date_added"] = time();
							$com["arc_added_by"] = user_id();
							$com["arc_part"] = $part;
							$com["arc_customer"] = $customer;


							$insert = $d->insert("all_readings_comments", $com);
							echo $d->error();
							$link = return_url().portion(1)."/".portion(2)."/".portion(3)."/".portion(4)."/".portion(5);
							
							if($tc==1){

								$approveOne = $_POST["approveOne"];
								$approveTwo = $_POST["approveTwo"];

								$insert = $d->insert("done_levels", $ind);
								echo $d->error();
								
								$next = $this->next($tc, $year, $month);
								$to = $next['email'];
								$message = "Hello ".$next['fname'].",";							
								
								$customer = $this->rgf("customer", $customer_id, "customer_id", "customer_short_name");

								$message .= "<br/><br/>The invoices for <b>".$customername. " $year-".month_name($month)."</b> is ready for approval.<br/>";
								$message .= "\r\nYou can use this link: <a href='$link'>$link</a>";	
								$message .= $ms;

							    $subject = "Energy Invoice";
							    Feedback::sendmail($to,$subject,$message,$name);
							}else{

								if($tc == 2){
									$next = $this->next($tc, $year, $month);
									$to = $next['email'];
									$message = "Hello ".$next['fname'].",";
									$message .= "<br/><br/>The invoice for <b>$customername $year-".month_name($month)."</b> is ready for approval.<br/>";
								}elseif($tc == 3){
									$next = $this->next($tc, $year, $month);
									$to = $next['email'];
									$message = "Hello ".$next['fname'].",";	
									$message .= "<br/><br/>The invoice for <b>$customername $year-".month_name($month)."</b> is Successfully approved.<br/>";						
								}
									
								
								$message .= "\r\nYou can use this link: <a href='$link'>$link</a>";	

								$message .= $ms;
							    $subject = "Energy Invoice Approval";
							    Feedback::sendmail($to,$subject,$message,$name);

								$insert = $d->update("done_levels", $ind, ["dl_id"=>$parc_id]);
							}

							FeedBack::redirect(return_url().portion(1)."/".portion(2)."/".portion(3)."/".portion(4)."/".portion(5));
						}
						//echo ">>>>".$id_id;
						echo 'Comment:<br/><textarea id="comment" name="comment" style="width: 100%;"></textarea>';
						
						if($tc==1){	
							echo 'Remarks:<br/><textarea id="remarks" name="comment" style="width: 100%;"></textarea>';
							$date1 = strtotime(date($year.'-'.$month.'-01'));
						    echo 'Posting Date:<br/><input type="date" id="invoice_date" value = "'.date('Y-m-t',$date1).'" name="" style="width: 100%;">';
							$app = array("One", "Two");
							foreach($app as $ap){
								echo '<br/>Select Approve '.$ap.':';
								echo '<select id="Approve'.$ap.'" required name="approve'.$ap.'">';
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
							if($tc==3){
								//echo '<br><br><button data-type="approve" data-count="'.$tc.'" type="button" name="approve" id="pushInvoiceToEFRIS" class="btn btn-xs btn-success"><i class="fa fa-fw fa-check"></i> Approve & Push Invoice to EFRIS</button>';
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
							echo '<input type="hidden" name="" id="customer" value="'.$customer.'"/>';
							$dx = new Db();
							$selector = $dx->select("SELECT * FROM invoice_date WHERE id_year='$year' AND id_month='$month' and id_customer_id ='$customer'");
							if($dx->num_rows()){
								extract($selector[0][0]);
								echo '<input type="hidden" name="" id="invoice_remarks" value="'.$id_remarks.'"/>';
							   //echo '<input type="hidden" name="" id="inv_date" value="'.date("Y-m-d",$id_invoice_date).'"/>';
								
							}	

							echo '<input type="hidden" name="" id="part" value="'.$tc.'"/>';

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
				        	//alert(parc_id);
				        	
					        //alert(ApproveOne);				        	
				        	var month = $('#month').val();				        	
				        	var year = $('#year').val();				        	
				        	var part = $('#part').val();			        	
				        	var customer = $('#customer').val();
				        	var remarks = $('#remarks').val();
				        	var invoice_date = $('#invoice_date').val();

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
				        	}else{
				        		$('#approvalStatus').html('Sending. Please wait.');
						        var form_data = new FormData();
						        form_data.append('comment', comment);
						        form_data.append('part', part);
						        form_data.append('month', month);
						        form_data.append('year', year);
						        form_data.append('parc_id', parc_id);
						        form_data.append('invoice_date', invoice_date);
						        form_data.append('remarks', remarks);
						        //alert(remarks);
						        if(part == 1){
							        form_data.append('ApproveOne', ApproveOne);
							        form_data.append('ApproveTwo', ApproveTwo);
							    }
						        form_data.append('customer', customer);
						        form_data.append('count', count);
						        form_data.append('type', type);
						        $.ajax({
						            url: urlPath+"ajax/approveEnergySales.php",
						            type: "POST",
						            data: form_data,
						            contentType: false,
						            cache: false,
						            processData:false,
						            success: function(data){
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
							echo '<b>'.strtoupper(''.$this->rgf("designation", ${"dl_selected".$tc}, 'designation_id', 'designation_name').'</b><br/>');

						}
						
					}else{

						if($tc == 1){
							echo '<b>'.strtoupper(''.$this->rgf("designation", $this->rgf("sysuser", $user_id, "user_id", "user_designation"), 'designation_id', 'designation_name').'</b><br/>');
						}else{
							echo '<b>'.strtoupper(''.$this->rgf("designation", ${"dl_selected".$tc}, 'designation_id', 'designation_name').'</b><br/>');

						}
						
						 echo 'Not Yet';
					}

				}else{
					if($tc == 1){
						echo '<b>'.strtoupper(''.$this->rgf("designation", $this->rgf("sysuser", $user_id, "user_id", "user_designation"), 'designation_id', 'designation_name').'</b><br/>');
					}else{
						echo '<b>'.strtoupper(''.$this->rgf("designation", ${"dl_selected".$tc}, 'designation_id', 'designation_name').'</b><br/>');

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

		return $tc_num = array('tc' =>$tc_num);					
	}

	public function getAssignedInvoiceNumber($customer, $year, $month){
		$j = new Db();
		$customer = $this->rgf("customer", $customer, "customer_id", "customer_short_name");
		$sql = "SELECT * FROM invoice_date WHERE id_customer = '$customer' AND id_year = '$year' AND id_month = '$month' ORDER BY id_number DESC";
		//echo $sql;
		$sel = $j->select($sql);
		//$v = "SNEL/2020-05/001";
		//echo substr($v, 13);
        if($j->num_rows()){
        	extract($sel[0][0]);    

        	$endofdate = strtotime($year.'-'.$month.'-01');
			
				
			return array($id_customer.'/'.$year.'-'.$month.'/'.substr($id_number, 4), $id_date_added, strtotime($year.'-'.$month.'-'.date('t', $endofdate)));
	    }
		return;
	}



	public function assignInvoiceNumber($year, $month, $customer_id){

		$time = strtotime(date($year.'-'.$month.'-15'));
		$dm = date('y', $time).date('m', $time);
		$suffix = $dm;

		$id_number = 0;
		$customers = array();
        $db = new Db();
        $j = new Db();
        $select = $db->select("SELECT customer_id FROM customer WHERE customer_id = '$customer_id' ORDER BY customer_short_name ASC");
        foreach($select[0] as $row){
            extract($row);
            $this->lockReadings($customer_id, $year, $month);
            $c = $this->rgf("customer", $customer_id, "customer_id", "customer_short_name");

            $sel = $j->select("SELECT TOP 1 id_number FROM invoice_date WHERE id_customer_id = '$customer_id' ORDER BY id_number DESC");
            if($j->num_rows()){
            	extract($sel[0][0]);            	
		    }
            $f = (int)(substr($id_number, 4))+1;
			$number = $suffix.str_pad($f, 3, "0", STR_PAD_LEFT);

			$sel = $j->select("SELECT id_id FROM invoice_date WHERE id_number = '$number' AND id_customer_id = '$customer_id'");

			$db = new Db();
			$sql = "SELECT id_id FROM invoice_date WHERE id_year = '$year' AND id_month = '$month' AND id_customer_id = '$customer_id'";
			$select = $db->select($sql);

            if(!$db->num_rows()){
            	$insert = $j->insert("invoice_date", [
            		"id_number"=>$number,
            		"id_year"=>$year,
            		"id_month"=>$month,
            		"id_date_added"=>time(),
            		"id_added_by"=>user_id(),
            		"id_customer"=>$c,
					"id_customer_id"=>$customer_id,
            	]);           	
		    }

		    echo $j->error();

        }
	}

	public function isOnListAndIsPending(){

		$db = new Db();
		$user_id = user_id();
		$designation = $this->rgf("sysuser", $user_id, "user_id", "user_designation");

			if ($designation=='35') {
			$select = $db->select("SELECT * FROM done_levels WHERE (dl_selected2 IS NOT NULL AND dl_app1 = 1 AND dl_app2 IS NULL) OR (dl_selected3 IS NOT NULL AND dl_app2 = 1 AND dl_app3 IS NULL) ORDER BY dl_date_added");
			}else{
				$select = $db->select("SELECT * FROM done_levels WHERE (dl_selected2 = '$designation' AND dl_app1 = 1 AND dl_app2 IS NULL) OR (dl_selected3 = '$designation' AND dl_app2 = 1 AND dl_app3 IS NULL) ORDER BY dl_date_added");
			}

		//extract($select[0][0]);

		return $db->num_rows();

	}

	public function energyApprovalListAction(){
		$this->isOnListAndIsPending();
		$db = new Db();
		$user_id = user_id();

		$designation = $this->rgf("sysuser", $user_id, "user_id", "user_designation");

		echo '<table id="table" border="1">';
		echo '<tr>';
		echo '<th>No.</th>';
		echo '<th>Invoice No.</th>';
		echo '<th style="text-align:center;">Approval 1</th>';
		echo '<th style="text-align:center;">Approval 2</th>';
		echo '<th style="text-align:center;">Approval 3</th>';
		echo '<th>Action</th>';
		echo '</tr>';
		if ($designation='35') {
			$select = $db->select("SELECT * FROM done_levels WHERE (dl_selected2 IS NOT NULL AND dl_app1 = 1) OR (dl_selected3 IS NOT NULL AND dl_app2 = 1) OR dl_selected1 IS NOT NULL ORDER BY dl_date_added");
		}else{
			$select = $db->select("SELECT * FROM done_levels WHERE (dl_selected2 = '$designation' AND dl_app1 = 1) OR (dl_selected3 = '$designation' AND dl_app2 = 1) OR dl_selected1 = '$designation' ORDER BY dl_date_added");
		}
		

		$db->num_rows();
		$no = 1;
		foreach($select[0] as $row){
			extract($row);

			echo '<tr>';
			echo '<td width="30px">'.($no++).'.</td>';
			echo '<td>'.$this->rgf("customer", $dl_customer, "customer_id", "customer_short_name").' '.$dl_year.'-'.month_name_short($dl_month).'</td>';

			echo '<td class="">';
			if($dl_app1){
				echo '<i class="fa fa-check"></i> Approved';
			}else{
				if($dl_selected1==$designation && empty($dl_app1)){
					echo '<span class="text-danger">Pending Your Approval</span>';
				}
			}
			echo '</td>';
			echo '<td class="">';
			if($dl_app2){
				echo '<i class="fa fa-check"></i> Approved';
			}else{
				if($dl_selected2==$designation && $dl_app1 == 1){
					echo '<span class="text-danger">Pending Your Approval</span>';
				}elseif($dl_app1 == 1){
					echo 'Pending '.$this->rgf("designation", $dl_selected2, "designation_id", "designation_name")."'s Approval ";
				}
			}
			echo '</td>';
			echo '<td class="">';
			if($dl_app3){
				echo '<i class="fa fa-check"></i> Approved';
			}else{
				if($dl_selected3==$designation && $dl_app2 == 1){
					echo '<span class="text-danger">Pending Your Approval</span>';
				}elseif($dl_app2 == 1){
					echo 'Pending '.$this->rgf("designation", $dl_selected3, "designation_id", "designation_name")."'s Approval ";
				}
			}
			echo '</td>';

			echo '<td><a href="'.return_url().'services/view-invoice/'.$dl_customer.'/'.$dl_year.'/'.$dl_month.'">View Invoice</a></td>';
			echo '</tr>';
		}


		echo '</table>';
	}
	public function energyApprovalListPendingAction(){
		$this->isOnListAndIsPending();
		$db = new Db();
		$user_id = user_id();

		$designation = $this->rgf("sysuser", $user_id, "user_id", "user_designation");

		echo '<table id="table" border="1">';
		echo '<tr>';
		echo '<th>No.</th>';
		echo '<th>Invoice No.</th>';
		echo '<th style="text-align:center;">Approval 1</th>';
		echo '<th style="text-align:center;">Approval 2</th>';
		echo '<th style="text-align:center;">Approval 3</th>';
		echo '<th>Action</th>';
		echo '</tr>';

	if ($designation=='35') {
		$select = $db->select("SELECT * FROM done_levels WHERE (dl_selected2 IS NOT NULL AND dl_app1 = 1 AND dl_app2 IS NULL) OR (dl_selected3 IS NOT NULL AND dl_app2 = 1 AND dl_app3 IS NULL) ORDER BY dl_date_added");
	}else{
		$select = $db->select("SELECT * FROM done_levels WHERE (dl_selected2 = '$designation' AND dl_app1 = 1 AND dl_app2 IS NULL) OR (dl_selected3 = '$designation' AND dl_app2 = 1 AND dl_app3 IS NULL) ORDER BY dl_date_added");
	}
		
		//$db->num_rows();
		$no = 1;
		foreach($select[0] as $row){
			extract($row);

			echo '<tr>';
			echo '<td width="30px">'.($no++).'.</td>';
			echo '<td>'.$this->rgf("customer", $dl_customer, "customer_id", "customer_short_name").' '.$dl_year.'-'.month_name_short($dl_month).'</td>';

			echo '<td class="">';
			if($dl_app1){
				echo '<i class="fa fa-check"></i> Approved';
			}else{
				if($dl_selected1==$designation && empty($dl_app1)){
					echo '<span class="text-danger">Pending Your Approval</span>';
				}
			}
			echo '</td>';
			echo '<td class="">';
			if($dl_app2){
				echo '<i class="fa fa-check"></i> Approved';
			}else{
				if($dl_selected2==$designation && $dl_app1 == 1){
					echo '<span class="text-danger">Pending Your Approval</span>';
				}elseif($dl_app1 == 1){
					echo 'Pending '.$this->rgf("designation", $dl_selected2, "designation_id", "designation_name")."'s Approval ";
				}
			}
			echo '</td>';
			echo '<td class="">';
			if($dl_app3){
				echo '<i class="fa fa-check"></i> Approved';
			}else{
				if($dl_selected3==$designation && $dl_app2 == 1){
					echo '<span class="text-danger">Pending Your Approval</span>';
				}elseif($dl_app2 == 1){
					echo 'Pending '.$this->rgf("designation", $dl_selected3, "designation_id", "designation_name")."'s Approval ";
				}
			}
			echo '</td>';

			echo '<td><a href="'.return_url().'services/view-invoice/'.$dl_customer.'/'.$dl_year.'/'.$dl_month.'">View Invoice</a></td>';
			echo '</tr>';
		}


		echo '</table>';
	}
	
	public function invoiceTotalSummary($id, $year, $month){
		
		$db = new Db();
		$select = $db->select("SELECT * FROM customer WHERE customer_id = '$id'");
		extract($select[0][0]);
	
		//$units = $this->customerAndSchedule($id, $year, $month);
		//$this->energyLinks($id, $year, $month);
		
		$db = new Db();
		$select = $db->select("SELECT * FROM metering_point, r_reading WHERE mp_customer_id = '$customer_id' AND rea_cus_id = '$customer_id' AND mp_id = rea_mp_id");
		foreach($select[0] as $row){
			extract($row);
			if(date('Y', $rea_date)==portion(5) && date('m', $rea_date)==portion(4)){
				$invoice_no = str_pad($rea_id, 5, '0', STR_PAD_LEFT);;	
			}
		}

						
		//================= currency ======================================
		if($customer_currency){
			$currency = $this->rgf("currency",$customer_currency, "currency_id", "currency_name");
			$symbol = $this->rgf("currency",$customer_currency, "currency_id", "currency_symbol");
		}else{
			$currency = $this->rgf("currency",1, "currency_id", "currency_name");
			$symbol = $this->rgf("currency",1, "currency_id", "currency_symbol");
		}
	
		$tou = $this->tou($customer_id, $month, $year);
		
		$peak = $tou[0];
		$shoulder = $tou[1];
		$off_peak = $tou[2];	

		//=============== WHEELING CHARGE BANDS =====================================
		$db = new Db();
		$select = $db->select("SELECT * FROM wheeling_charge WHERE wc_customer_id = '$customer_id' ORDER BY wc_id ASC");
		$assigned2 = 0;
		foreach($select[0] as $row){
			extract($row);
			if(date('Y', $wc_date_added)==$year && date('m', $wc_date_added)==$month)
			{
				$cwc = $wc_rate;
				$assigned2 = 1;
			}
		}

		if($assigned2 == 0){
			$db = new Db();
			$select = $db->select("SELECT * FROM wheeling_charge WHERE wc_customer_id = '$customer_id' ORDER BY wc_id ASC");
			$assigned = 0;
			foreach($select[0] as $row){
				extract($row);
				//if(date('Y', $wc_date_added)==$year && date('m', $wc_date_added)==$month)
				{
					$cwc = $wc_rate;
					$assigned2 = 1;
				}
			}
		}
						
		$t1 = $t2 = $t3 = $t4 = 0;
		$units = 0.001;
		$m = $this->sumFormulae($customer_id, $year, $month);
		//echo '->'.$m[0].'->'.$units;
		$t1 = ($m[0]*$units);
		$t2 = ($m[1]*$units);
		$t3 = ($m[2]*$units);
		$t4 = ($m[3]*$units);

		$h = $this->wheelingChargeSum($customer_id, $year, $month);
						
		$ttu = $tta = 0;
										 
		$o = $peak;
		$ttu += $t2;
		$tta += $o*$t2;
	
		$o = $shoulder;
		$ttu += $t1;
		$tta += $o*$t1;

		$o = $off_peak;
		$ttu += $t3;
		$tta += $o*$t3;

		$tr = $tta;

		$payables = array();
		$payablez = array();
		$db = new Db();

		$select = $db->select("SELECT distinct bn_customer FROM boundary_node WHERE bn_customer_payable_to = '$customer_id'");

		foreach($select[0] as $row){
			extract($row);

			$sel = $db->select("SELECT bn_metering_point FROM boundary_node WHERE bn_customer_payable_to = '$customer_id' AND bn_customer = '$bn_customer' ");
			foreach($sel[0] as $ro){
				extract($ro);
				$zz = rate_calculate($bn_customer, strtotime(date("$year-$month-15")), $bn_metering_point);
						
				if($zz[24]!="([DONOT_DISPLAY])")
				{
					$payables[$bn_customer][] = $bn_metering_point;
				}
			}
			
		}

		$payablez = array();
		$db = new Db();
		$select = $db->select("SELECT distinct bn_customer_payable_to FROM boundary_node WHERE bn_customer = '$customer_id'");
		foreach($select[0] as $row){
			extract($row);

			$sel = $db->select("SELECT bn_metering_point FROM boundary_node WHERE bn_customer_payable_to = '$bn_customer_payable_to' AND bn_customer = '$customer_id' ");
			foreach($sel[0] as $ro){
				extract($ro);
				//$payablez[$bn_customer_payable_to][] = $bn_metering_point;
				$zz = rate_calculate($customer_id, strtotime(date("$year-$month-15")), $bn_metering_point);

				if($zz[24]!="([DONOT_DISPLAY])")
				{
					$payablez[$bn_customer_payable_to][] = $bn_metering_point;
				}
			}
		}

		$payable = 0;
		foreach($payablez as $pay => $values){ echo $pay.' '.$customer_id. ', ';

			//AT KIL
			if($customer_id == 1017 && $pay == 2019 && $rea_date < strtotime(date('2019-08-18'))){

			}elseif(($customer_id == 2020 || $customer_id == 2040) && $pay==2017 ){
				$j = $this->collectable2($pay, $customer_id, $year, $month, $consider=0);
			}else{						
				$j = $this->collectable($pay, $customer_id, $year, $month, $consider=0);
			}
			

			$payable += $j[2];
			$i++;
		}

			//////////////////////////////////////////////////////////////////////////
				$i=1;
				$collectable = 0;
				if($customer_id == 2022){
					$payables[2020] = 5312;
				}
				foreach($payables as $pay=>$value){

					//AT KIL
					if($customer_id == 1017 && $pay == 2019 && $rea_date < strtotime(date('2019-08-18'))){

					}elseif(0){
						$jm = $this->krecsAndUedcl($customer_id, $pay, $year, $month);					
					}elseif($customer_id == 2021 && $pay==2017){
						$jm = $this->collectable2($customer_id, $pay, $year, $month, $consider=0);
					}else{						
						$jm = $this->collectable($customer_id, $pay, $year, $month, $consider=0);
					}


					$collectable += $jm[2];
					$i++;
				}

		/////////////////////////////////////////////////////////////
		// VAT EXCLUSIVE		
		/////////////////////////////////////////////////////////////
		$tbf = $this->totalBackflow($customer_id, $year, $month);
		$tta += $payable+($collectable*-1)+($tbf[5]*-1); 
		$total_vat_x = $tta;

		$vat = $customer_vat/100*$tta; 
		$total_vat_in = $tta*($customer_vat/100+1);	
		$total_energy = $tr; 

		if($tr==0){
			$total_vat_x=$vat=$total_vat_in=$payable=$collectable=0;
		}

			//          0              1            2         3           4           5
		return array($total_energy, $total_vat_x, $vat, $total_vat_in, $payable, $collectable);
	}
	
	
	public function viewInvoiceAction(){


		$class = ($this->set_class)?$this->set_class:portion(1);
		$method = ($this->set_method)?$this->set_method:portion(2);
		$id = ($this->set_customer)?$this->set_customer:portion(3);
		$year = ($this->set_year)?$this->set_year: portion(4);
		$month = ($this->set_month)?$this->set_month: portion(5);
		
		$note = ($this->set_note)?$this->set_note: portion(6);

		if(empty($year) || empty($month) || empty($id)){
			Feedback::error("Please select Customer, Year and Month");
			$units = $this->customerAndSchedule($id, $year, $month);
			$this->energyLinks($id, $year, $month);
			$units = 0.001;
			echo '<div class="clearfix"></div>';
		}elseif(!$this->readingExist($id, $year, $month)){
			Feedback::error("No readings found");
			$units = $this->customerAndSchedule($id, $year, $month);
			$this->energyLinks($id, $year, $month);
			$units = 0.001;
			echo '<div class="clearfix"></div>';
		}elseif($id==2036){			
			Feedback::error("No invoice for Backflows, Check Umeme");
			$units = $this->customerAndSchedule($id, $year, $month);
			$this->energyLinks($id, $year, $month);
			$units = 0.001;
			echo '<div class="clearfix"></div>';
		}elseif($id==2041){			
			Feedback::error("No invoice for Backflows, Check UEDCL");
			$units = $this->customerAndSchedule($id, $year, $month);
			$this->energyLinks($id, $year, $month);
			$units = 0.001;
			echo '<div class="clearfix"></div>';
		}else{
			if($note == "credit-note"){
				$this->viewInvoiceCreditNote($id, $month, $year);
			}else{
				$this->viewInvoiceNormal($id, $month, $year);
			}
		}


	}

	public function viewInvoiceNormal($id, $month, $year){

		$class = ($this->set_class)?$this->set_class:portion(1);
		$method = ($this->set_method)?$this->set_method:portion(2);
		$id = ($this->set_customer)?$this->set_customer:portion(3);
		$year = ($this->set_year)?$this->set_year: portion(4);
		$month = ($this->set_month)?$this->set_month: portion(5);

		// $this->assignInvoiceNumber($year, str_pad('0', $month, 2), $id);
		$this->assignInvoiceNumber($year, str_pad($month, 2, 0, STR_PAD_LEFT), $id);

		$db = new Db();
		$select = $db->select("SELECT * FROM customer WHERE customer_id = '$id'");
		extract($select[0][0]);
		
		
	
	?>
	<div id="side">
		
		<?php 

		$units = $this->customerAndSchedule($id, $year, $month);
		$this->energyLinks($id, $year, $month);

		?>
		<style>
		table tr th{ background-color:#ccc; color:black; }
		.hide{display:none;}
		.table tr:hover{
			background-color:none;
			background:none !important;
		}
		#y>tbody>tr>td{ padding:5px; background-color:#fbfbfb; border:1px solid #000; }
		.hide{ display: none; }
		</style>
		<div id="printMe" style="width:900px;margin:auto;">
			<?php echo '<title>'.strtoupper($customer_short_name.' INVOICE').' - '.$set_month.' '.$set_year.'</title>'; ?>
			<style type="text/css">
				table tr td{
					padding:5px 20px;
				}
			</style>
			<table border="0" cellspacing="0" cellpadding="2" id="y" style="width:100%; font-family: arial; margin:auto;">
				<tbody>
				<tr valign="bottom">
					<td colspan="11">
					<img class="xhide" src="<?php display_url();?>images/uetcl_header.png" width="100%">
					<center class="xhide"><h2 style="color:blue;margin: 5px 0 0 0; padding:0">TAX INVOICE</h2></center>
						<!-- <hr style="border:1px solid black;"/> -->
						<?php
						$db = new Db();
						$select = $db->select("SELECT * FROM metering_point, r_reading WHERE mp_customer_id = '$customer_id' AND rea_cus_id = '$customer_id' AND mp_id = rea_mp_id");
						foreach($select[0] as $row){
							extract($row);
							if(date('Y', $rea_date)==$this->set_year && date('m', $rea_date)==$this->set_month){
								$invoice_no = str_pad($rea_id, 5, '0', STR_PAD_LEFT);
								$to_use_rea_id = $invoice_no;
							}
						}

						?>
						<p align="right" style="font-size:10pt; padding:0;margin:0; font-weight: bold; color:red;"><span style="color:black;">Tax Invoice No.:</span> &nbsp; <?php 
						$c = $this->getAssignedInvoiceNumber($customer_id, $year, $month);
						echo $c[0];

						 ?></p>
						<table border="0" class="" cellspacing="10" cellpadding="0" style="width:100% !important;font-family: arial; font-size: 12pt;">
							
							<tr valign="top">
								<td>Invoice Date:</td>
								<td><?php 
								if($c[1] != "")
									echo Feedback::date_s($c[1]); 
								else{
									echo ' - ';
								}
								?></td>
								<!-- <td rowspan="4" style="width:40%;">
									
								</td> -->
							</tr>
							<tr valign="top">
								<td>Posting Date:</td>
								<td><?php 
								if($c[1] != "")
									echo Feedback::date_s($c[2]); 
								else{
									echo ' - ';
								}
								?></td>
								<!-- <td rowspan="4" style="width:40%;">
									
								</td> -->
							</tr>
							<tr valign="top">
								<td width="">Name&nbsp;of&nbsp;Customer:</td>
								<td>
								<?php
									echo '<b>'.$customer_full_name.'</b>';
								?>								
								</td>
							</tr>
							<?php
							 if($customer_tin){
							?>
							<tr valign="top">
								<td width="">Customer TIN:</td>
								<td>
								<?php
									echo '<b>'.$customer_tin.'</b>';
								?>								
								</td>
							</tr>
						    <?php }?>
							<tr valign="top" class="hide">
								<td width="">Customer Address:</td>
								<td>
								<?php
									if(empty($customer_street)){
										echo nl2br($customer_address);
									}else{
										echo nl2br($customer_street).'<br/>';
										echo nl2br($customer_postal_code).'<br/>';
										echo nl2br($customer_tel).'<br/>';
										echo nl2br($customer_email).'<br/>';
									}
								?>								
								</td>
							</tr>
							<tr>
								<?php $year; $month; ?>
								<td>Detail:</td>
								<td><u><b><?php echo $this->noSpace('Energy Bill for '.month_name($month).' '.$year); ?></b></u></td>
							</tr>
						</table>
						<table style="" class="" cellspacing="10">
					     <?php
					     $db = new Db();
						 
					     $efris_info = $db->select("SELECT * FROM other_invoice where oi_ref_no='$c[0]'");
					     if($db->num_rows()){
						 
					     	extract($efris_info[0][0]);
					     	//print_r($efris_info);

					       $item_id = 3180;
							$buyer = $this->rgf("customer",$customer_id,"customer_id","customer_short_name");
							 $tin = $this->rgf("customer",$customer_id,"customer_id","customer_tin");
							 $qr = '<img style="text-align:center;width:150px;" src="'.return_url().'efris_qr_images/'.$oi_qr.'" alt="'.$oi_fdn.'"/>';
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
							?>
						</table>
						<?php
						//================= currency ======================================
						if($customer_currency){
							$currency = $this->rgf("currency",$customer_currency, "currency_id", "currency_name");
							$symbol = $this->rgf("currency",$customer_currency, "currency_id", "currency_symbol");
						}else{
							$currency = $this->rgf("currency",1, "currency_id", "currency_name");
							$symbol = $this->rgf("currency",1, "currency_id", "currency_symbol");
						}
	
						$tou = $this->tou($customer_id, $month, $year);
		
						$peak = $tou[0];
						$shoulder = $tou[1];
						$off_peak = $tou[2];




						if($customer_id ==2028){//Rwanda
							$db = new Db();
							//current main 1
							$customer_id = ($this->set_customer)?$this->set_customer:portion(3);
							$reading_time = strtotime(date("$year-$month-15"));

							$sql = "SELECT * FROM r_reading, r_rate, metering_point WHERE rea_date = '$reading_time' AND rea_cus_id = '$customer_id' AND rate_cus_id = '$customer_id' AND rate_reading_id = rea_id AND rea_mp_id = mp_id AND upper(mp_location) = 'KATUNA-MAIN'";
							$select = $db->select($sql);
							extract($select[0][0]);
							$present_main = $rate_cum_imports;

							if(empty($reading_date)){
								$reading_date = $rate_date_read;
							}

							//previous main reading
							$sql = "SELECT TOP 1 * FROM r_reading, r_rate, metering_point WHERE rea_date < '$rea_date' AND rea_id = rate_reading_id AND rea_cus_id = '$customer_id' AND rea_mp_id = mp_id AND rate_cus_id = '$customer_id' AND upper(mp_location) = 'KATUNA-MAIN' ORDER BY rea_date DESC";
							$sel = $db->select($sql);
							extract($sel[0][0]);
							$previous_main = $rate_cum_imports;


							///////////////////// check

							$sql = "SELECT * FROM r_reading, r_rate, metering_point WHERE rea_date = '$reading_time' AND rea_cus_id = '$customer_id' AND rate_cus_id = '$customer_id' AND rate_reading_id = rea_id AND rea_mp_id = mp_id AND upper(mp_location) = 'KATUNA-CHECK'";
							$select = $db->select($sql);
							extract($select[0][0]);
							//print_r($select[0][0]);
							$present_check = $rate_cum_imports;

							if(empty($reading_date)){
								$reading_date = $rate_date_read;
							}

							//previous main reading
							$sql = "SELECT TOP 1 * FROM r_reading, r_rate, metering_point WHERE rea_date < '$rea_date' AND rea_id = rate_reading_id AND rea_cus_id = '$customer_id' AND rea_mp_id = mp_id AND rate_cus_id = '$customer_id' AND upper(mp_location) = 'KATUNA-CHECK' ORDER BY rea_date DESC";
							$sel = $db->select($sql);
							extract($sel[0][0]);
							$previous_check = $rate_cum_imports;

							$advance_main = $present_main - $previous_main;
							$advance_check = $present_check - $previous_check;

							$average = ($advance_main+ $advance_check)/2;
							?>
							<?php
							
							$tm = 0;
							


							if($peak != (int)$peak){
								$peak1 = number_format($peak, 5);
							}else{
								$peak1 = $peak;
							}
							if($shoulder != (int)$shoulder){
								$shoulder1 = number_format($shoulder, 5);
							}else{
								$shoulder1 = $shoulder;
							}
							if($off_peak != (int)$off_peak){
								$off_peak1 = number_format($off_peak, 5);
							}else{
								$off_peak1 = $off_peak;
							}
							
							if($reading_date == ""){
								$reading_date = "";
							}else{
								$reading_date = Feedback::date_c(strtotime($reading_date));
							}

							$q1 = ($present_main+$present_check)/2/1000;
							$q2 = ($previous_main+$previous_check)/2/1000;
							
                         
							?>

							<style>
								#x tr td, #x tr th{
									padding:2px 10px;
								}
								#x tr th{
									text-align: center;
								}
							</style>
							<table cellpadding="5" style="font-family: arial; font-size: 20px;" cellspacing="0" border="1" width="100%" id="x">
							<?php
							

							echo '<tr style="height:10px;">';
							echo '<th align="center">Advance Category</th>';
							echo '<th align="center">Kwh</th>';
							echo '<th align="center">Rate<br/>(US $)</th>';
							echo '<th align="center">Amount<br/>(US $)</th>';
							echo '</tr>';

							echo '<tr style="height:90px;">';
							echo '<td>Import Advance</td>';
							echo '<td align="right">'.number_format($q1-$q2,2).'</td>';
							echo '<td align="right">'.($shoulder).'</td>';
							echo '<td align="right">'.number_format($shoulder*($q1-$q2), 2).'</td>';
							echo '</tr>';
							
							/*
							echo '<tr style="height:90px;">';
							echo '<td>Total</td>';
							echo '<td align="right">'.number_format($q1-$q2,2).'</td>';
							echo '<td align="right"></td>';
							echo '<td align="right">'.number_format($shoulder*($q1-$q2), 2).'</td>';
							echo '</tr>';
							
							*/

							?>

							<tr>
								<td colspan="4" style="font-size:12px;"><br/>
								<?php $tot = $shoulder*($q1-$q2)*($customer_vat/100+1); if($tot<200000000000){?>
								Total in words:<br/>
								<?php 
								echo '<b style="">';
								if($symbol != '$'){
									echo number_to_words($shoulder*($q1-$q2));
								}else{
									$t = $shoulder*($q1-$q2);
									$tt = (int)$t;
									if($tt != $t){
										$d = round($t - $tt,2);

										echo number_to_words($t);
										if(!empty($d)){
											echo ' Point ';
											$g = $d*10;
											$g2 = (int)$g;
											echo number_to_words($g2);
											echo ' ';
											echo number_to_words((int)(($g-$g2)*10));
										}
									}else{
										echo number_to_words($t);
									}
								}
								echo " $currency".'</b>'; 
								?><br/>
								</td>
								<?php } ?>
							</tr>
							<?php if($oi_exchange_rate && $currency =='Dollars'){?>
							<tr>
							<td colspan="4">
								Exhange Rate:
								<?php 
								//echo ;
									echo number_format($oi_exchange_rate,2);
								echo " $currency".''; 
								?>
								</td>
							</tr>
						<?php }?>

							<?php

							echo '</table>';
							$total_amt = $tot;
							//rwanda@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
						}elseif($customer_id =='2028_OLD'){
							$db = new Db();
							//current main 1
							$customer_id = portion(3);
							$reading_time = strtotime(date("$year-$month-15"));

							$sql = "SELECT * FROM r_reading, r_rate, metering_point WHERE rea_date = '$reading_time' AND rea_cus_id = '$customer_id' AND rate_cus_id = '$customer_id' AND rate_reading_id = rea_id AND rea_mp_id = mp_id AND upper(mp_location) = 'KATUNA-MAIN'";
							$select = $db->select($sql);
							extract($select[0][0]);
							$present_main = $rate_cum_imports;

							if(empty($reading_date)){
								$reading_date = $rate_date_read;
							}

							//previous main reading
							$sql = "SELECT TOP 1 * FROM r_reading, r_rate, metering_point WHERE rea_date < '$rea_date' AND rea_id = rate_reading_id AND rea_cus_id = '$customer_id' AND rea_mp_id = mp_id AND rate_cus_id = '$customer_id' AND upper(mp_location) = 'KATUNA-MAIN' ORDER BY rea_date DESC";
							$sel = $db->select($sql);
							extract($sel[0][0]);
							$previous_main = $rate_cum_imports;


							///////////////////// check

							$sql = "SELECT * FROM r_reading, r_rate, metering_point WHERE rea_date = '$reading_time' AND rea_cus_id = '$customer_id' AND rate_cus_id = '$customer_id' AND rate_reading_id = rea_id AND rea_mp_id = mp_id AND upper(mp_location) = 'KATUNA-CHECK'";
							$select = $db->select($sql);
							extract($select[0][0]);
							//print_r($select[0][0]);
							$present_check = $rate_cum_imports;

							if(empty($reading_date)){
								$reading_date = $rate_date_read;
							}

							//previous main reading
							$sql = "SELECT TOP 1 * FROM r_reading, r_rate, metering_point WHERE rea_date < '$rea_date' AND rea_id = rate_reading_id AND rea_cus_id = '$customer_id' AND rea_mp_id = mp_id AND rate_cus_id = '$customer_id' AND upper(mp_location) = 'KATUNA-CHECK' ORDER BY rea_date DESC";
							$sel = $db->select($sql);
							extract($sel[0][0]);
							$previous_check = $rate_cum_imports;

							$advance_main = $present_main - $previous_main;
							$advance_check = $present_check - $previous_check;

							$average = ($advance_main+ $advance_check)/2;
							?>
							<style>
								#x tr td, #x tr th{
									padding:2px 10px;
								}
								#x tr th{
									text-align: center;
								}
							</style>

							<table cellpadding="5" style="font-family: arial; font-size: 16px;" cellspacing="0" border="1" width="100%" id="x">
							<?php
							
							$tm = 0;
							


							if($peak != (int)$peak){
								$peak1 = number_format($peak, 5);
							}else{
								$peak1 = $peak;
							}
							if($shoulder != (int)$shoulder){
								$shoulder1 = number_format($shoulder, 5);
							}else{
								$shoulder1 = $shoulder;
							}
							if($off_peak != (int)$off_peak){
								$off_peak1 = number_format($off_peak, 5);
							}else{
								$off_peak1 = $off_peak;
							}

							echo '<tr style="height:10px;">';
							echo '<th align="center">Description</th>';
							echo '<th align="center">QTY - Kwh</th>';
							echo '<th align="center">Rate<br/>(US $)</th>';
							echo '<th align="center">Amount<br/>(US $)</th>';
							echo '</tr>';

							//$select 
							
							if($reading_date == ""){
								$reading_date = "";
							}else{
								$reading_date = Feedback::date_c(strtotime($reading_date));
							}

							echo '<tr style="">';
							echo '<td>Reading Date: '.$reading_date.'</td>';
							echo '<td align="right"></td>';
							echo '<td align="right"></td>';
							echo '<td align="right"></td>';
							echo '</tr>';

							echo '<tr style="">';
							if($sc[1])
								echo '<td>Billing Date: '.Feedback::date_s($c[1]).'</td>';
							else
								echo '<td>Billing Date: - </td>';

							echo '<td align="right"></td>';
							echo '<td align="right"></td>';
							echo '<td align="right"></td>';
							echo '</tr>';

							echo '<tr style="">';
							echo '<td>Current Reading: (Average)</td>';
							$q1 = ($present_main+$present_check)/2/1000;
							echo '<td align="right">'.number_format($q1, 2).'</td>';
							echo '<td align="right"></td>';
							echo '<td align="right"></td>';
							echo '</tr>';

							echo '<tr style="">';
							echo '<td>Previous reading:</td>';
							$q2 = ($previous_main+$previous_check)/2/1000;
							echo '<td align="right">'.number_format($q2, 2).'</td>';
							echo '<td align="right"></td>';
							echo '<td align="right"></td>';
							echo '</tr>';

							echo '<tr style="">';
							echo '<td>Total Units Consumed (Kwh):</td>';
							echo '<td align="right">'.number_format($q1-$q2, 2).'</td>';
							echo '<td align="right">'.($shoulder1).'</td>';
							echo '<td align="right">'.number_format($shoulder*($q1-$q2), 2).'</td>';
							echo '</tr>';

							echo '<tr style="">';
							echo '<td colspan="3">Sub Total (VAT Exclusive)</td>';
							echo '<td align="right">'.number_format($shoulder*($q1-$q2), 2).'</td>';
							echo '</tr>';

							echo '<tr style="">';
							echo '<td colspan="3">VAT</td>';
							echo '<td align="right">'.number_format(0).'</td>';
							echo '</tr>';

							echo '<tr style="">';
							echo '<td colspan="3">Total Amount (VAT Inclusive)</td>';
							echo '<td align="right">'.number_format($shoulder*($q1-$q2), 2).'</td>';
							echo '</tr>';
							

							?>

							<tr>
								<td colspan="4" style="font-size:12px;">
								<?php $tot = $tta*($customer_vat/100+1); if($tot<200000000000){?>
								Total in words:<br/>
								<?php 
								echo '<b style="">';
								if($symbol != '$'){
									echo number_to_words($shoulder*($q1-$q2));
								}else{
									$t = $shoulder*($q1-$q2);
									$tt = (int)$t;
									if($tt != $t){
										$d = round($t - $tt,2);

										echo number_to_words($t);
										if(!empty($d)){
											echo ' Point ';
											$g = $d*10;
											$g2 = (int)$g;
											echo number_to_words($g2);
											echo ' ';
											echo number_to_words((int)(($g-$g2)*10));
										}
									}else{
										echo number_to_words($t);
									}
								}
								echo " $currency".'</b>'; 
								?>
								</td>
								<?php } ?>
							</tr>
							<?php if($oi_exchange_rate && $currency =='Dollars'){?>
							<tr>
							<td colspan="4">
								Exhange Rate:
								<?php 
								//echo ;
									echo number_format($oi_exchange_rate,2);
								echo " $currency".''; 
								?>
								</td>
							</tr>
						<?php }?>

							<?php

							echo '</table>';
						}elseif($customer_id ==2029){ //TANESCO@@@@@@@@@@@@@@@@@@@@@
							
							$db = new Db();
							//current main 1
							$customer_id = $id;
							// $customer_id = $set_customer;
							$reading_time = strtotime(date("$year-$month-15"));

							$sql = "SELECT * FROM r_reading, r_rate, metering_point WHERE rea_date = '$reading_time' AND rea_cus_id = '$customer_id' AND rate_cus_id = '$customer_id' AND rate_reading_id = rea_id AND rea_mp_id = mp_id";
							$select = $db->select($sql);

							$total_advance = 0;

							foreach($select[0] as $row){
							
								extract($row);
								$present_main = $rate_cum_imports;
								$present_main_ex = $rate_cum_exports;
								$mp_location123 = $mp_location;

								//previous main reading
								$sql = "SELECT TOP 1 * FROM r_reading, r_rate, metering_point WHERE rea_date < '$rea_date' AND rea_id = rate_reading_id AND rea_cus_id = '$customer_id' AND rea_mp_id = '$mp_id' AND rate_cus_id = '$customer_id' ORDER BY rea_date DESC";
								$sel = $db->select($sql);
								extract($sel[0][0]);
								$previous_main = $rate_cum_imports;
								$previous_main_ex = $rate_cum_exports;

								$total_advance += (($present_main-$previous_main) - ($present_main_ex-$previous_main_ex))/1000;
							}

							$total_advance;
									
							$average = $total_advance;
							
							?>
							<style>
								#x tr td, #x tr th{
									padding:2px 10px;
								}
								#x tr th{
									text-align: center;
								}
							</style>
							<table cellpadding="5" style="font-family: arial; font-size: 16px;" cellspacing="0" border="1" width="100%" id="x">
							<?php
							
							$tm = 0;
							


							if($peak != (int)$peak){
								$peak1 = number_format($peak, 5);
							}else{
								$peak1 = $peak;
							}
							if($shoulder != (int)$shoulder){
								$shoulder1 = number_format($shoulder, 5);
							}else{
								$shoulder1 = $shoulder;
							}
							if($off_peak != (int)$off_peak){
								$off_peak1 = number_format($off_peak, 5);
							}else{
								$off_peak1 = $off_peak;
							}


							//$shoulder = $this->monthTariff($year, $month);

							if($shoulder != (int)$shoulder){
								$shoulder1 = number_format($shoulder, 5);
							}else{
								$shoulder1 = $shoulder;
							}
							if($off_peak != (int)$off_peak){
								$off_peak1 = number_format($off_peak, 5);
							}else{
								$off_peak1 = $off_peak;
							}



							$ta = $average;

							echo '<tr style="height:10px;">';
							echo '<th align="center">Advance Category</th>';
							echo '<th align="center">Kwh</th>';
							echo '<th align="center">Rate<br/>(US $)</th>';
							echo '<th align="center">Amount<br/>(US $)</th>';
							echo '</tr>';

							echo '<tr style="height:90px;">';
							echo '<td>Net Energy Advance</td>';
							echo '<td align="right">'.number_format($ta).'</td>';
							echo '<td align="right">'.($shoulder1).'</td>';
							echo '<td align="right">'.number_format($shoulder*$ta, 2).'</td>';
							echo '</tr>';


							$total_amt = $shoulder*$average;
							?>

							<tr>
								<td colspan="4" style="font-size:12px;">
								<?php $tot = $tta*($customer_vat/100+1); if($tot<200000000000){?>
								Total in words:<br/>
								<?php 
								echo '<b style="">';
								if($symbol != '$'){
									echo number_to_words($shoulder*($ta));
								}else{
									$t = $shoulder*($ta);
									$tt = (int)$t;
									if($tt != $t){
										$d = round($t - $tt,2);

										echo number_to_words($t);
										if(!empty($d)){
											echo ' And ';
											$g = $d*10;
											$g2 = (int)$g;
											echo number_to_words($g2);
											echo ' ';
											echo number_to_words((int)(($g-$g2)*10));
										}
									}else{
										echo number_to_words($t);
									}
								}
								echo " $currency".'</b>'; 
								?>
								</td>
								<?php } ?>
							</tr>
							<?php if($oi_exchange_rate && $currency =='Dollars'){?>
							<tr>
							<td colspan="4">
								Exhange Rate:
								<?php 
								//echo ;
									echo number_format($oi_exchange_rate,2);
								echo " $currency".''; 
								?>
								</td>
							</tr>
						<?php }?>

							<?php

							echo '</table>'; //TANESCO @@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
						}elseif($customer_id =='2029_old'){ //TANESCO@@@@@@@@@@@@@@@@@@@@@
							$db = new Db();
							//current main 1
							$customer_id = $id;//portion(3);
							$reading_time = strtotime(date("$year-$month-15"));

							$sql = "SELECT * FROM r_reading, r_rate, metering_point WHERE rea_date = '$reading_time' AND rea_cus_id = '$customer_id' AND rate_cus_id = '$customer_id' AND rate_reading_id = rea_id AND rea_mp_id = mp_id AND upper(mp_location) = 'TANESCO'";
							$select = $db->select($sql);
							extract($select[0][0]);
							$current_main = $rate_cum_imports;

							if(empty($reading_date)){
								$reading_date = $rate_date_read;
							}

							//previous main reading
							$sql = "SELECT TOP 1 * FROM r_reading, r_rate, metering_point WHERE rea_date < '$rea_date' AND rea_id = rate_reading_id AND rea_cus_id = '$customer_id' AND rea_mp_id = mp_id AND rate_cus_id = '$customer_id' AND upper(mp_location) = 'TANESCO' ORDER BY rea_date DESC";
							$sel = $db->select($sql);
							extract($sel[0][0]);
							$previous_main = $rate_cum_imports;


							$average = ($advance_main+ $advance_check)/2;
							?>
							<style>
								#x tr td, #x tr th{
									padding:2px 10px;
								}
								#x tr th{
									text-align: center;
								}
							</style>
							<table cellpadding="5" style="font-family: arial; font-size: 16px;" cellspacing="0" border="1" width="100%" id="x">
							<?php
							
							$tm = 0;
							


							if($peak != (int)$peak){
								$peak1 = number_format($peak, 5);
							}else{
								$peak1 = $peak;
							}
							if($shoulder != (int)$shoulder){
								$shoulder1 = number_format($shoulder, 5);
							}else{
								$shoulder1 = $shoulder;
							}
							if($off_peak != (int)$off_peak){
								$off_peak1 = number_format($off_peak, 5);
							}else{
								$off_peak1 = $off_peak;
							}

							echo '<tr style="height:10px;">';
							echo '<th align="center">Description</th>';
							echo '<th align="center">QTY - Kwh</th>';
							echo '<th align="center">Rate<br/>(US $)</th>';
							echo '<th align="center">Amount<br/>(US $)</th>';
							echo '</tr>';

							//$select 
							
							if($reading_date == ""){
								$reading_date = "";
							}else{
								$reading_date = Feedback::date_c(strtotime($reading_date));
							}

							echo '<tr style="">';
							echo '<td>Reading Date: '.$reading_date.'</td>';
							echo '<td align="right"></td>';
							echo '<td align="right"></td>';
							echo '<td align="right"></td>';
							echo '</tr>';

							echo '<tr style="">';
							if($sc[1])
								echo '<td>Billing Date: '.Feedback::date_s($c[1]).'</td>';
							else
								echo '<td>Billing Date: - </td>';

							echo '<td align="right"></td>';
							echo '<td align="right"></td>';
							echo '<td align="right"></td>';
							echo '</tr>';

							echo '<tr style="">';
							echo '<td>Current Reading: </td>';
							$q1 = ($current_main/1000);
							echo '<td align="right">'.number_format($q1, 2).'</td>';
							echo '<td align="right"></td>';
							echo '<td align="right"></td>';
							echo '</tr>';

							echo '<tr style="">';
							echo '<td>Previous reading:</td>';
							$q2 = ($previous_main/1000);
							echo '<td align="right">'.number_format($q2, 2).'</td>';
							echo '<td align="right"></td>';
							echo '<td align="right"></td>';
							echo '</tr>';

							$a1 = array(1,2,3);
							$a2 = array(4,5,6);
							$a3 = array(7,8,9);
							$a4 = array(10,11,12);

							$use = "";

							//$year;

							if(in_array($month, $a1)){
								$use = "a4";
								$year -= 1;
								$factor_month = $$use[0];
							}elseif(in_array($month, $a2)){
								$use = "a1";
								$factor_month = $$use[0];
							}elseif(in_array($month, $a3)){
								$use = "a2";
								$factor_month = $$use[0];
							}elseif(in_array($month, $a4)){
								$use = "a3";
								$factor_month = $$use[0];
							}
							$e = 0;
							foreach($$use as $m){
								$e += $this->sum($year, $m, 0);
							}

							$shoulder = $e/3/100;
							$shoulder1 = number_format($shoulder,6);

							echo '<tr style="">';
							echo '<td>Total Units Consumed (Kwh):</td>';
							echo '<td align="right">'.number_format($q1-$q2, 2).'</td>';
							echo '<td align="right">'.($shoulder1).'</td>';
							echo '<td align="right">'.number_format($shoulder*($q1-$q2), 2).'</td>';
							echo '</tr>';

							echo '<tr style="">';
							echo '<td colspan="3">Sub Total (VAT Exclusive)</td>';
							echo '<td align="right">'.number_format($shoulder*($q1-$q2), 2).'</td>';
							echo '</tr>';

							echo '<tr style="">';
							echo '<td colspan="3">VAT</td>';
							echo '<td align="right">'.number_format(0).'</td>';
							echo '</tr>';

							echo '<tr style="">';
							echo '<td colspan="3">Total Amount (VAT Inclusive)</td>';
							echo '<td align="right">'.number_format($shoulder*($q1-$q2), 2).'</td>';
							echo '</tr>';
							$total_amt = $shoulder*($q1-$q2);
							?>

							<tr>
								<td colspan="4" style="font-size:12px;">
								<?php $tot = $tta*($customer_vat/100+1); if($tot<200000000000){?>
								Total in words:<br/>
								<?php 
								echo '<b style="">';
								if($symbol != '$'){
									echo number_to_words($shoulder*($q1-$q2));
								}else{
									$t = $shoulder*($q1-$q2);
									$tt = (int)$t;
									if($tt != $t){
										$d = round($t - $tt,2);

										echo number_to_words($t);
										if(!empty($d)){
											echo ' Point ';
											$g = $d*10;
											$g2 = (int)$g;
											echo number_to_words($g2);
											echo ' ';
											echo number_to_words((int)(($g-$g2)*10));
										}
									}else{
										echo number_to_words($t);
									}
								}
								echo " $currency".'</b>'; 
								?>
								</td>
								<?php } ?>
							</tr>

							<?php

							echo '</table>'; //TANESCO @@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
						}elseif($customer_id == 2030){ //KPLC @@@@@@@@@@@@@@@@@@@@@@@@@@
							//echo ">>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>";
							?>
							<style>
								#x tr td, #x tr th{
									padding:2px 10px;
								}
								#x tr th{
									text-align: center;
								}
							</style>
							<table cellpadding="5" style="font-family: arial; font-size: 20px;" cellspacing="0" border="1" width="100%" id="x">
							<?php
							$a = $this->KPLC($year, $month);
							$tm = 0;
							for($i=0; $i<3; $i++)
								$tm += $a['main'][$i];

							$tc = 0;
							for($i=0; $i<3; $i++)
								$tc += $a['check'][$i];

							$ta = ($tm + $tc)/(2*1000);

							if($peak != (int)$peak){
								$peak1 = number_format($peak, 5);
							}else{
								$peak1 = $peak;
							}

							$shoulder = $this->monthTariff($year, $month);//LINK FOR KPLC

							if($shoulder != (int)$shoulder){
								$shoulder1 = number_format($shoulder, 5);
							}else{
								$shoulder1 = $shoulder;
							}
							if($off_peak != (int)$off_peak){
								$off_peak1 = number_format($off_peak, 5);
							}else{
								$off_peak1 = $off_peak;
							}



							echo '<tr style="height:10px;">';
							echo '<th align="center">Advance Category</th>';
							echo '<th align="center">Kwh</th>';
							echo '<th align="center">Rate<br/>(US $)</th>';
							echo '<th align="center">Amount<br/>(US $)</th>';
							echo '</tr>';

							echo '<tr style="height:90px;">';
							echo '<td>Net Energy Advance</td>';
							echo '<td align="right">'.number_format($ta).'</td>';
							echo '<td align="right">'.($shoulder1).'</td>';
							echo '<td align="right">'.number_format($shoulder*$ta, 2).'</td>';
							echo '</tr>';

							/*
							echo '<tr style="height:90px;">';
							echo '<td>Total</td>';
							echo '<td align="right">'.number_format($ta).'</td>';
							echo '<td align="right"></td>';
							echo '<td align="right">'.number_format($shoulder*$ta, 2).'</td>';
							echo '</tr>';
							*/

							?>

							<tr>
								<td colspan="4" style="font-size:12px;"><br/>
								<?php $tot = $tta*($customer_vat/100+1); 

								if($tot<200000000000){?>
								Total in words:<br/>
								<?php 
								echo '<b style="">';
								if($symbol != '$'){
									echo number_to_words($shoulder*$ta);
								}else{
									$t = $shoulder*$ta;
									$tt = (int)$t;
									if($tt != $t){
										$d = round($t - $tt,2);

										echo number_to_words($t);
										if(!empty($d)){
											echo ' Point ';
											$g = $d*10;
											$g2 = (int)$g;
											echo number_to_words($g2);
											echo ' ';
											echo number_to_words((int)(($g-$g2)*10));
										}
									}else{
										echo number_to_words($t);
									}
								}
								echo " $currency".'</b>'; 
								?><br/>
								</td>
								<?php } ?>
							</tr>
							<?php if($oi_exchange_rate && $currency =='Dollars'){?>
							<tr>
							<td colspan="4">
								Exhange Rate:
								<?php 
								//echo ;
									echo number_format($oi_exchange_rate,2);
								echo " $currency".''; 
								?>
								</td>
							</tr>
						<?php }?>

							<?php
							
							echo '</table>';
							$total_amt = $shoulder*$ta;
							//echo ">>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>".$tot;

						}elseif($customer_id == 2035){ //MIRAMA @@@@@@@@@@@@@@@@@@@@@@@@@@
							//echo ">>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>";
							?>
							<style>
								#x tr td, #x tr th{
									padding:2px 10px;
								}
								#x tr th{
									text-align: center;
								}
							</style>
							<table cellpadding="5" style="font-family: arial; font-size: 20px;" cellspacing="0" border="1" width="100%" id="x">
							<?php
							$a = $this->MIRAMA($year, $month);
							$tm = 0;

							$ta = $a/1000; //($tm + $tc)/(2*1000);

							$tou = $this->tou($customer_id, $month, $year);

							$peak = $tou[0];
							$shoulder = $tou[1];
							$off_peak = $tou[2];

							if($shoulder != (int)$shoulder){
								$shoulder1 = number_format($shoulder, 5);
							}else{
								$shoulder1 = $shoulder;
							}
							if($off_peak != (int)$off_peak){
								$off_peak1 = number_format($off_peak, 5);
							}else{
								$off_peak1 = $off_peak;
							}

							$t = new Db();
							$reading_time = strtotime(date("$year-$month-15"));
							$sql = "SELECT mp_location,mp_id,mp_region_id,rea_mp_id, rea_date, rea_id FROM metering_point, r_reading WHERE mp_customer_id = '$customer_id' AND rea_cus_id = '$customer_id' AND mp_id = rea_mp_id AND rea_date = '$reading_time' ORDER BY mp_location ASC";
							$sel = $db->select($sql);
							
							$sha1 = array();
							$sha2 = array();
							
							$xx1 = $xx2 = $xx3 = 0;
							foreach($sel[0] as $r){
								extract($r);
								$cal = rate_calculate($customer_id, ($rea_date), $mp_id); 

								$sha1[] = $cal[33];
								//$sha2[] = $cal[34];
								$xx1 += $cal[33]/1000;
								$xx2 += $cal[34]/1000;
								$xx3 += ($cal[33] - $cal[34])/1000;
							}
							
							$s1 = $sha1[1] - $sha1[0];
							//$s2 = $sha2[1] - $sha2[0];	
							$to_imp =abs($xx1);
							
						

							echo '<tr style="height:10px;">';
							echo '<th align="center">Advance Category</th>';
							echo '<th align="center">Kwh</th>';
							echo '<th align="center">Rate<br/>(US $)</th>';
							echo '<th align="center">Amount<br/>(US $)</th>';
							echo '</tr>';

							echo '<tr style="height:90px;">';
							echo '<td>Import Advance</td>';
							echo '<td align="right">'.number_format($to_imp).'</td>';
							echo '<td align="right">'.($shoulder1).'</td>';
							echo '<td align="right">'.number_format($shoulder*$to_imp, 2).'</td>';
							echo '</tr>';

							/*
							echo '<tr style="height:90px;">';
							echo '<td>Total</td>';
							echo '<td align="right">'.number_format($ta).'</td>';
							echo '<td align="right"></td>';
							echo '<td align="right">'.number_format($shoulder*$ta, 2).'</td>';
							echo '</tr>';
							*/

							?>

							<tr>
								<td colspan="4" style="font-size:12px;"><br/>
								<?php $tot = $tta*($customer_vat/100+1); 

								if($tot<200000000000){?>
								Total in words:<br/>
								<?php 
								echo '<b style="">';
								if($symbol != '$'){
									echo number_to_words($shoulder*$to_imp);
								}else{
									$t = $shoulder*$to_imp;
									$tt = (int)$t;
									if($tt != $t){
										$d = round($t - $tt,2);

										echo number_to_words($t);
										if(!empty($d)){
											echo ' Point ';
											$g = $d*10;
											$g2 = (int)$g;
											echo number_to_words($g2);
											echo ' ';
											echo number_to_words((int)(($g-$g2)*10));
										}
									}else{
										echo number_to_words($t);
									}
								}
								echo " $currency".'</b>'; 
								?><br/>
								</td>
								<?php } ?>
							</tr>
							<?php if($oi_exchange_rate && $currency =='Dollars'){?>
							<tr>
							<td colspan="4">
								Exhange Rate:
								<?php 
								//echo ;
									echo number_format($oi_exchange_rate,2);
								echo " $currency".''; 
								?>
								</td>
							</tr>
						<?php }?>

							<?php
							
							echo '</table>';
							$total_amt = $shoulder*$to_imp;
							//echo ">>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>".$tot;

						}elseif($id == 201701){	
						//echo 'her';

						//=============== WHEELING CHARGE BANDS =====================================
						$db = new Db();
						$select = $db->select("SELECT * FROM wheeling_charge WHERE wc_customer_id = '$customer_id' ORDER BY wc_id ASC");
						$assigned2 = 0;
						foreach($select[0] as $row){
							extract($row);
							if(date('Y', $wc_date_added)==$year && date('m', $wc_date_added)==$month)
							{
								$cwc = $wc_rate;
								$assigned2 = 1;
							}
						}

						if($assigned2 == 0){
							$db = new Db();
							$select = $db->select("SELECT * FROM wheeling_charge WHERE wc_customer_id = '$customer_id' ORDER BY wc_id ASC");
							$assigned = 0;
							foreach($select[0] as $row){
								extract($row);
								//if(date('Y', $wc_date_added)==$year && date('m', $wc_date_added)==$month)
								{
									$cwc = $wc_rate;
									$assigned2 = 1;
								}
							}
						}
						
						$t1 = $t2 = $t3 = $t4 = 0;
							$units = 0.001;
						$m = $this->sumFormulae($customer_id, $year, $month);
						//echo '->'.$m[0].'->'.$units;
						$t1 = ($m[0]*$units);
						$t2 = ($m[1]*$units);
						$t3 = ($m[2]*$units);
						$t4 = ($m[3]*$units);

						$h = $this->wheelingChargeSum($customer_id, $year, $month);
						$tbf = $this->totalBackflow($id, $year, $month);
						$ttu = $tta = 0;
						//echo $year;
						?>
						<style>
							#x tr td, #x tr th{
								padding:2px 10px;
							}
							#x tr th{
								text-align: center;
							}
						</style>
						<table cellpadding="5" style="font-family: arial; font-size: 10pt;" cellspacing="0" border="1" width="100%" id="x">
							<tr>
								<th>TIME BAND</th>
								<th>UNITS - 
									<?php
									echo '(';
									if($units == 1){
										echo 'Wh';
									}elseif($units == 0.001){
										echo 'KWh';
									}elseif($units == 0.000001){
										echo 'MWh';
									}
									echo ')';
									?>
								</th>
								<th>RATE - <?php echo "($symbol / kWh)"; ?></th>
								<th>AMOUNT<?php echo " ($symbol)"; ?></th>
							</tr>
							<tr>

								<?php 
								
								
								if($peak != (int)$peak){
									$peak1 = number_format($peak, 6);
								}else{
									$peak1 = $peak;
								}
								if($shoulder != (int)$shoulder){
									$shoulder1 = number_format($shoulder, 6);
								}else{
									$shoulder1 = $shoulder;
								}
								if($off_peak != (int)$off_peak){
									$off_peak1 = number_format($off_peak, 6);
								}else{
									$off_peak1 = $off_peak;
								}

								$o = $peak;
								$ttu += ($t2-$tbf[1]);
								$tta += $o*($t2-$tbf[1]);
								?>
								<?php 
								
								$a1 = array(1,2,3);
								$a2 = array(4,5,6);
								$a3 = array(7,8,9);
								$a4 = array(10,11,12);

								$use = "";

								//$year;

								if(in_array($month, $a1)){
									$use = "a4";
									$year -= 1;
									$factor_month = $$use[0];
								}elseif(in_array($month, $a2)){
									$use = "a1";
									$factor_month = $$use[0];
								}elseif(in_array($month, $a3)){
									$use = "a2";
									$factor_month = $$use[0];
								}elseif(in_array($month, $a4)){
									$use = "a3";
									$factor_month = $$use[0];
								}
								$e = 0;
								foreach($$use as $m){
									$e += $this->sum($year, $m, 0);
								}

								$shoulder = $peak = $off_peak = $e/3/100*1.05;
								
								if($peak != (int)$peak){
									$peak1 = number_format($peak, 6);
								}else{
									$peak1 = $peak;
								}
								if($shoulder != (int)$shoulder){
									$shoulder1 = number_format($shoulder, 6);
								}else{
									$shoulder1 = $shoulder;
								}
								if($off_peak != (int)$off_peak){
									$off_peak1 = number_format($off_peak, 6);
								}else{
									$off_peak1 = $off_peak;
								}

								$o = $peak;
								$ttu += ($t2-$tbf[1]);
								$tta += $o*($t2-$tbf[1]);
								?>
								<?php 
									if($symbol == '$'){
										$a = number_format($o*($t2-$tbf[1]),2);
									}else{
										$a = number_format($o*($t2-$tbf[1]));
									}
								?>
								<td>Peak</td>
								<td align="right"><?php echo number_format($t2-$tbf[1],2); ?></td>
								<td align="right"><?php echo ((float)$peak1). ""; ?></td>
								<td align="right"><?php echo $a. ""; ?></td>
							</tr>
							<tr>
								<?php 
								$o = $shoulder;
								$ttu += ($t1-$tbf[0]);
								$tta += $o*($t1-$tbf[0]);
								?>
								<?php 
									if($symbol == '$'){
										$a = number_format($o*($t1-$tbf[0]),2);
									}else{
										$a = number_format($o*($t1-$tbf[0]));
									}
								?>
								<td>Shoulder</td>
								<td align="right"><?php echo number_format(($t1-$tbf[0]),2); ?></td>
								<td align="right"><?php echo ((float)$shoulder1). ""; ?></td>
								<td align="right"><?php echo $a. ""; ?></td>
							</tr>
							<tr>
								<td>Off Peak</td>
								<?php 
								$o = $off_peak;
								$ttu += ($t3-$tbf[2]);
								$tta += $o*($t3-$tbf[2]);
								?>
								<?php 
									if($symbol == '$'){
										$a = number_format($o*($t3-$tbf[2]),2);
									}else{
										$a = number_format($o*($t3-$tbf[2]));
									}
								?>
								<td align="right"><?php echo number_format(($t3-$tbf[2]),2); ?></td>
								<td align="right"><?php echo ((float)$off_peak1). " "; ?></td>
								<td align="right"><?php echo $a. " "; ?></td>
							</tr>
							<tr>
								<td>Total</td>
								<td align="right"><?php echo number_format($ttu,2); ?></td>
								<?php 
									if($symbol == '$'){
										$a = number_format($tta,2);
									}else{
										$a = number_format($tta);
									}
								?>
								<td></td>
								<td align="right"><?php echo $a. " "; ?></td>
							</tr>
							<?php


							?>
							<tr>
								<td rowspan="2">Wheeling Charges</td>
								<td>Payable To <b>(Add)</b></td>
								<td><?php 
								//echo $year2.' => '.$month.' => '.$customer_id;
								$payable_ = $this->total_payable($year2, $month, $customer_id);
								$payable = $payable_['payable'];
								echo '<b>'.$payable_['customers'].'</b>';
								?></td>

								<td align="right"><?php  echo ($symbol == '$')? number_format($payable,2):number_format($payable); ?></td>
								<!-- <td colspan="2">Collectable(Less)</td> -->
								<!-- <td align="right"><?php // $payable = array_sum($h[0]); echo number_format($payable). " $symbol"; ?></td> -->

							</tr>
							<tr>
								<td>Collectable From <b>(Less)</b></td>
								<td style="width:40%;"><?php 
								$payable_ = $this->total_collectable($year2, $month, $customer_id);
								$collectable = $payable_['collectable'];
								echo '<b>'.$payable_['customers'].'</b>';
								?></td>
								<td align="right"><?php echo ($symbol == '$')?'('.number_format($collectable,2). ") ":'('.number_format($collectable). ") "; ?></td>
							</tr>
							
							<?php 
							$tbf = $this->totalBackflow($id, $year2, $month);
							if($customer_id == 2019 && 0){ 

							?>
							<tr>
								<td colspan="2">Backflows</td>
								<td align="right"><?php echo number_format($tbf[1],2); ?></td>
								<td align="right">(<?php echo number_format($tbf[5]); ?>)</td>
							</tr>
							<?php
							} 
							?>
							<?php $tta += $payable+($collectable*-1); ?>
							<tr>
								<td colspan="3">Total (VAT Exclusive)</td>
								<td align="right"><?php echo ($symbol=='$')?number_format($tta,2):number_format($tta); ?></td>
							</tr>							
							<tr>
								<td colspan="3">Add <?php echo ($symbol == '$')?number_format($customer_vat,2):number_format($customer_vat); ?>% VAT</td>
								<?php $tot = $customer_vat/100*$tta; ?>
								<td align="right"><?php echo number_format($tot). " "; ?></td>
							</tr>							
							<tr>
								<td colspan="3"><b>Total Amount (VAT Incl)</b></td>
								<?php $tot = $customer_vat/100*$tta; ?>
								<td align="right"><b><?php echo ($symbol == '$')?number_format($tta*($customer_vat/100+1), 2): number_format($tta*($customer_vat/100+1)); ?></b></td>							
							<tr>
								<td colspan="4">
								<?php $tot = $tta*($customer_vat/100+1); if($tot<200000000000){?>
								Total in words:<br/>
								<?php 
								echo '<b>';
								if($symbol != '$'){
									echo number_to_words($tta*($customer_vat/100+1));
								}else{
									$t = $tta*($customer_vat/100+1);
									$tt = (int)$t;
									if($tt != $t){
										$d = round($t - $tt,2);

										echo number_to_words($t);
										if(!empty($d)){
											echo ' Point ';
											$g = $d*10;
											$g2 = (int)$g;
											echo number_to_words($g2);
											echo ' ';
											echo number_to_words((int)(($g-$g2)*10));
										}
									}else{
										echo number_to_words($t);
									}
								}
								echo " $currency".'</b>'; 
								?>
								</td>
								<?php } ?>
							</tr>
							<?php if($oi_exchange_rate && $currency =='Dollars'){?>
							<tr>
							<td colspan="4">
								Exhange Rate:
								<?php 
								//echo ;
									echo number_format($oi_exchange_rate,2);
								echo " $currency".''; 
								?>
								</td>
							</tr>
						<?php }?>
						</table>


						<?php
						$total_amt = $tot;
						echo'<table style="" class="" cellspacing="10">';
					     
					       $item_id = 3180;
							$buyer = $this->rgf("customer",$customer_id,"customer_id","customer_short_name");
							$tin = $this->rgf("customer",$customer_id,"customer_id","customer_tin");
							$qr = '<img style="text-align:center;width:150px;" src="'.return_url().'efris_qr_images/'.$rea_qr.'" alt="'.$rea_fdn.'"/>';
							$item = $this->rgf("efris_goods", $item_id, "eg_id", "eg_name");
							$vc = $rea_vc;
							$fdn = $rea_fdn;

							if($fdn){
							echo '<tr valign="top" style="border-left:none;">';
							echo '<td colspan="5" style="border-left:none;border-right:none;">';
							echo '<br/><B>URA EFRIS DETAILS:</B><BR/>';
							echo '<table>';
							echo '<tr>';
							echo '<td>Fiscal Document Number:</td>';
							echo '<td>'.$rea_fdn.'</td>';
							echo '</tr>';
							echo '<tr>';
							echo '<td>Verification Code:</td>';
							echo '<td>'.$rea_vc.'</td>';
							echo '</tr>';
							echo '</table><br/>';
							echo '</td>';
							echo '<td colspan="2" class="pull-right" style="border-left:none;border-right:none;"><br/><center>'.$qr.'<br/></center></td>';
							echo '</tr>';
						    }
							
						echo'</table>';

						 }else if($customer_id == 2036){

						 }else if($customer_id == 2041){

						 }else{	
						//invoices-else

						//=============== WHEELING CHARGE BANDS =====================================
						$db = new Db();
						$select = $db->select("SELECT * FROM wheeling_charge WHERE wc_customer_id = '$customer_id' ORDER BY wc_id ASC");
						$assigned2 = 0;
						foreach($select[0] as $row){
							extract($row);
							if(date('Y', $wc_date_added)==$year && date('m', $wc_date_added)==$month)
							{
								$cwc = $wc_rate;
								$assigned2 = 1;
							}
						}

						if($assigned2 == 0){
							$db = new Db();
							$select = $db->select("SELECT * FROM wheeling_charge WHERE wc_customer_id = '$customer_id' ORDER BY wc_id ASC");
							$assigned = 0;
							foreach($select[0] as $row){
								extract($row);
								//if(date('Y', $wc_date_added)==$year && date('m', $wc_date_added)==$month)
								{
									$cwc = $wc_rate;
									$assigned2 = 1;
								}
							}
						}
						
						$t1 = $t2 = $t3 = $t4 = 0;
							$units = 0.001;
						$m = $this->sumFormulae($customer_id, $year, $month);
						//echo '->'.$m[0].'->'.$units;
						$t1 = ($m[0]*$units);
						$t2 = ($m[1]*$units);
						$t3 = ($m[2]*$units);
						$t4 = ($m[3]*$units);

						$h = $this->wheelingChargeSum($customer_id, $year, $month);
						$tbf = $this->totalBackflowInvoice($id, $year, $month);
						$ttu = $tta = 0;
						?>
						<style>
							#x tr td, #x tr th{
								padding:2px 10px;
							}
							#x tr th{
								text-align: center;
							}
						</style>

						<table cellpadding="5" style="font-family: arial; font-size: 10pt;" cellspacing="0" border="1" width="100%" id="x">
							<tr>
								<th>TIME BAND</th>
								<th>UNITS - 
									<?php
									echo '(';
									if($units == 1){
										echo 'Wh';
									}elseif($units == 0.001){
										echo 'KWh';
									}elseif($units == 0.000001){
										echo 'MWh';
									}
									echo ')';
									?>
								</th>
								<th>RATE - <?php echo "($symbol / kWh)"; ?></th>
								<th>AMOUNT<?php echo " ($symbol)"; ?></th>
							</tr>
							<tr>

								<?php 
								if($peak != (int)$peak){
									$peak1 = number_format($peak, 5);
								}else{
									$peak1 = $peak;
								}
								if($shoulder != (int)$shoulder){
									$shoulder1 = number_format($shoulder, 5);
								}else{
									$shoulder1 = $shoulder;
								}
								if($off_peak != (int)$off_peak){
									$off_peak1 = number_format($off_peak, 5);
								}else{
									$off_peak1 = $off_peak;
								}

								$o = $peak;
								$ttu += ($t2-$tbf[1]);
								$tta += $o*($t2-$tbf[1]);
								?>
								<?php 
									if($symbol == '$'){
										$a = number_format($o*($t2-$tbf[1]),2);
									}else{
										$a = number_format($o*($t2-$tbf[1]));
									}
									$peak_value = $a;
								?>
								<td>Peak</td>
								<td align="right"><?php echo number_format($t2-$tbf[1],2); ?></td>
								<td align="right"><?php echo ((float)$peak1). ""; ?></td>
								<td align="right"><?php echo $a. ""; ?></td>
							</tr>
							<tr>
								<?php 
								$o = $shoulder;
								$ttu += ($t1-$tbf[0]);
								$tta += $o*($t1-$tbf[0]);
								?>
								<?php 
									if($symbol == '$'){
										$a = number_format($o*($t1-$tbf[0]),2);
									}else{
										$a = number_format($o*($t1-$tbf[0]));
									}
									$shoulder_value = $a;
								?>
								<td>Shoulder</td>
								<td align="right"><?php echo number_format(($t1-$tbf[0]),2); ?></td>
								<td align="right"><?php echo ((float)$shoulder1). ""; ?></td>
								<td align="right"><?php echo $a. ""; ?></td>
							</tr>
							<tr>
								<td>Off Peak</td>
								<?php 
								$o = $off_peak;
								$ttu += ($t3-$tbf[2]);
								$tta += $o*($t3-$tbf[2]);
								?>
								<?php 
									if($symbol == '$'){
										$a = number_format($o*($t3-$tbf[2]),2);
									}else{
										$a = number_format($o*($t3-$tbf[2]));
									}

									$off_peak_value = $a;
								?>
								<td align="right"><?php echo number_format(($t3-$tbf[2]),2); ?></td>
								<td align="right"><?php echo ((float)$off_peak1). " "; ?></td>
								<td align="right"><?php echo $a. " "; ?></td>
							</tr>
							<tr>
								<td>Total</td>
								<td align="right"><?php echo number_format($ttu,2); ?></td>
								<?php 
									if($symbol == '$'){
										$a = number_format($tta,2);
									}else{
										$a = number_format($tta);
									}
								?>
								<td></td>
								<td align="right"><?php echo $a. " "; ?></td>
							</tr>
							<?php

							?>
							<tr>
								<td rowspan="2">Wheeling Charges</td>
								<td>Payable To <b>(Add)</b></td>
								<td><?php 
								$payable_ = $this->total_payable($year, $month, $customer_id);
								$payable = $payable_['payable'];
								echo '<b>'.$payable_['customers'].'</b>';
								?></td>

								<td align="right"><?php  echo ($symbol == '$')? number_format($payable,2):number_format($payable); ?></td>
								<?php $payable_value = number_format($payable);?>

								<!-- <td colspan="2">Collectable(Less)</td> -->
								<!-- <td align="right"><?php // $payable = array_sum($h[0]); echo number_format($payable). " $symbol"; ?></td> -->

							</tr>
							<tr>
								<td>Collectable From <b>(Less)</b></td>
								<td style="width:40%;"><?php 
								$payable_ = $this->total_collectable($year, $month, $customer_id);
								$collectable = $payable_['collectable'];
								echo '<b>'.$payable_['customers'].'</b>';
								?></td>
								<td align="right"><?php echo ($symbol == '$')?'('.number_format($collectable,2). ") ":'('.number_format($collectable). ") "; ?></td>
								<?php $collectable_value = number_format($collectable);?>
							</tr>
							<?php
							$tlf_ = $this->total_tlf($year, $month, $customer_id);
							$tlf = $tlf_['tlf'];
							if($tlf){
							?>
							<tr>
								<td colspan="3">
									Transmission Loss Factor <b>(Less)</b>
									<?php 
								echo '<b> :'.$tlf_['customers'].'</b>';
								?></td>
								<td align="right"><?php echo ($symbol == '$')?'('.number_format($tlf,2). ") ":'('.number_format($tlf). ") "; ?></td>
							</tr>
							<?php 
							}

							$tbf = $this->totalBackflow($id, $year, $month);
							if($customer_id == 2019 && 0){ 

							?>
							<tr>
								<td colspan="2">Backflows</td>
								<td align="right"><?php echo number_format($tbf[1],2); ?></td>
								<td align="right">(<?php echo number_format($tbf[5]); ?>)</td>
							</tr>
							<?php
							} 
							?>
							<?php $tta += $payable+($collectable*-1)+($tlf*-1); ?>

							<tr>
								<td colspan="3">Total (VAT Exclusive)</td>
								<td align="right"><?php echo ($symbol=='$')?number_format($tta,2):number_format($tta); ?></td>
							</tr>
							<?php
							$dba = new Db();
							$admin_cost = 0;
							$time_readings2 = strtotime($year.'-'.$month.'-01 12:00:00 am');
							$sqla = "SELECT * FROM other_charges WHERE oc_start_date <= '$time_readings2' AND oc_end_date >= '$time_readings2' AND oc_customer_id = '$customer_id'"; 
							$selecta = $dba->select($sqla);

							if($dba->num_rows()){
								extract($selecta[0][0]);
							?>
							<tr>
								<td colspan="3">
									<?php echo $oc_charge_name; ?>
									 <b>(<?php echo $oc_type; ?>)</b>
									<?php 
								?></td>
								<td align="right"><?php echo ($symbol == '$')?''.number_format($tta*$oc_percentage/100,2). " ":''.number_format($tta*$oc_percentage/100). " "; ?></td>
							</tr>

							<tr>
								<td colspan="3">Total (VAT Exclusive + Admin Cost)</td>
								<td align="right"><?php echo ($symbol=='$')?number_format($tta+$tta*$oc_percentage/100,2):number_format($tta+$tta*$oc_percentage/100); ?></td>
							</tr>
							<?php 

							$tta += $tta*$oc_percentage/100;
							}
							?>							
							<tr>
								<td colspan="3">Add <?php echo ($symbol == '$')?number_format($customer_vat,2):number_format($customer_vat); ?>% VAT</td>
								<?php

								 $tot1 = $customer_vat/100*$tta;
								 $tot = $customer_vat/100*$tta; 
	
								 $vat_value = number_format($tot);
								?>
								<td align="right"><?php echo number_format($tot). " "; ?></td>
							</tr>							
							<tr>
								<td colspan="3"><b>Total Amount (VAT Incl)</b></td>
								<?php $tot = $customer_vat/100*$tta; ?>
								<td align="right"><b><?php echo ($symbol == '$')?number_format($tta*($customer_vat/100+1), 2): number_format($tta*($customer_vat/100+1));
								$net_value = number_format($tta*($customer_vat/100+1));

								 ?></b></td>							
							<tr>
								<td colspan="4">
								<?php $tot = $tta*($customer_vat/100+1); if($tot<200000000000){?>
								Total in words:<br/>
								<?php 
								echo '<b>';
								if($symbol != '$'){
									echo number_to_words($tta*($customer_vat/100+1));
								}else{
									$t = $tta*($customer_vat/100+1);
									$tt = (int)$t;
									if($tt != $t){
										$d = round($t - $tt,2);

										echo number_to_words($t);
										if(!empty($d)){
											echo ' Point ';
											$g = $d*10;
											$g2 = (int)$g;
											echo number_to_words($g2);
											echo ' ';
											echo number_to_words((int)(($g-$g2)*10));
										}
									}else{
										echo number_to_words($t);
									}
								}
								echo " $currency".'</b>'; 
								?>
								</td>
								<?php } ?>
							</tr>
							<?php if($oi_exchange_rate && $currency =='Dollars'){?>
							<tr>
							<td colspan="4">
								Exhange Rate:
								<?php 
								//echo ;
									echo number_format($oi_exchange_rate,2);
								echo " $currency".''; 
								?>
								</td>
							</tr>
						<?php }?>
							
						</table>
						<?php 


						$total_amt = $tta*($customer_vat/100+1);
					} ?>

						<span style="color:red; font-weight: bold; font-size: 12px; text-align: center; ">
							<?php 
							echo $this->rgf("customer", $customer_id, "customer_id", "customer_accounts_due");
							?>
						</span>						
						<?php $this->showReadingComments(); ?>	
						
						<?php $c = $this->energyApprovals(user_id(), $year, $month, 1, 1, $customer_id); ?>
						
					</td>				
				</tr>
				
			</tbody></table>
	<form method="post">
		<?php	
		$item_id = 3180;
		$symbol = $this->rgf("currency",$customer_currency, "currency_id", "currency_symbol");
		echo '<input type="hidden" id="items" value="'.$item_id.'"/>';
		echo '<input type="hidden" id="unitPrice" value="'.number_format($total_amt,2).'"/>';
		echo '<input type="hidden" id="a_currency" value="'.$symbol.'"/>';
		$customer_vat = ($customer_vat==18)?"RES":"EXP";
		echo '<input type="hidden" id="vat" value="'.$customer_vat.'"/>';
		echo '<input type="hidden" id="buyer" value="'.$customer_id.'"/>';
		echo '<input type="hidden" id="qty" value="1"/>';
		//echo '<input type="hidden" id="a_date" value="'.date('Y-m-d',time()).'"/>';
		echo '<input type="hidden" id="total_vat" value="'.round($tot1).'"/>';
		echo '<input type="hidden" id="total_vat_ex" value="'.round($tta).'"/>';


		echo '<input type="hidden" id="date" value="'.time().'"/>';
		echo '<input type="hidden" id="ref" value="'.$rea_id.'"/>';
		$cc = $this->getAssignedInvoiceNumber($customer_id, portion(4), $month);
		echo '<input type="hidden" id="id" value="'.$cc[0].'"/>';
		echo '<input type="hidden" id="id_REF_INVOICE" value="'.$cc[0].'"/>';
		echo '<input type="hidden" name="" id="inv_date" value="2025-06-30"/>';
		echo '<input type="hidden" name="" id="customer" value="'.$customer_id.'"/>'; 
		 echo '<input type="hidden" id="exchange" value=""/>';
		// @@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
		 echo '<input type="hidden" name="" id="peak" value="'.$peak_value.'"/>'; 
		 echo '<input type="hidden" name="" id="shoulder" value="'.$shoulder_value.'"/>'; 
		 echo '<input type="hidden" name="" id="off_peak" value="'.$off_peak_value.'"/>'; 
		 echo '<input type="hidden" name="" id="payable" value="'.$payable_value.'"/>'; 
		 echo '<input type="hidden" name="" id="collectable" value="'.$collectable_value.'"/>'; 
		 echo '<input type="hidden" name="" id="vat_amount" value="'.$vat_value.'"/>';
		 $post_date = $cc[2]; 
		$invoice_date = $cc[1]; 
		 echo '<input type="hidden" name="" id="post_date" value="'.$post_date.'"/>';
		 echo '<input type="hidden" name="" id="invoice_date" value="'.$invoice_date.'"/>';

		 //@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
		if(!$fdn){
			if($c['tc'] == 3){
			echo '<br><br><button data-type="approve" data-count="'.$tc.'" type="button" name="approve" id="pushInvoiceToEFRIS" class="btn btn-xs btn-success"><i class="fa fa-fw fa-plane"></i>Push To EFRIS</button>';
			echo '<span id="pushInvoiceToEFRISStatus"></span>';
	     }
	    }
		?>
	</form>
<script type="text/javascript">	
	$(document).ready(function(){  
		// id = $(this).val();
		//$('#exchange').val('checking...');
		var form_data = new FormData();
		form_data.append('id', $('#inv_date').val());
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
		//$('#pushInvoiceToEFRIS').click(function(){	
		$(document).off('click','#pushInvoiceToEFRIS').on('click', '#pushInvoiceToEFRIS', function(e){
			var items = $('#items').val();
			var post_date = $('#post_date').val();
			var invoice_date = $('#invoice_date').val();

			var a_currency = $('#a_currency').val();
			var a_amount = $('#unitPrice').val();
			var inv_date = $('#inv_date').val();
			var exchange = $('#exchange').val();
			var buyer = $('#buyer').val();
			var id = $('#id_REF_INVOICE').val();
			var currency = $('#currency').val();
			var vat = $('#vat').val();
			var date = $('#date').val();
			var item = $('#item').val();
			var peak = $('#peak').val();

			var shoulder = $('#shoulder').val();
			var peak = $('#peak').val();
			var off_peak = $('#off_peak').val();
			var payable = $('#payable').val();
			var collectable = $('#collectable').val();
			var vat_amount = $('#vat_amount').val();


			var type = $(this).attr('data-type');
        	var count = $(this).attr('data-count');
        	var parc_id = $('#parc_id').val();
        	var comment = $('#comment').val();

        	var month = $('#month').val();
        	var invoice_remarks = $('#invoice_remarks').val();				        	
        	var year = $('#year').val();				        	
        	var part = $('#part').val();			        	
        	var customer = $('#customer').val();
        	var total_vat = $('#total_vat').val();			        	
        	var total_vat_ex = $('#total_vat_ex').val();
        	

        	//alert(comment);

			var unitPrice = $('#unitPrice').val();

			if(!confirm('Please confirm the details below\nAmount: '+a_amount+'\nCurrency: '+a_currency+'\nInvoice Date: '+inv_date+'')) return false;

			$(this).attr("disabled", true);
			$('#pushInvoiceToEFRISStatus').html('<img src="'+urlPath+'images/loading.gif" alt="loading..."/>');	

			if($('#ref').val()){
				var form_data = new FormData();				
				form_data.append('items', $('#items').val());
				form_data.append('post_date', $('#post_date').val());
				form_data.append('invoice_date', $('#invoice_date').val());

				form_data.append('shoulder', $('#shoulder').val());
				form_data.append('off_peak', $('#off_peak').val());
				form_data.append('payable', $('#payable').val());
				form_data.append('peak', $('#peak').val());
				form_data.append('collectable', $('#collectable').val());
				//form_data.append('collectable', $('#collectable').val());
				form_data.append('vat_amount', $('#vat_amount').val());

			
				
				form_data.append('qty', $('#qty').val());
				form_data.append('buyer', $('#buyer').val());
				form_data.append('ref', $('#ref').val());
				form_data.append('id', $('#id_REF_INVOICE').val());
				form_data.append('currency', $('#a_currency').val());
				form_data.append('vat', $('#vat').val());
				form_data.append('date', $('#date').val());
				form_data.append('unitPrice', $('#unitPrice').val());
				form_data.append('exchange', $('#exchange').val());
				form_data.append('comment', comment);
		        form_data.append('part', part);
		        form_data.append('month', month);
		        form_data.append('year', year);
		        form_data.append('parc_id', parc_id);
		        form_data.append('customer', customer);
		        form_data.append('count', count);
		        form_data.append('type', type);
		        form_data.append('a_amount', a_amount);
		        form_data.append('invoice_remarks', invoice_remarks);
		        form_data.append('inv_date', inv_date);
		        form_data.append('total_vat', total_vat);
		        form_data.append('total_vat_ex', total_vat_ex);

		        //alert($('#id_REF_INVOICE').val());

				//return 0;
				
				$.ajax({
					url: urlPath+"ajax/pushEnergyCustomerInvoice.php",
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
			
		</div>
		
		<?php 
		$invoice_no = $cc[0];
		$post_date = $cc[2]; 
		$invoice_date = $cc[1]; 
		$entity = "ENERGY";
		
		//$this->GL_ACCOUNT($invoice_no,$post_date,$invoice_date,$id,$entity);
		if($this->anyCustomerReading($year, $month)){	
			$this->energyApprovals(user_id(), $year, $month,0, $tab, $customer_id);
		}

		?>
		<div class="clearfix"></div>
		<?php 
		echo '<div style="width:900px;margin:auto;">';
		$this->addReadingComment();
		echo '</div>';
		?>

	</div>
	<?php
	}

	
	public function viewInvoiceCreditNote($id, $month, $year){

		$class = ($this->set_class)?$this->set_class:portion(1);
		$method = ($this->set_method)?$this->set_method:portion(2);
		$id = ($this->set_customer)?$this->set_customer:portion(3);
		$year = ($this->set_year)?$this->set_year: portion(4);
		$month = ($this->set_month)?$this->set_month: portion(5);

		$db = new Db();
		$select = $db->select("SELECT * FROM customer WHERE customer_id = '$id'");
		extract($select[0][0]);
	
	?>
	<div id="side">
		
		<?php 

		$units = $this->customerAndSchedule($id, $year, $month);
		$this->energyLinks($id, $year, $month);

		?>
		<style>
		table tr th{ background-color:#ccc; color:black; }
		.hide{display:none;}
		.table tr:hover{
			background-color:none;
			background:none !important;
		}
		#y>tbody>tr>td{ padding:5px; background-color:#fbfbfb; border:1px solid #000; }
		.hide{ display: none; }
		</style>
		<div id="printMe" style="width:900px;margin:auto;">
			<?php echo '<title>'.strtoupper($customer_short_name.' INVOICE').' - '.$set_month.' '.$set_year.'</title>'; ?>
			<style type="text/css">
				table tr td{
					padding:5px 20px;
				}
			</style>
			<table border="0" cellspacing="0" cellpadding="2" id="y" style="width:100%; font-family: arial; margin:auto;">
				<tbody>
				<tr valign="bottom">
					<td colspan="11">
					<img class="xhide" src="<?php display_url();?>images/uetcl_header.png" width="100%">
					<center class="xhide"><h2 style="color:blue;margin: 5px 0 0 0; padding:0">TAX INVOICE</h2></center>
						<!-- <hr style="border:1px solid black;"/> -->
						<?php
						$db = new Db();
						$select = $db->select("SELECT * FROM metering_point, r_reading WHERE mp_customer_id = '$customer_id' AND rea_cus_id = '$customer_id' AND mp_id = rea_mp_id");
						foreach($select[0] as $row){
							extract($row);
							if(date('Y', $rea_date)==$this->set_year && date('m', $rea_date)==$this->set_month){
								$invoice_no = str_pad($rea_id, 5, '0', STR_PAD_LEFT);
							}
						}

						?>
						<p align="right" style="font-size:10pt; padding:0;margin:0; font-weight: bold; color:red;"><span style="color:black;">Tax Invoice No.:</span> &nbsp; <?php 
						$c = $this->getAssignedInvoiceNumber($customer_id, portion(4), $month);
						echo $c[0]; ?></p>
						<table border="0" class="" cellspacing="10" cellpadding="0" style="width:100% !important;font-family: arial; font-size: 12pt;">
							
							<tr valign="top">
								<td>Date:</td>
								<td><?php 
								if($c[1] != "")
									echo Feedback::date_s($c[1]); 
								else{
									echo ' - ';
								}
								?></td>
								<!-- <td rowspan="4" style="width:40%;">
									
								</td> -->
							</tr>
							<tr valign="top">
								<td width="">Name&nbsp;of&nbsp;Customer:</td>
								<td>
								<?php
									echo '<b>'.$customer_full_name.'</b>';
								?>								
								</td>
							</tr>
							<tr valign="top" class="hide">
								<td width="">Customer Address:</td>
								<td>
								<?php
									if(empty($customer_street)){
										echo nl2br($customer_address);
									}else{
										echo nl2br($customer_street).'<br/>';
										echo nl2br($customer_postal_code).'<br/>';
										echo nl2br($customer_tel).'<br/>';
										echo nl2br($customer_email).'<br/>';
									}
								?>								
								</td>
							</tr>
							<tr>
								<?php $year; $month; ?>
								<td>Detail:</td>
								<td><u><b><?php
								$toux = $this->tou($customer_id, $month, $year, 1);

								if(empty($toux[5]))
								 	echo "NO SET NARRATION";//$this->noSpace('Energy Bill for '.month_name($month).' '.$year); 
								 else
								 	echo $toux[5];

								 ?></b></u></td>
							</tr>
						</table>
						
						<?php
						//================= currency ======================================
						if($customer_currency){
							$currency = $this->rgf("currency",$customer_currency, "currency_id", "currency_name");
							$symbol = $this->rgf("currency",$customer_currency, "currency_id", "currency_symbol");
						}else{
							$currency = $this->rgf("currency",1, "currency_id", "currency_name");
							$symbol = $this->rgf("currency",1, "currency_id", "currency_symbol");
						}
	
						$tou = $this->tou($customer_id, $month, $year);
		
						$peak = $tou[0];
						$shoulder = $tou[1];
						$off_peak = $tou[2];




						if($customer_id ==2028){//Rwanda
							$db = new Db();
							//current main 1
							$customer_id = ($this->set_customer)?$this->set_customer:portion(3);
							$reading_time = strtotime(date("$year-$month-15"));

							$sql = "SELECT * FROM r_reading, r_rate, metering_point WHERE rea_date = '$reading_time' AND rea_cus_id = '$customer_id' AND rate_cus_id = '$customer_id' AND rate_reading_id = rea_id AND rea_mp_id = mp_id AND upper(mp_location) = 'KATUNA-MAIN'";
							$select = $db->select($sql);
							extract($select[0][0]);
							$present_main = $rate_cum_imports;

							if(empty($reading_date)){
								$reading_date = $rate_date_read;
							}

							//previous main reading
							$sql = "SELECT TOP 1 * FROM r_reading, r_rate, metering_point WHERE rea_date < '$rea_date' AND rea_id = rate_reading_id AND rea_cus_id = '$customer_id' AND rea_mp_id = mp_id AND rate_cus_id = '$customer_id' AND upper(mp_location) = 'KATUNA-MAIN' ORDER BY rea_date DESC";
							$sel = $db->select($sql);
							extract($sel[0][0]);
							$previous_main = $rate_cum_imports;


							///////////////////// check

							$sql = "SELECT * FROM r_reading, r_rate, metering_point WHERE rea_date = '$reading_time' AND rea_cus_id = '$customer_id' AND rate_cus_id = '$customer_id' AND rate_reading_id = rea_id AND rea_mp_id = mp_id AND upper(mp_location) = 'KATUNA-CHECK'";
							$select = $db->select($sql);
							extract($select[0][0]);
							//print_r($select[0][0]);
							$present_check = $rate_cum_imports;

							if(empty($reading_date)){
								$reading_date = $rate_date_read;
							}

							//previous main reading
							$sql = "SELECT TOP 1 * FROM r_reading, r_rate, metering_point WHERE rea_date < '$rea_date' AND rea_id = rate_reading_id AND rea_cus_id = '$customer_id' AND rea_mp_id = mp_id AND rate_cus_id = '$customer_id' AND upper(mp_location) = 'KATUNA-CHECK' ORDER BY rea_date DESC";
							$sel = $db->select($sql);
							extract($sel[0][0]);
							$previous_check = $rate_cum_imports;

							$advance_main = $present_main - $previous_main;
							$advance_check = $present_check - $previous_check;

							$average = ($advance_main+ $advance_check)/2;
							?>
							<?php
							
							$tm = 0;
							


							if($peak != (int)$peak){
								$peak1 = number_format($peak, 5);
							}else{
								$peak1 = $peak;
							}
							if($shoulder != (int)$shoulder){
								$shoulder1 = number_format($shoulder, 5);
							}else{
								$shoulder1 = $shoulder;
							}
							if($off_peak != (int)$off_peak){
								$off_peak1 = number_format($off_peak, 5);
							}else{
								$off_peak1 = $off_peak;
							}
							
							if($reading_date == ""){
								$reading_date = "";
							}else{
								$reading_date = Feedback::date_c(strtotime($reading_date));
							}

							$q1 = ($present_main+$present_check)/2/1000;
							$q2 = ($previous_main+$previous_check)/2/1000;
							

							?>
							<style>
								#x tr td, #x tr th{
									padding:2px 10px;
								}
								#x tr th{
									text-align: center;
								}
							</style>
							<table cellpadding="5" style="font-family: arial; font-size: 20px;" cellspacing="0" border="1" width="100%" id="x">
							<?php
							

							echo '<tr style="height:10px;">';
							echo '<th align="center">Advance Category</th>';
							echo '<th align="center">Kwh</th>';
							echo '<th align="center">Rate<br/>(US $)</th>';
							echo '<th align="center">Amount<br/>(US $)</th>';
							echo '</tr>';

							echo '<tr style="height:90px;">';
							echo '<td>Net Energy Advance</td>';
							echo '<td align="right">'.number_format($q1-$q2,2).'</td>';
							echo '<td align="right">'.($shoulder).'</td>';
							echo '<td align="right">'.number_format($shoulder*($q1-$q2), 2).'</td>';
							echo '</tr>';
							
							/*
							echo '<tr style="height:90px;">';
							echo '<td>Total</td>';
							echo '<td align="right">'.number_format($q1-$q2,2).'</td>';
							echo '<td align="right"></td>';
							echo '<td align="right">'.number_format($shoulder*($q1-$q2), 2).'</td>';
							echo '</tr>';
							
							*/

							?>

							<tr>
								<td colspan="4" style="font-size:12px;"><br/>
								<?php $tot = $shoulder*($q1-$q2)*($customer_vat/100+1); if($tot<200000000000){?>
								Total in words:<br/>
								<?php 
								echo '<b style="">';
								if($symbol != '$'){
									echo number_to_words($shoulder*($q1-$q2));
								}else{
									$t = $shoulder*($q1-$q2);
									$tt = (int)$t;
									if($tt != $t){
										$d = round($t - $tt,2);

										echo number_to_words($t);
										if(!empty($d)){
											echo ' Point ';
											$g = $d*10;
											$g2 = (int)$g;
											echo number_to_words($g2);
											echo ' ';
											echo number_to_words((int)(($g-$g2)*10));
										}
									}else{
										echo number_to_words($t);
									}
								}
								echo " $currency".'</b>'; 
								?><br/>
								</td>
								<?php } ?>
							</tr>
							<?php if($oi_exchange_rate && $currency =='Dollars'){?>
							<tr>
							<td colspan="4">
								Exhange Rate:
								<?php 
								//echo ;
									echo number_format($oi_exchange_rate,2);
								echo " $currency".''; 
								?>
								</td>
							</tr>
						<?php }?>

							<?php

							echo '</table>';
						}elseif($customer_id =='2028_OLD'){
							$db = new Db();
							//current main 1
							$customer_id = portion(3);
							$reading_time = strtotime(date("$year-$month-15"));

							$sql = "SELECT * FROM r_reading, r_rate, metering_point WHERE rea_date = '$reading_time' AND rea_cus_id = '$customer_id' AND rate_cus_id = '$customer_id' AND rate_reading_id = rea_id AND rea_mp_id = mp_id AND upper(mp_location) = 'KATUNA-MAIN'";
							$select = $db->select($sql);
							extract($select[0][0]);
							$present_main = $rate_cum_imports;

							if(empty($reading_date)){
								$reading_date = $rate_date_read;
							}

							//previous main reading
							$sql = "SELECT TOP 1 * FROM r_reading, r_rate, metering_point WHERE rea_date < '$rea_date' AND rea_id = rate_reading_id AND rea_cus_id = '$customer_id' AND rea_mp_id = mp_id AND rate_cus_id = '$customer_id' AND upper(mp_location) = 'KATUNA-MAIN' ORDER BY rea_date DESC";
							$sel = $db->select($sql);
							extract($sel[0][0]);
							$previous_main = $rate_cum_imports;


							///////////////////// check

							$sql = "SELECT * FROM r_reading, r_rate, metering_point WHERE rea_date = '$reading_time' AND rea_cus_id = '$customer_id' AND rate_cus_id = '$customer_id' AND rate_reading_id = rea_id AND rea_mp_id = mp_id AND upper(mp_location) = 'KATUNA-CHECK'";
							$select = $db->select($sql);
							extract($select[0][0]);
							//print_r($select[0][0]);
							$present_check = $rate_cum_imports;

							if(empty($reading_date)){
								$reading_date = $rate_date_read;
							}

							//previous main reading
							$sql = "SELECT TOP 1 * FROM r_reading, r_rate, metering_point WHERE rea_date < '$rea_date' AND rea_id = rate_reading_id AND rea_cus_id = '$customer_id' AND rea_mp_id = mp_id AND rate_cus_id = '$customer_id' AND upper(mp_location) = 'KATUNA-CHECK' ORDER BY rea_date DESC";
							$sel = $db->select($sql);
							extract($sel[0][0]);
							$previous_check = $rate_cum_imports;

							$advance_main = $present_main - $previous_main;
							$advance_check = $present_check - $previous_check;

							$average = ($advance_main+ $advance_check)/2;
							?>
							<style>
								#x tr td, #x tr th{
									padding:2px 10px;
								}
								#x tr th{
									text-align: center;
								}
							</style>
							<table cellpadding="5" style="font-family: arial; font-size: 16px;" cellspacing="0" border="1" width="100%" id="x">
							<?php
							
							$tm = 0;
							


							if($peak != (int)$peak){
								$peak1 = number_format($peak, 5);
							}else{
								$peak1 = $peak;
							}
							if($shoulder != (int)$shoulder){
								$shoulder1 = number_format($shoulder, 5);
							}else{
								$shoulder1 = $shoulder;
							}
							if($off_peak != (int)$off_peak){
								$off_peak1 = number_format($off_peak, 5);
							}else{
								$off_peak1 = $off_peak;
							}

							echo '<tr style="height:10px;">';
							echo '<th align="center">Description</th>';
							echo '<th align="center">QTY - Kwh</th>';
							echo '<th align="center">Rate<br/>(US $)</th>';
							echo '<th align="center">Amount<br/>(US $)</th>';
							echo '</tr>';

							//$select 
							
							if($reading_date == ""){
								$reading_date = "";
							}else{
								$reading_date = Feedback::date_c(strtotime($reading_date));
							}

							echo '<tr style="">';
							echo '<td>Reading Date: '.$reading_date.'</td>';
							echo '<td align="right"></td>';
							echo '<td align="right"></td>';
							echo '<td align="right"></td>';
							echo '</tr>';

							echo '<tr style="">';
							if($sc[1])
								echo '<td>Billing Date: '.Feedback::date_s($c[1]).'</td>';
							else
								echo '<td>Billing Date: - </td>';

							echo '<td align="right"></td>';
							echo '<td align="right"></td>';
							echo '<td align="right"></td>';
							echo '</tr>';

							echo '<tr style="">';
							echo '<td>Current Reading: (Average)</td>';
							$q1 = ($present_main+$present_check)/2/1000;
							echo '<td align="right">'.number_format($q1, 2).'</td>';
							echo '<td align="right"></td>';
							echo '<td align="right"></td>';
							echo '</tr>';

							echo '<tr style="">';
							echo '<td>Previous reading:</td>';
							$q2 = ($previous_main+$previous_check)/2/1000;
							echo '<td align="right">'.number_format($q2, 2).'</td>';
							echo '<td align="right"></td>';
							echo '<td align="right"></td>';
							echo '</tr>';

							echo '<tr style="">';
							echo '<td>Total Units Consumed (Kwh):</td>';
							echo '<td align="right">'.number_format($q1-$q2, 2).'</td>';
							echo '<td align="right">'.($shoulder1).'</td>';
							echo '<td align="right">'.number_format($shoulder*($q1-$q2), 2).'</td>';
							echo '</tr>';

							echo '<tr style="">';
							echo '<td colspan="3">Sub Total (VAT Exclusive)</td>';
							echo '<td align="right">'.number_format($shoulder*($q1-$q2), 2).'</td>';
							echo '</tr>';

							echo '<tr style="">';
							echo '<td colspan="3">VAT</td>';
							echo '<td align="right">'.number_format(0).'</td>';
							echo '</tr>';

							echo '<tr style="">';
							echo '<td colspan="3">Total Amount (VAT Inclusive)</td>';
							echo '<td align="right">'.number_format($shoulder*($q1-$q2), 2).'</td>';
							echo '</tr>';

							?>

							<tr>
								<td colspan="4" style="font-size:12px;">
								<?php $tot = $tta*($customer_vat/100+1); if($tot<200000000000){?>
								Total in words:<br/>
								<?php 
								echo '<b style="">';
								if($symbol != '$'){
									echo number_to_words($shoulder*($q1-$q2));
								}else{
									$t = $shoulder*($q1-$q2);
									$tt = (int)$t;
									if($tt != $t){
										$d = round($t - $tt,2);

										echo number_to_words($t);
										if(!empty($d)){
											echo ' Point ';
											$g = $d*10;
											$g2 = (int)$g;
											echo number_to_words($g2);
											echo ' ';
											echo number_to_words((int)(($g-$g2)*10));
										}
									}else{
										echo number_to_words($t);
									}
								}
								echo " $currency".'</b>'; 
								?>
								</td>
								<?php } ?>
							</tr>
							<?php if($oi_exchange_rate && $currency =='Dollars'){?>
							<tr>
							<td colspan="4">
								Exhange Rate:
								<?php 
								//echo ;
									echo number_format($oi_exchange_rate,2);
								echo " $currency".''; 
								?>
								</td>
							</tr>
						<?php }?>

							<?php

							echo '</table>';
						}elseif($customer_id ==2029){
							$db = new Db();
							//current main 1
							$customer_id = portion(3);
							$reading_time = strtotime(date("$year-$month-15"));

							$sql = "SELECT * FROM r_reading, r_rate, metering_point WHERE rea_date = '$reading_time' AND rea_cus_id = '$customer_id' AND rate_cus_id = '$customer_id' AND rate_reading_id = rea_id AND rea_mp_id = mp_id AND upper(mp_location) = 'TANESCO'";
							$select = $db->select($sql);
							extract($select[0][0]);
							$current_main = $rate_cum_imports;

							if(empty($reading_date)){
								$reading_date = $rate_date_read;
							}

							//previous main reading
							$sql = "SELECT TOP 1 * FROM r_reading, r_rate, metering_point WHERE rea_date < '$rea_date' AND rea_id = rate_reading_id AND rea_cus_id = '$customer_id' AND rea_mp_id = mp_id AND rate_cus_id = '$customer_id' AND upper(mp_location) = 'TANESCO' ORDER BY rea_date DESC";
							$sel = $db->select($sql);
							extract($sel[0][0]);
							$previous_main = $rate_cum_imports;


							$average = ($advance_main+ $advance_check)/2;
							?>
							<style>
								#x tr td, #x tr th{
									padding:2px 10px;
								}
								#x tr th{
									text-align: center;
								}
							</style>
							<table cellpadding="5" style="font-family: arial; font-size: 16px;" cellspacing="0" border="1" width="100%" id="x">
							<?php
							
							$tm = 0;
							


							if($peak != (int)$peak){
								$peak1 = number_format($peak, 5);
							}else{
								$peak1 = $peak;
							}
							if($shoulder != (int)$shoulder){
								$shoulder1 = number_format($shoulder, 5);
							}else{
								$shoulder1 = $shoulder;
							}
							if($off_peak != (int)$off_peak){
								$off_peak1 = number_format($off_peak, 5);
							}else{
								$off_peak1 = $off_peak;
							}

							echo '<tr style="height:10px;">';
							echo '<th align="center">Description</th>';
							echo '<th align="center">QTY - Kwh</th>';
							echo '<th align="center">Rate<br/>(US $)</th>';
							echo '<th align="center">Amount<br/>(US $)</th>';
							echo '</tr>';

							//$select 
							
							if($reading_date == ""){
								$reading_date = "";
							}else{
								$reading_date = Feedback::date_c(strtotime($reading_date));
							}

							echo '<tr style="">';
							echo '<td>Reading Date: '.$reading_date.'</td>';
							echo '<td align="right"></td>';
							echo '<td align="right"></td>';
							echo '<td align="right"></td>';
							echo '</tr>';

							echo '<tr style="">';
							if($sc[1])
								echo '<td>Billing Date: '.Feedback::date_s($c[1]).'</td>';
							else
								echo '<td>Billing Date: - </td>';

							echo '<td align="right"></td>';
							echo '<td align="right"></td>';
							echo '<td align="right"></td>';
							echo '</tr>';

							echo '<tr style="">';
							echo '<td>Current Reading: </td>';
							$q1 = ($current_main/1000);
							echo '<td align="right">'.number_format($q1, 2).'</td>';
							echo '<td align="right"></td>';
							echo '<td align="right"></td>';
							echo '</tr>';

							echo '<tr style="">';
							echo '<td>Previous reading:</td>';
							$q2 = ($previous_main/1000);
							echo '<td align="right">'.number_format($q2, 2).'</td>';
							echo '<td align="right"></td>';
							echo '<td align="right"></td>';
							echo '</tr>';

							$a1 = array(1,2,3);
							$a2 = array(4,5,6);
							$a3 = array(7,8,9);
							$a4 = array(10,11,12);

							$use = "";

							//$year;

							if(in_array($month, $a1)){
								$use = "a4";
								$year -= 1;
								$factor_month = $$use[0];
							}elseif(in_array($month, $a2)){
								$use = "a1";
								$factor_month = $$use[0];
							}elseif(in_array($month, $a3)){
								$use = "a2";
								$factor_month = $$use[0];
							}elseif(in_array($month, $a4)){
								$use = "a3";
								$factor_month = $$use[0];
							}
							$e = 0;
							foreach($$use as $m){
								$e += $this->sum($year, $m, 0);
							}

							$shoulder = $e/3/100;
							$shoulder1 = number_format($shoulder,6);

							echo '<tr style="">';
							echo '<td>Total Units Consumed (Kwh):</td>';
							echo '<td align="right">'.number_format($q1-$q2, 2).'</td>';
							echo '<td align="right">'.($shoulder1).'</td>';
							echo '<td align="right">'.number_format($shoulder*($q1-$q2), 2).'</td>';
							echo '</tr>';

							echo '<tr style="">';
							echo '<td colspan="3">Sub Total (VAT Exclusive)</td>';
							echo '<td align="right">'.number_format($shoulder*($q1-$q2), 2).'</td>';
							echo '</tr>';

							echo '<tr style="">';
							echo '<td colspan="3">VAT</td>';
							echo '<td align="right">'.number_format(0).'</td>';
							echo '</tr>';

							echo '<tr style="">';
							echo '<td colspan="3">Total Amount (VAT Inclusive)</td>';
							echo '<td align="right">'.number_format($shoulder*($q1-$q2), 2).'</td>';
							echo '</tr>';

							?>

							<tr>
								<td colspan="4" style="font-size:12px;">
								<?php $tot = $tta*($customer_vat/100+1); if($tot<200000000000){?>
								Total in words:<br/>
								<?php 
								echo '<b style="">';
								if($symbol != '$'){
									echo number_to_words($shoulder*($q1-$q2));
								}else{
									$t = $shoulder*($q1-$q2);
									$tt = (int)$t;
									if($tt != $t){
										$d = round($t - $tt,2);

										echo number_to_words($t);
										if(!empty($d)){
											echo ' Point ';
											$g = $d*10;
											$g2 = (int)$g;
											echo number_to_words($g2);
											echo ' ';
											echo number_to_words((int)(($g-$g2)*10));
										}
									}else{
										echo number_to_words($t);
									}
								}
								echo " $currency".'</b>'; 
								?>
								</td>
								<?php } ?>
							</tr>
							<?php if($oi_exchange_rate && $currency =='Dollars'){?>
							<tr>
							<td colspan="4">
								Exhange Rate:
								<?php 
								//echo ;
									echo number_format($oi_exchange_rate,2);
								echo " $currency".''; 
								?>
								</td>
							</tr>
						<?php }?>

							<?php

							echo '</table>';
						}elseif($customer_id == 2030){
							?>
							<style>
								#x tr td, #x tr th{
									padding:2px 10px;
								}
								#x tr th{
									text-align: center;
								}
							</style>
							<table cellpadding="5" style="font-family: arial; font-size: 20px;" cellspacing="0" border="1" width="100%" id="x">
							<?php
							$a = $this->KPLC($year, $month);
							$tm = 0;
							for($i=0; $i<3; $i++)
								$tm += $a['main'][$i];

							$tc = 0;
							for($i=0; $i<3; $i++)
								$tc += $a['check'][$i];

							$ta = ($tm + $tc)/(2*1000);

							if($peak != (int)$peak){
								$peak1 = number_format($peak, 5);
							}else{
								$peak1 = $peak;
							}
							if($shoulder != (int)$shoulder){
								$shoulder1 = number_format($shoulder, 5);
							}else{
								$shoulder1 = $shoulder;
							}
							if($off_peak != (int)$off_peak){
								$off_peak1 = number_format($off_peak, 5);
							}else{
								$off_peak1 = $off_peak;
							}

							echo '<tr style="height:10px;">';
							echo '<th align="center">Advance Category</th>';
							echo '<th align="center">Kwh</th>';
							echo '<th align="center">Rate<br/>(US $)</th>';
							echo '<th align="center">Amount<br/>(US $)</th>';
							echo '</tr>';

							echo '<tr style="height:90px;">';
							echo '<td>Net Energy Advance</td>';
							echo '<td align="right">'.number_format($ta).'</td>';
							echo '<td align="right">'.($shoulder1).'</td>';
							echo '<td align="right">'.number_format($shoulder*$ta, 2).'</td>';
							echo '</tr>';

							/*
							echo '<tr style="height:90px;">';
							echo '<td>Total</td>';
							echo '<td align="right">'.number_format($ta).'</td>';
							echo '<td align="right"></td>';
							echo '<td align="right">'.number_format($shoulder*$ta, 2).'</td>';
							echo '</tr>';
							*/

							?>

							<tr>
								<td colspan="4" style="font-size:12px;"><br/>
								<?php $tot = $tta*($customer_vat/100+1); if($tot<200000000000){?>
								Total in words:<br/>
								<?php 
								echo '<b style="">';
								if($symbol != '$'){
									echo number_to_words($shoulder*$ta);
								}else{
									$t = $shoulder*$ta;
									$tt = (int)$t;
									if($tt != $t){
										$d = round($t - $tt,2);

										echo number_to_words($t);
										if(!empty($d)){
											echo ' Point ';
											$g = $d*10;
											$g2 = (int)$g;
											echo number_to_words($g2);
											echo ' ';
											echo number_to_words((int)(($g-$g2)*10));
										}
									}else{
										echo number_to_words($t);
									}
								}
								echo " $currency".'</b>'; 
								?><br/>
								</td>
								<?php } ?>
							</tr>
							<?php if($oi_exchange_rate && $currency =='Dollars'){?>
							<tr>
							<td colspan="4">
								Exhange Rate:
								<?php 
								//echo ;
									echo number_format($oi_exchange_rate,2);
								echo " $currency".''; 
								?>
								</td>
							</tr>
						<?php }?>

							<?php

							echo '</table>';
						}elseif($id == 20111111111117){	
						//echo 'her';

						//=============== WHEELING CHARGE BANDS =====================================
						$db = new Db();
						$select = $db->select("SELECT * FROM wheeling_charge WHERE wc_customer_id = '$customer_id' ORDER BY wc_id ASC");
						$assigned2 = 0;
						foreach($select[0] as $row){
							extract($row);
							if(date('Y', $wc_date_added)==$year && date('m', $wc_date_added)==$month)
							{
								$cwc = $wc_rate;
								$assigned2 = 1;
							}
						}

						if($assigned2 == 0){
							$db = new Db();
							$select = $db->select("SELECT * FROM wheeling_charge WHERE wc_customer_id = '$customer_id' ORDER BY wc_id ASC");
							$assigned = 0;
							foreach($select[0] as $row){
								extract($row);
								//if(date('Y', $wc_date_added)==$year && date('m', $wc_date_added)==$month)
								{
									$cwc = $wc_rate;
									$assigned2 = 1;
								}
							}
						}
						
						$t1 = $t2 = $t3 = $t4 = 0;
							$units = 0.001;
						$m = $this->sumFormulae($customer_id, $year, $month);
						//echo '->'.$m[0].'->'.$units;
						$t1 = ($m[0]*$units);
						$t2 = ($m[1]*$units);
						$t3 = ($m[2]*$units);
						$t4 = ($m[3]*$units);

						$h = $this->wheelingChargeSum($customer_id, $year, $month);
						$tbf = $this->totalBackflow($id, $year, $month);
						$ttu = $tta = 0;
						?>
						<style>
							#x tr td, #x tr th{
								padding:2px 10px;
							}
							#x tr th{
								text-align: center;
							}
						</style>
						<table cellpadding="5" style="font-family: arial; font-size: 10pt;" cellspacing="0" border="1" width="100%" id="x">
							<tr>
								<th>TIME BAND</th>
								<th>UNITS - 
									<?php
									echo '(';
									if($units == 1){
										echo 'Wh';
									}elseif($units == 0.001){
										echo 'KWh';
									}elseif($units == 0.000001){
										echo 'MWh';
									}
									echo ')';
									?>
								</th>
								<th>RATE - <?php echo "($symbol / kWh)"; ?></th>
								<th>AMOUNT<?php echo " ($symbol)"; ?></th>
							</tr>
							<tr>

								<?php 
								
								$a1 = array(1,2,3);
								$a2 = array(4,5,6);
								$a3 = array(7,8,9);
								$a4 = array(10,11,12);

								$use = "";

								//$year;

								if(in_array($month, $a1)){
									$use = "a4";
									$year -= 1;
									$factor_month = $$use[0];
								}elseif(in_array($month, $a2)){
									$use = "a1";
									$factor_month = $$use[0];
								}elseif(in_array($month, $a3)){
									$use = "a2";
									$factor_month = $$use[0];
								}elseif(in_array($month, $a4)){
									$use = "a3";
									$factor_month = $$use[0];
								}
								$e = 0;
								foreach($$use as $m){
									$e += $this->sum($year, $m, 0);
								}

								$shoulder = $peak = $off_peak = $e/3/100*1.05;
								
								if($peak != (int)$peak){
									$peak1 = number_format($peak, 6);
								}else{
									$peak1 = $peak;
								}
								if($shoulder != (int)$shoulder){
									$shoulder1 = number_format($shoulder, 6);
								}else{
									$shoulder1 = $shoulder;
								}
								if($off_peak != (int)$off_peak){
									$off_peak1 = number_format($off_peak, 6);
								}else{
									$off_peak1 = $off_peak;
								}

								$o = $peak;
								$ttu += ($t2-$tbf[1]);
								$tta += $o*($t2-$tbf[1]);
								?>
								<?php 
									if($symbol == '$'){
										$a = number_format($o*($t2-$tbf[1]),2);
									}else{
										$a = number_format($o*($t2-$tbf[1]));
									}
								?>
								<td>Peak</td>
								<td align="right"><?php echo number_format($t2-$tbf[1],2); ?></td>
								<td align="right"><?php echo ((float)$peak1). ""; ?></td>
								<td align="right"><?php echo $a. ""; ?></td>
							</tr>
							<tr>
								<?php 
								$o = $shoulder;
								$ttu += ($t1-$tbf[0]);
								$tta += $o*($t1-$tbf[0]);
								?>
								<?php 
									if($symbol == '$'){
										$a = number_format($o*($t1-$tbf[0]),2);
									}else{
										$a = number_format($o*($t1-$tbf[0]));
									}
								?>
								<td>Shoulder</td>
								<td align="right"><?php echo number_format(($t1-$tbf[0]),2); ?></td>
								<td align="right"><?php echo ((float)$shoulder1). ""; ?></td>
								<td align="right"><?php echo $a. ""; ?></td>
							</tr>
							<tr>
								<td>Off Peak</td>
								<?php 
								$o = $off_peak;
								$ttu += ($t3-$tbf[2]);
								$tta += $o*($t3-$tbf[2]);
								?>
								<?php 
									if($symbol == '$'){
										$a = number_format($o*($t3-$tbf[2]),2);
									}else{
										$a = number_format($o*($t3-$tbf[2]));
									}
								?>
								<td align="right"><?php echo number_format(($t3-$tbf[2]),2); ?></td>
								<td align="right"><?php echo ((float)$off_peak1). " "; ?></td>
								<td align="right"><?php echo $a. " "; ?></td>
							</tr>
							<tr>
								<td>Total</td>
								<td align="right"><?php echo number_format($ttu,2); ?></td>
								<?php 
									if($symbol == '$'){
										$a = number_format($tta,2);
									}else{
										$a = number_format($tta);
									}
								?>
								<td></td>
								<td align="right"><?php echo $a. " "; ?></td>
							</tr>
							<?php

							$payables = array();
							$payablez = array();
							$db = new Db();

							$select = $db->select("SELECT distinct bn_customer FROM boundary_node WHERE bn_customer_payable_to = '$customer_id'");

							foreach($select[0] as $row){
								extract($row);

								$sel = $db->select("SELECT bn_metering_point FROM boundary_node WHERE bn_customer_payable_to = '$customer_id' AND bn_customer = '$bn_customer' ");
								foreach($sel[0] as $ro){
									extract($ro);
									$zz = rate_calculate($bn_customer, strtotime(date("$year-$month-15")), $bn_metering_point);
						
									if($zz[24]!="([DONOT_DISPLAY])")
									{
										$payables[$bn_customer][] = $bn_metering_point;
									}
								}
								
							}

							$payablez = array();
							$db = new Db();
							$select = $db->select("SELECT distinct bn_customer_payable_to FROM boundary_node WHERE bn_customer = '$customer_id'");
							foreach($select[0] as $row){
								extract($row);

								$sel = $db->select("SELECT bn_metering_point FROM boundary_node WHERE bn_customer_payable_to = '$bn_customer_payable_to' AND bn_customer = '$customer_id' ");
								foreach($sel[0] as $ro){
									extract($ro);
									//$payablez[$bn_customer_payable_to][] = $bn_metering_point;
									$zz = rate_calculate($customer_id, strtotime(date("$year-$month-15")), $bn_metering_point);

									if($zz[24]!="([DONOT_DISPLAY])")
									{
										$payablez[$bn_customer_payable_to][] = $bn_metering_point;
									}
								}
							}

							?>
							<tr>
								<td rowspan="2">Wheeling Charges</td>
								<td>Payable To <b>(Add)</b></td>
								<td><?php 
									$payable = 0;
									foreach($payablez as $pay => $values){

										//AT KIL
										if($customer_id == 1017 && $pay == 2019 && $rea_date < strtotime(date('2019-08-18'))){

										}elseif(0){
											$j = $this->krecsAndUedcl($pay, $customer_id, $year, $month);					
										}elseif($customer_id == 2021 && $pay==2017){
											$j = $this->collectable2($pay, $customer_id, $year, $month, $consider=0);
										}else{						
											$j = $this->collectable($pay, $customer_id, $year, $month, $consider=0);
										}
										/*
										echo $v = '<b>'.$this->rgf("customer", $pay, "customer_id", "customer_short_name").' ('.number_format($j[2]).' '.$symbol.'):</b>';

										echo '<span style="font-size:12px"> ';

										echo $v = implode(", ", $j[1]).'  </span> <br/>';
										*/

										$v12[] = '<b>'.$this->rgf("customer", $pay, "customer_id", "customer_short_name").'</b>';

										if($customer_id != 2020){
											//echo $v = '('.implode(", ", $j[1]).') ';
										}else{ 
											//echo '<br/>'; 
										}

										$payable += $j[2];
										$i++;
									}
									
									echo implode(', ', $v12);

								?></td>

								<td align="right"><?php  echo ($symbol == '$')? number_format($payable,2):number_format($payable); ?></td>
								<!-- <td colspan="2">Collectable(Less)</td> -->
								<!-- <td align="right"><?php // $payable = array_sum($h[0]); echo number_format($payable). " $symbol"; ?></td> -->

							</tr>
							<tr>
								<td>Collectable From <b>(Less)</b></td>
								<td style="width:40%;"><?php 
									$i=1;
									$v = array();
									$collectable = 0;
									if($customer_id == 2022){
										$payables[2020] = 5312;
									}
									foreach($payables as $pay=>$value){

										//AT KIL
										if($customer_id == 1017 && $pay == 2019 && $rea_date < strtotime(date('2019-08-18'))){

										}elseif(0){
											$jm = $this->krecsAndUedcl($customer_id, $pay, $year, $month);					
										}elseif(($customer_id == 2020 || $customer_id == 2040) && $pay==2017 ){
											$jm = $this->collectable2($customer_id, $pay, $year, $month, $consider=0);
										}else{						
											$jm = $this->collectable($customer_id, $pay, $year, $month, $consider=0);
										}

										/*
										$v .= '<b>'.$this->rgf("customer", $pay, "customer_id", "customer_short_name").' ('.number_format($jm[2]).' '.$symbol.'):</b>'; 
										echo '  <span style="font-size:12px">';

										$v .= implode(", ", $jm[1]).'</span><br/>';
										*/



										$v[] = '<b>'.$this->rgf("customer", $pay, "customer_id", "customer_short_name").'</b>'; 

										$collectable += $jm[2];
										$i++;
									}

									echo implode(', ',$v);
									?></td>
								<td align="right"><?php echo ($symbol == '$')?'('.number_format($collectable,2). ") ":'('.number_format($collectable). ") "; ?></td>
							</tr>
							<?php 
							$tbf = $this->totalBackflow($id, $year, $month);
							if($customer_id == 2019 && 0){ 

							?>
							<tr>
								<td colspan="2">Backflows</td>
								<td align="right"><?php echo number_format($tbf[1],2); ?></td>
								<td align="right">(<?php echo number_format($tbf[5]); ?>)</td>
							</tr>
							<?php
							} 
							?>
							<?php $tta += $payable+($collectable*-1); ?>
							<tr>
								<td colspan="3">Total (VAT Exclusive)</td>
								<td align="right"><?php echo ($symbol=='$')?number_format($tta,2):number_format($tta); ?></td>
							</tr>							
							<tr>
								<td colspan="3">Add <?php echo ($symbol == '$')?number_format($customer_vat,2):number_format($customer_vat); ?>% VAT</td>
								<?php $tot = $customer_vat/100*$tta; ?>
								<td align="right"><?php echo number_format($tot). " "; ?></td>
							</tr>							
							<tr>
								<td colspan="3"><b>Total Amount (VAT Incl)</b></td>
								<?php $tot = $customer_vat/100*$tta; ?>
								<td align="right"><b><?php echo ($symbol == '$')?number_format($tta*($customer_vat/100+1), 2): number_format($tta*($customer_vat/100+1)); ?></b></td>							
							<tr>
								<td colspan="4">
								<?php $tot = $tta*($customer_vat/100+1); if($tot<200000000000){?>
								Total in words:<br/>
								<?php 
								echo '<b>';
								if($symbol != '$'){
									echo number_to_words($tta*($customer_vat/100+1));
								}else{
									$t = $tta*($customer_vat/100+1);
									$tt = (int)$t;
									if($tt != $t){
										$d = round($t - $tt,2);

										echo number_to_words($t);
										if(!empty($d)){
											echo ' Point ';
											$g = $d*10;
											$g2 = (int)$g;
											echo number_to_words($g2);
											echo ' ';
											echo number_to_words((int)(($g-$g2)*10));
										}
									}else{
										echo number_to_words($t);
									}
								}
								echo " $currency".'</b>'; 
								?>
								</td>
								<?php } ?>
							</tr>
							<?php if($oi_exchange_rate && $currency =='Dollars'){?>
							<tr>
							<td colspan="4">
								Exhange Rate:
								<?php 
								//echo ;
									echo number_format($oi_exchange_rate,2);
								echo " $currency".''; 
								?>
								</td>
							</tr>
						<?php }?>
						</table>
											<?php }else{	
						//echo 'her';

							$units = 0.001;
						$m = $this->sumFormulaeX($customer_id, $year, $month);
						//echo '->'.$m[0].'->'.$units;
						$t1 = ($m[0]*$units);
						$t2 = ($m[1]*$units);
						$t3 = ($m[2]*$units);
						$t4 = ($m[3]*$units);

							$units = 0.001;
						$mx = $this->sumFormulae($customer_id, $year, $month, 1);
						//echo '->'.$xm[0].'->'.$units;
						$t1x = ($mx[0]*$units);
						$t2x = ($mx[1]*$units);
						$t3x = ($mx[2]*$units);
						$t4x = ($mx[3]*$units);


						$ttu = $tta = 0;
						?>
						<style>
							#x tr td, #x tr th{
								padding:2px 10px;
							}
							#x tr th{
								text-align: center;
							}
						</style>
						<table cellpadding="5" style="font-family: arial; font-size: 10pt;" cellspacing="0" border="1" width="100%" id="x">
							<tr>
								<th rowspan="2">TIME BAND</th>
								<th colspan="3" style="width:30%;">PREVIOUS </th>
								<th colspan="3" style="width:30%;">CURRENT</th>
								<th colspan="2" style="width:20%;">VARIANCE</th>
							</tr>
							<tr>
								<th style="width:30%;">UNITS - 
									<?php
									echo '(';
									if($units == 1){
										echo 'Wh';
									}elseif($units == 0.001){
										echo 'KWh';
									}elseif($units == 0.000001){
										echo 'MWh';
									}
									echo ')';
									?>
								</th>
								<th style="width:30%;">RATE - <?php echo "($symbol / kWh)"; ?></th>
								<th>AMOUNT<?php echo " ($symbol)"; ?></th>
								<th>UNITS - 
									<?php
									echo '(';
									if($units == 1){
										echo 'Wh';
									}elseif($units == 0.001){
										echo 'KWh';
									}elseif($units == 0.000001){
										echo 'MWh';
									}
									echo ')';
									?>
								</th>
								<th>RATE - <?php echo "($symbol / kWh)"; ?></th>
								<th>AMOUNT<?php echo " ($symbol)"; ?></th>
								<th>UNITS - 
									<?php
									echo '(';
									if($units == 1){
										echo 'Wh';
									}elseif($units == 0.001){
										echo 'KWh';
									}elseif($units == 0.000001){
										echo 'MWh';
									}
									echo ')';
									?>
								</th>
								<th>AMOUNT<?php echo " ($symbol)"; ?></th>
							</tr>
							<tr>

								<?php 
								
								$toux = $this->tou($customer_id, $month, $year,1);
								$tou = $this->tou($customer_id, $month, $year);

								$tta = $ttax = 0;
				
								?>
								<td>Peak@</td>
								<td align="right"><?php echo number_format($t1-$tbf[0],2); ?></td>
								<td align="right"><?php echo ((float)$tou[1]). ""; ?></td>
								<td align="right"><?php echo number_format(($t1-$tbf[0])*$tou[1],2); ?></td>
								<?php $tta += ($t1-$tbf[0])*$tou[1]; ?>

								<!-- Previous -->
								<td align="right"><?php echo number_format($t1x-$tbf[0],2); ?></td>
								<td align="right"><?php echo ((float)$toux[1]). ""; ?></td>
								<td align="right"><?php echo number_format(($t1x-$tbf[0])*$toux[1],2); ?></td>
								<?php $ttax += ($t1x-$tbf[0])*$toux[1]; ?>
								
								<!-- variance -->
								<td align="right"><?php echo number_format($t1x-$tbf[0] - ($t1-$tbf[0]),2); ?></td>
								<td align="right"><?php echo number_format(($t1x-$tbf[0])*$toux[1] - (($t1-$tbf[0])*$tou[1]),2); ?></td>
							</tr>
							<tr>
								
								<td>Shoulder</td>
								<td align="right"><?php echo number_format($t2-$tbf[1],2); ?></td>
								<td align="right"><?php echo ((float)$tou[0]). ""; ?></td>
								<td align="right"><?php echo number_format(($t2-$tbf[1])*$tou[0],2); ?></td>

								<?php $tta += ($t2-$tbf[1])*$tou[0]; ?>
								<!-- Previous -->
								<td align="right"><?php echo number_format($t2x-$tbf[1],2); ?></td>
								<td align="right"><?php echo ((float)$toux[0]). ""; ?></td>
								<td align="right"><?php echo number_format(($t2x-$tbf[1])*$toux[0],2); ?></td>
								<?php $ttax += ($t2x-$tbf[1])*$toux[0]; ?>								
								
								<!-- variance -->
								<td align="right"><?php echo number_format($t2x-$tbf[1] - ($t2-$tbf[1]),2); ?></td>
								<td align="right"><?php echo number_format(($t2x-$tbf[1])*$toux[0] -(($t2-$tbf[1])*$tou[0]),2); ?></td>
							</tr>
							<tr>
								
								<td>Off Peak</td>
								<td align="right"><?php echo number_format($t3-$tbf[2],2); ?></td>
								<td align="right"><?php echo ((float)$tou[2]). ""; ?></td>
								<td align="right"><?php echo number_format(($t3-$tbf[2])*$tou[2],2); ?></td>

								<?php $tta += ($t3-$tbf[2])*$tou[2]; ?>
								<!-- Previous -->
								<td align="right"><?php echo number_format($t3x-$tbf[2],2); ?></td>
								<td align="right"><?php echo ((float)$toux[2]). ""; ?></td>
								<td align="right"><?php echo number_format(($t3x-$tbf[2])*$toux[2],2); ?></td>
								<?php $ttax += ($t3x-$tbf[2])*$toux[2]; ?>								
								
								<!-- variance -->
								<td align="right"><?php echo number_format($t3x-$tbf[2] - ($t3-$tbf[2]),2); ?></td>
								<td align="right"><?php echo number_format(($t3x-$tbf[2])*$toux[2] - (($t3-$tbf[2])*$tou[2]),2); ?></td>
							</tr>
							<tr>
								<td>Total</td>
								<td align="right"><?php echo number_format($t4,2); ?></td>
								<?php 
									if($symbol == '$'){
										$a = number_format($tta,2);
									}else{
										$a = number_format($tta);
									}
								?>
								<td></td>
								<td align="right" style="color:black; font-weight:bold; background-color:yellow;"><?php echo $a. " "; ?></td>
							
								<td align="right"><?php echo number_format($t4x,2); ?></td>
								<?php 
									if($symbol == '$'){
										$a = number_format($ttax,2);
									}else{
										$a = number_format($ttax);
									}
								?>
								<td></td>
								<td align="right" style="color:black; font-weight:bold; background-color:yellow;"><?php echo $a. " "; ?></td>
								
								<!-- variance -->
								<?php 
									if($symbol == '$'){
										$a = number_format($ttax - $tta,2);
									}else{
										$a = number_format($ttax - $tta);
									}
								?>
								<td></td>
								<td align="right" style="color:black; font-weight:bold; background-color:yellow;"><?php echo $a. " "; ?></td>
							</tr>
							<tr>
								<td rowspan="2">Wheeling Charges</td>
								<td>Payable To <b>(Add)</b></td>
								<td><?php 

								$payablew_ = $this->total_payable($year, $month, $customer_id, 1);
								$collectablew_ = $this->total_collectable($year, $month, $customer_id, 1);

								//$toux = $this->tou($customer_id, $month, $year,1);
								//echo $year2.' => '.$month.' => '.$customer_id;
								$payable_ = $this->total_payable($year, $month, $customer_id);
								$payable = $payable_['payable'];
								echo '<b>'.$payable_['customers'].'</b>';
								?></td>

								<td align="right">
									<?php  
									if($payablew_['payable'] == 0)
										$payable = 0;


									$payable2 = $payablew_['payable'];
									$payable1 = $payable;
									
									
									$payable_v1 = $payable;

										echo ($symbol == '$')? number_format($payable,2):number_format($payable); 
									?></td>
								<!-- <td colspan="2">Collectable(Less)</td> -->
								<!-- <td align="right"><?php // $payable = array_sum($h[0]); echo number_format($payable). " $symbol"; ?></td> -->
								<td></td>
								<td></td>
								<?php
								$payable_ = $this->total_payable($year, $month, $customer_id, 1);
								$payable = $payable_['payable'];
								
								$payable_v2 = $payable;
								
								$payable_v1 = $payable_v2 - $payable_v1;
								
								?>
								<td align="right">
									<?php  echo ($symbol == '$')? number_format($payable,2):number_format($payable); ?></td>
								
								<td></td>
								<td align="right">
									<?php  echo ($symbol == '$')? number_format($payable_v1,2):number_format($payable_v1); ?></td>

							</tr>
							<tr>
								<td>Collectable From <b>(Less)</b></td>
								<td style="width:40%;"><?php 
								$collectable_ = $this->total_collectable($year, $month, $customer_id);
								$collectable = $collectable_['collectable'];
								echo '<b>'.$collectable_['customers'].'</b>';
								?></td>
								<td align="right">
									<?php 

									if($collectablew_['collectable'] == 0)
										$collectable = 0;

									$collectable2 = $collectablew_['collectable'];
									$collectable1 = $collectable;
									
									$collectable_v1 = $collectable;

									echo ($symbol == '$')?'('.number_format($collectable,2). ") ":'('.number_format($collectable). ") "; 
									?></td>
								<td></td>
								<td></td>
								<?php
								$payablew = $collectablew_['collectable'];
								$collectablew_['customers'];
								
								$collectable_v1 -= $payablew;
								?>
								<td align="right"><?php  
								echo ($symbol == '$')?'('.number_format($payablew,2). ") ":'('.number_format($payablew). ") "; 
								?></td>
								
								<td></td>
								<td align="right"><?php  
								echo ($symbol == '$')?'('.number_format($collectable_v1,2). ") ":'('.number_format($collectable_v1). ") "; 
								?></td>
								
							</tr>
							
							<?php

							echo '<tr>';
							echo '<td colspan="7">';

							$tta += $collectable1 - $collectable2;
							$ttax += $payable1 - $payable2;

							if($ttax == $tta){
								echo "&nbsp;";
							}else if($ttax > $tta){
								echo 'Debit Note: = ('.number_format($ttax,2).' - '.number_format($tta,2).')';
								$GGG = ($ttax+$tta*-1);
							}else{
								echo 'Credit Note: <br/>= ('.number_format($tta,2).' - '.number_format($ttax,2).')';
								$GGG = ($tta+$ttax*-1);
							}

								
							if($ttax != $tta)
								echo ' = <span style="font-weight:bold;">'.number_format($GGG,2).'</span>';

							echo '</td>';
							echo '</tr>';

							$tta = $GGG;
							?>
							<tr>
								<td colspan="8">Add <?php echo ($symbol == '$')?number_format($customer_vat,2):number_format($customer_vat); ?>% VAT</td>
								<?php $tot = $customer_vat/100*$tta; ?>
								<td align="right"><?php echo number_format($tot). " "; ?></td>
							</tr>							
							<tr>
								<td colspan="8"><b>Total Amount (VAT Incl)</b></td>
								<?php $tot = $customer_vat/100*$tta; ?>
								<td align="right"><b><?php echo ($symbol == '$')?number_format($tta*($customer_vat/100+1), 2): number_format($tta*($customer_vat/100+1)); ?></b></td>							
							<tr>
								<td colspan="9">
								<?php $tot = $tta*($customer_vat/100+1); if($tot<200000000000){?>
								Total in words:<br/>
								<?php 
								echo '<b>';
								if($symbol != '$'){
									echo number_to_words($tta*($customer_vat/100+1));
								}else{
									$t = $tta*($customer_vat/100+1);
									$tt = (int)$t;
									if($tt != $t){
										$d = round($t - $tt,2);

										echo number_to_words($t);
										if(!empty($d)){
											echo ' Point ';
											$g = $d*10;
											$g2 = (int)$g;
											echo number_to_words($g2);
											echo ' ';
											echo number_to_words((int)(($g-$g2)*10));
										}
									}else{
										echo number_to_words($t);
									}
								}
								echo " $currency".'</b>'; 
								?>
								</td>
								<?php } ?>
							</tr>
							<?php if($oi_exchange_rate && $currency =='Dollars'){?>
							<tr>
							<td colspan="9">
								Exhange Rate:
								<?php 
								//echo ;
									echo number_format($oi_exchange_rate,2);
								echo " $currency".''; 
								?>
								</td>
							</tr>
						<?php }?>
						</table>
						<?php } ?>

						<span style="color:red; font-weight: bold; font-size: 12px; text-align: center; ">
							<?php 
							echo $this->rgf("customer", $customer_id, "customer_id", "customer_accounts_due");
							?>
						</span>						
						<?php $this->showReadingComments(); ?>	
						
						<?php $this->energyApprovals(user_id(), $year, $month, 1, 1, $customer_id); ?>
					</td>				
				</tr>
				
			</tbody></table>
			
		</div>
		<?php 
		
		if($this->anyCustomerReading($year, $month))
		{	
			$this->energyApprovals(user_id(), $year, $month,0, $tab, $customer_id);
		}

		?>
		<div class="clearfix"></div>
		<?php 
		echo '<div style="width:900px;margin:auto;">';
		$this->addReadingComment();
		echo '</div>';
		?>
	</div>
	<?php
	}

	public function krecsAndUedcl($customer_payable, $customer_collectable, $year, $month){
		
		//if($customer_collectable == 2022 && $customer_payable == 2020)
		{

			$rea_date = strtotime(date("$year $month 15"));

			$tou = $this->tou($customer_payable, $month, $year);		
			$peak = $tou[0];
			$shoulder = $tou[1];
			$off_peak = $tou[2];

			if($customer_collectable == 2022 && $customer_payable == 2020)
				$cal = rate_calculate2($customer_collectable, ($rea_date), '5315CUSTOM'); // kiseruka ==> UEDCL
			else

				$cal = rate_calculate2($customer_payable, ($rea_date), '5315CUSTOM'); 
				//$cal = rate_calculate($customer_collectable, ($rea_date), 5315); // kiseruka ==> UEDCL
			
			//$to = ($cal[4]*$shoulder+$cal[5]*$peak+$cal[6]*$off_peak)/1000;
			//echo 'here now';
			if($customer_payable == 2020){

				$tou = $this->tou($customer_collectable, $month, $year);		
				$peak = $tou[0];
				$shoulder = $tou[1];
				$off_peak = $tou[2];

				$to1 = ($cal[4]) /1000*5.68/100;
				$to2 = ($cal[5]) /1000*5.68/100;
				$to3 = ($cal[6]) /1000*5.68/100;

			}elseif($customer_payable == 2020){

				$tou = $this->tou($customer_collectable, $month, $year);		
				$peak = $tou[0];
				$shoulder = $tou[1];
				$off_peak = $tou[2];

				$to1 = ($cal[4]) /1000*5.68/100;
				$to2 = ($cal[5]) /1000*5.68/100;
				$to3 = ($cal[6]) /1000*5.68/100;

			}else{
				$tou = $this->tou($customer_collectable, $month, $year);		
				$peak = $tou[0];
				$shoulder = $tou[1];
				$off_peak = $tou[2];

				$to1 = ($cal[0]) /1000*5.68/100;
				$to2 = ($cal[1]) /1000*5.68/100;
				$to3 = ($cal[2]) /1000*5.68/100;
			}
			//echo $cal[4];
			//$to1 = ($cal[4]); //*$shoulder/1000;
			//$to2 = ($cal[5]); //*$peak/1000;
			//$to3 = ($cal[6]); //*$off_peak/1000;

			//$to1 = $shoulder;
			//$to2 = $peak;
			//$to3 = $off_peak;

			$to = $to1*$shoulder+$to2*$peak+$to3*$off_peak;

			$to4 = $to1+$to2+$to3;

		}

		$all = array(
			array(5315), 
			array("Kiseruka"), 
			$to, 
									// 2 ==> total amount payable by the customer
			$to1, 
			$to2, 
			$to3, 
			$to4
		);

		return $all;

	}

	public function templateUploadAction(){
		
		echo '<div>';
		echo '<form action="" method="post" class="search" enctype="multipart/form-data">';

		if(isset($_POST['upload'])){
			$colum_names = array('year', 'month', 'customer', 'units');
			$action = array('action');
			$action_values = array('COMPARE','CHANGE','REMOVE','ADD','COM', 'CHA', 'REM');
			$units = array('WH', 'KWH', 'MWH');
			
			//$readings = array('METERING POINT', 'Imports Shoulder(R1)', 'Imports Peak(R2)', 'Imports Off Peak(R3)', 'Exports Shoulder(R4)', 'Exports Peak(R5)', 'Exports Off Peak(R6)');

			$readings = array('METERING POINT', 'RATE 1', 'RATE 2', 'RATE 3', 'RATE 4', 'RATE 5', 'RATE 6');
			$error = array();		
			$filename=$_FILES["file"]["tmp_name"];

			if($_FILES["file"]["size"] > 0){					
				$file = fopen($filename, "r");
				$count = 0;
				$error = array();
				while (($emapData = fgetcsv($file, 10000, ",")) !== FALSE){
					$count++;
					
					$col_1 = @strtoupper(trim(str_replace("'","\'",strip_tags($emapData[0]))));
					$col_2 = @strtoupper(trim(str_replace("'","\'",strip_tags($emapData[1]))));
					$col_3 = @strtoupper(trim(str_replace("'","\'",strip_tags($emapData[2]))));
					$col_4 = @strtoupper(trim(str_replace("'","\'",strip_tags($emapData[3]))));
					$col_5 = @strtoupper(trim(str_replace("'","\'",strip_tags($emapData[4]))));
					$col_6 = @strtoupper(trim(str_replace("'","\'",strip_tags($emapData[5]))));
					$col_7 = @strtoupper(trim(str_replace("'","\'",strip_tags($emapData[6]))));			
					
					if($count == 5) continue;

					if($count < 6){

						if(strtolower($col_1) != $colum_names[$count-1]){
							$error[] = "Invalid Template. Please Download the Correct Template.";
							break;
						}

						if($count == 1){
							if(strtolower($col_6) != $action[0]){
								$error[] = "Invalid Template. Please Download the Correct Template.";
								break;
							}

							if(!in_array($col_7, $action_values)){
								$error[] = '<b>Check Line '.$count.':</b> Units should be one of the following: <b>'.implode(',', $units).'</b>';
							}else{
								$action_to_use = $col_7;
							}
						}

						if($col_2 == ""){
							$error[] = '<b>Check Line '.$count.':</b>'.$colum_names[$count-1]." value in B$count, should not be empty.";
						}

						if($count == 4){
							if(!in_array($col_2, $units)){
								$error[] = '<b>Check Line '.$count.':</b> Units should be one of the following: <b>'.implode(',', $units).'</b>';
							}else{
								$unit_to_use = $col_2;
							}
						}

					}else{
						for($i=1; $i<=7; $i++){
							if(${"col_".$i} != $readings[$i-1]){
								$error[] = '<b>Check Line '.$count.':</b> Invalid Template Name <b>'.${"col_".$i}.'</b>';
							}
						}
					}

					if($count == 6) break;					
				}

				//check customer existing
				if(empty($error)){
					$file = fopen($filename, "r");
					$count = 0;
					$error = array();
					$section = "";
					$isReached = False;
					$value = array();
					$unit = array();
					while (($emapData = fgetcsv($file, 10000, ",")) !== FALSE){
						$count++;						
						$col_2 = @strtoupper(trim(str_replace("'","\'",strip_tags($emapData[1]))));

						if($count == 3){
							$db = new Db();
							$select = $db->select("SELECT customer_id FROM customer WHERE customer_full_name = '$col_2' OR customer_short_name = '$col_2'");
							
							if($db->num_rows()==0){
								$error[] = '<b>Check Line '.$count.':</b> Customer Name <b>('.$col_2.')</b> does not exist in the System.';
							}else{
								extract($select[0][0]);
								//customer_id
							}
						}
					}
				}

				//check if metering point exist
				$mp_ids = array();
				$mepo = array();
				if(empty($error)){
					$file = fopen($filename, "r");
					$count = 0;
					$error = array();
					$section = "";
					$isReached = False;
					$value = array();
					$unit = array();
					while (($emapData = fgetcsv($file, 10000, ",")) !== FALSE){
						$count++;						
						$col_2 = @strtoupper(trim(str_replace("'","\'",strip_tags($emapData[0]))));

						if($count >= 7){
							$mepo[] = $col_2;
							$db = new Db();
							$select = $db->select("SELECT mp_id FROM metering_point WHERE mp_customer_id = '$customer_id' AND mp_location = '$col_2'");

							if($db->num_rows()==0){
								$error[] = '<b>Check Line '.$count.':</b> Metering Point <b>('.$col_2.')</b> does not exist in the System.';
							}else{
								extract($select[0][0]);
								$mp_ids[] = $mp_id;
							}
						}
					}
				}
				
				
				//check if isLocked
				if(empty($error)){
					$file = fopen($filename, "r");
					$count = 0;
					$error = array();
					$section = "";
					$isReached = False;
					$value = array();
					$unit = array();
					$year = $month = "";
					while (($emapData = fgetcsv($file, 10000, ",")) !== FALSE){
						$count++;						
						$col_2 = @strtoupper(trim(str_replace("'","\'",strip_tags($emapData[1]))));

						if($count == 1){
							$year = $col_2;
						}else if($count == 2){
							$month = $col_2;
						}
					}

					if($this->isLocked($customer_id, $year, $month)){
						$error[] = "Readings are locked for  &nbsp;  &nbsp;  &nbsp; <b>Customer: </b>".$this->rgf("customer", $customer_id, "customer_id", "customer_short_name")." &nbsp;  &nbsp;  &nbsp; <b>Year: </b>".$year."  &nbsp;  &nbsp;  &nbsp;  <b>Month: </b>".$month;
					}

				}
				

				if(strtolower($action_to_use)=="change" || strtolower($action_to_use) == "cha" || strtolower($action_to_use) == "remove" || strtolower($action_to_use) == "rem"){
					if(empty($error)){
						$file = fopen($filename, "r");
						$count = 0;
						$error = array();
						$section = "";
						$isReached = False;
						$value = array();
						$unit = array();
						$year = $month = "";
						while (($emapData = fgetcsv($file, 10000, ",")) !== FALSE){
							$count++;						
							$col_2 = @strtoupper(trim(str_replace("'","\'",strip_tags($emapData[1]))));

							if($count == 1){
								$year = $col_2;
							}else if($count == 2){
								$month = $col_2;
							}
						}
					}
				}else{

					//check if reading not uploaded
					if(empty($error)){
						$file = fopen($filename, "r");
						$count = 0;
						$error = array();
						$section = "";
						$isReached = False;
						$value = array();
						$unit = array();
						$year = $month = "";
						while (($emapData = fgetcsv($file, 10000, ",")) !== FALSE){
							$count++;						
							$col_2 = @strtoupper(trim(str_replace("'","\'",strip_tags($emapData[1]))));

							if($count == 1){
								$year = $col_2;
							}else if($count == 2){
								$month = $col_2;
							}
						}

						for($i=0; $i<count($mp_ids); $i++)
						{
							$mp_id = $mp_ids[$i];
							$db = new Db();
							$select = $db->select("SELECT * FROM metering_point, r_reading WHERE mp_customer_id = '$customer_id' AND rea_cus_id = '$customer_id' AND mp_id = rea_mp_id AND mp_id = '$mp_id'");
							foreach($select[0] as $row){
								extract($row);								
								if(date('Y', $rea_date)==$year && date('m', $rea_date)==$month){
									$error[] = '<b>Check Line '.(6+$i).':</b> Metering Point (<b>'.$mepo[$i].'</b>) Readings already exist.'; 
								}
							}
						}

					}
				}

				//upload final
				$count = 0;
				if(empty($error)){				
					
					$time = time();
					$user_id = user_id();
					$time_readings = strtotime(date($year.'-'.$month.'-15'));
					$file = fopen($filename, "r");
					while (($emapData = fgetcsv($file, 10000, ",")) !== FALSE){
						$count++;	

						if($unit_to_use == 'WH')
							$use = 1;
						else if($unit_to_use == 'KWH')
							$use = 1000;
						else if($unit_to_use == 'MWH')
							$use = 1000000;

						$point = @strtoupper(trim(str_replace("'","\'",strip_tags($emapData[0]))));
						$rate1 = $use*str_replace(',', '', @strtoupper(trim(str_replace("'","\'",strip_tags($emapData[1])))));
						$rate2 = $use*str_replace(',', '', @strtoupper(trim(str_replace("'","\'",strip_tags($emapData[2])))));
						$rate3 = $use*str_replace(',', '', @strtoupper(trim(str_replace("'","\'",strip_tags($emapData[3])))));
						$rate4 = $use*str_replace(',', '', @strtoupper(trim(str_replace("'","\'",strip_tags($emapData[4])))));
						$rate5 = $use*str_replace(',', '', @strtoupper(trim(str_replace("'","\'",strip_tags($emapData[5])))));
						$rate6 = $use*str_replace(',', '', @strtoupper(trim(str_replace("'","\'",strip_tags($emapData[6])))));

						$rate1 = str_replace('-','',$rate1);
						$rate2 = str_replace('-','',$rate2);
						$rate3 = str_replace('-','',$rate3);
						$rate4 = str_replace('-','',$rate4);
						$rate5 = str_replace('-','',$rate5);
						$rate6 = str_replace('-','',$rate6);

						if($count >= 7){
							if(strtolower($action_to_use)=="add"){
								if($point != ""){
									$db = new Db();
									$select = $db->select("SELECT mp_id,mp_advance FROM metering_point WHERE mp_customer_id = '$customer_id' AND mp_location = '$point'");
									extract($select[0][0]);

									$insert = $db->insert("r_reading",[
										"rea_number"=>$file_name,
										"rea_date"=>$time_readings,
										"rea_cus_id"=>$customer_id,
										"rea_added_by"=>user_id(),
										"rea_date_added"=>time(),
										"rea_mp_id"=>$mp_id,
									]);					
									
									$db = new Db();
									$select = $db->select("SELECT TOP 1 rea_id FROM r_reading ORDER BY rea_id DESC");
									extract($select[0][0]);
									$rrr = $rea_id;


									$sql = "SELECT TOP 1 rate_sfi, rate_sfe, rate_lfi, rate_lfe rate_narration, rate_label FROM r_rate, r_reading WHERE rate_cus_id = '$customer_id' AND rea_id = rate_reading_id AND rea_cus_id = '$customer_id' AND rea_mp_id = '$mp_id' ORDER BY rate_id DESC";
									$select = $db->select($sql);
									extract($select[0][0]);
									
									//=====================================================================
									//checking which part to use for advance
									//=====================================================================
									//echo $mp_advance;

									if($mp_advance == 2){
										$ad = 1;
									}else{
										$ad = 0;
									}
									if(1){ //imports as advance
										$select = $db->insert("r_rate",[
											"rate_import_wh_1"=>$rate1,
											"rate_import_wh_2"=>$rate2,
											"rate_import_wh_3"=>$rate3,
											"rate_export_wh_4"=>$rate4,
											"rate_export_wh_5"=>$rate5,
											"rate_export_wh_6"=>$rate6,
											"rate_reading_id"=>$rrr,
											"rate_added_by"=>user_id(),
											"rate_date_added"=>time(),
											"rate_cus_id"=>$customer_id,
											"rate_swap"=>$ad,
											"rate_sfi"=>$rate_sfi,
											"rate_sfe"=>$rate_sfe,
											"rate_lfi"=>$rate_lfi,
											"rate_lfe"=>$rate_lfe,
											"rate_narration"=>$rate_narration,
											"rate_label"=>$rate_label
										]);
									}
								

								}
							}elseif(strtolower($action_to_use)=="change" || strtolower($action_to_use) == "cha"){
								if($point != ""){
									$db = new Db();
									$select = $db->select("SELECT mp_id,mp_advance FROM metering_point WHERE mp_customer_id = '$customer_id' AND mp_location = '$point'");
									extract($select[0][0]);

									$select = $db->select("SELECT * FROM r_reading WHERE rea_date = '$time_readings' AND rea_cus_id = '$customer_id' AND rea_mp_id = '$mp_id'");

									if($db->num_rows()){
										extract($select[0][0]);
										$rrr = $rea_id;

										//=====================================================================
										//checking which part to use for advance
										//=====================================================================

										if($mp_advance == 2){
											$ad = 1;
										}else{
											$ad = 0;
										}
										if(1){ //imports as advance
											$select = $db->update("r_rate",[
												"rate_import_wh_1"=>$rate1,
												"rate_import_wh_2"=>$rate2,
												"rate_import_wh_3"=>$rate3,
												"rate_export_wh_4"=>$rate4,
												"rate_export_wh_5"=>$rate5,
												"rate_export_wh_6"=>$rate6,
												"rate_swap"=>$ad,
												"rate_added_by"=>user_id(),
												"rate_date_added"=>time(),
											], ["rate_cus_id"=>$customer_id, "rate_reading_id"=>$rrr]);
										}
									}else{
										$error[] = "Readings Do not Exist for <b>".$this->rgf('customer',$customer_id, 'customer_id', 'customer_short_name').'</b> Metering Point <b>'.$point.'</b>';
									}
								}
							}elseif(strtolower($action_to_use)=="remove" || strtolower($action_to_use) == "rem"){
								if($point != ""){
									$db = new Db();
									$select = $db->select("SELECT mp_id,mp_advance FROM metering_point WHERE mp_customer_id = '$customer_id' AND mp_location = '$point'");
									extract($select[0][0]);

									$select = $db->select("SELECT * FROM r_reading WHERE rea_date = '$time_readings' AND rea_cus_id = '$customer_id' AND rea_mp_id = '$mp_id'");

									if($db->num_rows()){
										extract($select[0][0]);
										$rrr = $rea_id;

										$db = new Db();
										$sql = "DELETE FROM r_rate WHERE rate_cus_id = '$customer_id' AND rate_reading_id = '$rrr'";
										$delete = $db->query($sql);

										$db = new Db();
										$sql = "DELETE FROM r_reading WHERE rea_id = '$rrr'";
										$delete = $db->query($sql);
										
									}else{
										$error[] = "Readings Do not Exist for <b>".$this->rgf('customer',$customer_id, 'customer_id', 'customer_short_name').'</b> Metering Point <b>'.$point.'</b>';
									}
								}
							}
						}

					}

					if($insert){
						//FeedBack::success();
					}else{
						//FeedBack::error("Error Occured".));
					}
				}
				
			}else{
				$error[] = "Attach a CSV File.";
			}

			if(!empty($error)){
				FeedBack::errors($error);
			}else{
				FeedBack::refresh();
				FeedBack::success();
			}

		}

		echo '<div class="pull-left">';
		echo '<h5>Upload File (CSV)</h5>';
		echo '<div class="clearfix"></div>';
		echo '<input type="file" name="file"/>';
		echo '</div>';
		echo '<div class="pull-left">';
		echo '<br/>';
		echo '<p></p>';
		echo '<button name="upload" type="submit"> <i class="fa fa-fx fa-upload"></i> Upload</button>';
		echo '</div>';
		
		echo '<div class="clearfix"></div>';
		echo '</form>';
		$current = date('Y-M', strtotime('-1 month', time()));
		echo '<h4>Download Readings Templates '.$current.'</h4>';
		$db = new Db();
		$sql = "SELECT * FROM customer ORDER BY customer_short_name ASC";
		echo '<div class="row">';
		$select = $db->select($sql);
		foreach($select[0] as $row){
			extract($row);
			if($customer_id != 2030 && $customer_id != 2029 && $customer_id != 2028){
				echo '<div class="col-md-2">';
				echo '<a title="Download CSV" href="'.return_url().'classes/download-csv.php?cus='.$customer_id.'"><i class="fa fa-fw fa-download"></i> '.$customer_short_name.'</a> ';
				echo '</div>';
			}
		}
		echo '</div>';

		echo '<br/>';
		echo '<h4>Elster Metering System</h4>';
		echo '<form action="" method="post" class="search" enctype="multipart/form-data">';
		echo '<fieldset>';

		if(isset($_POST['importElester'])){				
			$columns = array(
				"From unit", //1
				"Cumulative totals",//2
				"Multi-utility registers", //3
				"Rates", //4
				"Maximum demands", //5
				"Co-incident demands", //6
				"Cumulative maximum demands", //7
				"Billing event details"//8
			);

			$error = array();	
			$customer_id = $_POST['customer_id'];
			$metering_point_id = $_POST['metering_point_id'];
			$year_uploaded = $_POST['year'];
			$month_uploaded = $_POST['month'];

			$time_readings = strtotime(date($year_uploaded.'-'.$month_uploaded.'-15'));
			
			if($year_uploaded == ""){
				$error[] = "Select Year";
			}
			if($month_uploaded == ""){
				$error[] = "Select Month";
			}
			if($customer_id == ""){
				$error[] = "Select Customer Name";
			}
			if($metering_point_id == ""){
				$error[] = "Select Metering Point";
			}	

			if($this->isLocked($customer_id, $year_uploaded, $month_uploaded)){
				$errors[] = "Readings are locked for  &nbsp;  &nbsp;  &nbsp; <b>Customer: </b>".$this->rgf("customer", $customer_id, "customer_id", "customer_short_name")." &nbsp;  &nbsp;  &nbsp; <b>Year: </b>".$year_uploaded."  &nbsp;  &nbsp;  &nbsp;  <b>Month: </b>".$month_uploaded;
			}

			$filename=$_FILES["file"]["tmp_name"];
			if($_FILES["file"]["size"] > 0){					
				$file = fopen($filename, "r");
				$count = 0;

				if(empty($error)){
					while (($emapData = fgetcsv($file, 10000, ",")) !== FALSE){
						$count++;
						
						$col_1 = @strtoupper(trim(str_replace("'","\'",strip_tags($emapData[0]))));
						$col_2 = @strtoupper(trim(str_replace("'","\'",strip_tags($emapData[1]))));
						$col_3 = @strtoupper(trim(str_replace("'","\'",strip_tags($emapData[2])))); 
						$col_4 = @strtoupper(trim(str_replace("'","\'",strip_tags($emapData[3])))); 
												
						//checking file required file
						$string = strtoupper("Elster Metering Systems Power Master Unit");
						
						if($count == 1){
							if($col_2 != $string){
								$error[] = "Invalid File";
							}
						}						
					}
				}					
								
				//start From Unit
				if(empty($error)){
					$file = fopen($filename, "r");
					$count = 0;
					
					$section = "";
					$isReached = False;
					$value = array();
					$unit = array();
					while (($emapData = fgetcsv($file, 10000, ",")) !== FALSE){
						$count++;
						
						$col_1 = @(trim(str_replace("'","\'",strip_tags($emapData[0]))));
						$col_2 = @(trim(str_replace("'","\'",strip_tags($emapData[1]))));
						$col_3 = @(trim(str_replace("'","\'",strip_tags($emapData[2])))); 
						$col_4 = @(trim(str_replace("'","\'",strip_tags($emapData[3]))));
						
						if($col_1 == $columns[0]){									
							$isReached = True;
							$re = explode(' ',$col_2);										
							$file_name = $re[0];
							$date = $re[1];
							$time = $re[2];
							$time_format = $re[3];
													
							//$time_readings = date('Y-m-d H:i:s a', strtotime($date.' '.$time.' '.$time_format));
							$tim = time();
							$user_id = user_id();
							$dx = new Db();
							ECHO $sql = "SELECT * FROM r_reading WHERE rea_date = '$time_readings' AND rea_mp_id = '$metering_point_id' AND rea_cus_id = '$customer_id'";
							$check = $dx->select($sql);
							if($dx->num_rows() > 0 ){
								$error[] = "SSReadings already exit for  &nbsp;  &nbsp;  &nbsp; <b>Customer: </b>".$this->rgf("customer", $customer_id, "customer_id", "customer_short_name")." &nbsp;  &nbsp;  &nbsp; <b>Year: </b>".$year_uploaded."  &nbsp;  &nbsp;  &nbsp;  <b>Month: </b>".$month_uploaded;
								break;
							}

							if(empty($error)){
								$db = new Db();
								$sql = "INSERT INTO r_reading(rea_number, rea_date, rea_cus_id, rea_added_by, rea_date_added, rea_mp_id) VALUES ('$file_name','$time_readings','$customer_id','$user_id','$tim','$metering_point_id')";
								$insert = $db->query($sql);
								
								if($db->error()){									
									$error[] = "Not inserted";
									echo $db->error();
								}else{
									$db = new Db();
									$select = $db->select("SELECT TOP 1 rea_id FROM r_reading ORDER BY rea_id DESC");
									extract($select[0][0]);
									$rrr = $rea_id;
								}
							}
							
							break;
						}
					}
				}//ending FROM unit
								
				//4. starting Rates
				if(empty($error)){
					$file = fopen($filename, "r");
					$count = 0;

					$section = "";
					$isReached = False;
					$value = array();
					$unit = array();
					while (($emapData = fgetcsv($file, 10000, ",")) !== FALSE){
						$count++;
						
						$col_1 = @(trim(str_replace("'","\'",strip_tags($emapData[0]))));
						$col_2 = @(trim(str_replace("'","\'",strip_tags($emapData[1]))));
						$col_3 = @(trim(str_replace("'","\'",strip_tags($emapData[2])))); 
						$col_4 = @(trim(str_replace("'","\'",strip_tags($emapData[3]))));
						
						if($col_1 == $columns[3]){									
							$isReached = True;
							$section = $col_1;
							continue;
						}else{
							if(!$isReached) continue;
							if(!empty($col_2)&&!empty($col_3)){
								if($col_3 != "No Source"){
									if($col_2 != "Value" && $col_2 != "Unit"){
										//echo $col_1.' => '.((number_format((double)str_replace(',', '', @$col_2),3))).' => '.$col_3.' => '.$col_4.'<br/>';
										if(empty($col_2)||empty($col_3)){
											$error[] = "Check Line $count. Its should not be empty.";
											break;
										}
										$unit[] = strtolower(str_replace(' ', '_', @$col_3)).'_'.strtolower(str_replace(' ', '_', @$col_1));
										$value[] = ((((double)str_replace(',', '', @$col_2))));
									}
								}
							}
						}

						//break after another section has started.
						if($col_1 == $columns[4]){
							$sql = 'INSERT INTO r_rate ';
							$sql .= '( rate_'.implode(', rate_', $unit).', rate_reading_id, rate_cus_id)';
							$sql .= ' VALUES ';
							$sql .=  '(\''.implode('\',\'', $value).'\', \''.$rrr.'\', \''.$customer_id.'\')';
							
							$db = new Db();
							$insert = $db->query($sql);
							if($db->error()){
								$error[] = "Not inserted";
								echo $db->error();
							}							
							break;
						}
					}
				}				
							
			}else{
				$error[] = 'No Attachment Found';
			}

			if($error){
				FeedBack::errors($error);
			}else{				
				FeedBack::success();
				FeedBack::refresh();
			}
			
		}
		?>

			<div style="padding:10px;">
				<div class="pull-left">
					<p style="font-weight:bold;">Year</p>
					<select name="year" style="width:80px; margin-right:10px;">
						<option>Select</option>
						<?php 
						for($i=2010; $i<=date('Y'); $i++){
							if($i == date('Y') && empty($year_uploaded))
								echo '<option value="'.$i.'" selected="selected">'.($i).'</option>';
							elseif($i == $year_uploaded)
								echo '<option value="'.$i.'" selected="selected">'.($i).'</option>';
							else
								echo '<option value="'.$i.'">'.($i).'</option>';
						}
						?>
					</select>
				</div>
				<div class="pull-left">
					<p style="font-weight:bold;">Month</p>
					<select name="month" style="width:120px; margin-right:10px;">
						<option>Select</option>
						<?php 
						for($i=1; $i<=12; $i++){
							if($i == date('m') && empty($month_uploaded))
								echo '<option value="'.$i.'" selected="selected">'.month_name($i).'</option>';
							elseif($i == $month_uploaded)
								echo '<option value="'.$i.'" selected="selected">'.month_name($i).'</option>';
							else
								echo '<option value="'.$i.'">'.month_name($i).'</option>';
						}
						?>
					</select>
				</div>
				<div class="pull-left">
					<p style="font-weight:bold;">Customer Name</p>
					<select id="customer" onchange="return metering_points('<?php echo return_url(); ?>'); " name="customer_id" style="width:150px; margin-right:10px;">
						<option value="">Select</option>
						<?php 
						$ccid = $customer_id;
						$db = new Db();
						$select = $db->select("SELECT * FROM customer ORDER BY customer_short_name ASC ");
						foreach($select[0] as $row){
							extract($row);
							if($ccid == $customer_id)
								echo '<option value="'.$customer_id.'" selected="selected">'.$customer_short_name.'</option>';
							else
								echo '<option value="'.$customer_id.'">'.$customer_short_name.'</option>';
						}
						?>
					</select>
				</div>
				<div class="pull-left">
					<p style="font-weight:bold;">Metering Point <span id="meterPointNumber"></span></p>
					<select id="meterPoint" name="metering_point_id" style="width:150px; margin-right:10px;">
						<option value="">Select</option>
					</select>
				</div>				
				<div  class="pull-left">
					<p style="font-weight:bold;">Choose File (CSV)</p>
					<input type="file" name="file" id="file" style="width:200px"/>
				</div>
				<div  class="pull-left">
					<p>&nbsp;</p>
					<input type="submit" value="Save" name="importElester" style="width:100%; margin: auto 10px;"/>
				</div>
				<div class="clear"></div>
			</fieldset>
		</form>
		<br/>
		<h4>Ruling Rates:</h4>
		<table id="table" border="1">
			<tr>
				<th rowspan="2">No.</th>
				<th rowspan="2">Customer</th>
				<th colspan="3">Rates</th>
				<th colspan="2">Peroid</th>
				<th rowspan="2">Status</th>
			</tr>
			<tr>
				<th>Shoulder</th>
				<th>Peak</th>
				<th>Off Peak</th>
				<th>From</th>
				<th>To</th>
			</tr>

			<?php
				$select = $db->select("SELECT * FROM customer");
				$i = 1;
				foreach($select[0] as $row){
					extract($row);
					$month = date('m');
					$year = date('Y');
					$tou = $this->tou($customer_id, $month, $year);

					$s = $tou[4];

					if(date('Y-m-d') > date('Y-m-d', $s)){
						echo '<tr class="text-danger" >';
						//echo '<tr>';
						$status = "Expired";
					}else{
						$status = "Active";
						echo '<tr>';
					}

					
					echo '<td width="30px">'.($i++).'.</td>';
					echo '<td>'.$customer_short_name.'</td>';
					echo '<td>'.$tou[0].'</td>';
					echo '<td>'.$tou[1].'</td>';
					echo '<td>'.$tou[2].'</td>';
					echo '<td>'.FeedBack::date_s($tou[3]).'</td>';
					echo '<td>'.FeedBack::date_s($tou[4]).'</td>';
					echo '<td>'.$status.'</td>';
					echo '</tr>';
				}
			?>
		</table>

		<?php

		

		echo '</div>';
	}	

	public function downloadCsv(){
		

		
	}	
	
	public function AddSectionAction(){
		if(isset($_POST['submit'])){
		
			$section = $_POST['section_name'];
			$department_id = $_POST['department_id'];
			$time = time();
			$user = user_id();
			$db = new Db();
		
			$x = $db->insert("section",["section_date_added"=>$time,"section_dept_id"=>$department_id, "section_name"=>$section,"section_added_by"=>$user]);
			echo $db->error();
			if($x){
				FeedBack::success();
				FeedBack::refresh(3, return_url().'department/all-sections');
			}else{
				FeedBack::error();
			}
		}
		?>
		<div class="col-md-4">
			<h3>&nbsp;</h3>
			<div class="col-lg-12">
			<div class="panel panel-default">
				<div class="panel-heading">
					Add Section Form
				</div>
				<div class="panel-body">
					<div class="row">
						<div class="col-lg-12">
							<form role="form" action="" method="post">
								<div class="form-group">
									<label>Section Name</label>
									<div class="form-line">
										<input class="form-control" type="text" name="section_name" placeholder="Enter Department Name" value="<?php echo @$department; ?>">
									</div>
								</div>
								<div class="form-group">
								
									<label>Belongs To:</label>
									<div class="form-line">
										<select name="department_id">
											<option value="">--Select--</option>
											<?php
											$db = new Db();
											$select = $db->select("SELECT dept_id, dept_name FROM department ORDER BY dept_name ASC");
											foreach($select[0] as $row){
												extract($row);
												if($dept_id == $department_id)
													echo '<option value="'.$dept_id.'" selected="selected">'.$dept_name.'</option>';
												else
													echo '<option value="'.$dept_id.'">'.$dept_name.'</option>';
											}
											?>
										</select>
									</div>
								</div>
								
								<button type="submit" name="submit" class="btn btn-primary"><i class="fa fa-fw fa-save"></i> Save</button>
							</form>
						</div>
					</div>
				</div>
			</div>
		</div>
	<?php
	}
	
	public function AllSectionsAction(){
	
	?>
		<div class="col-md-12">
			<h3>Section List</h3>
			<?php 
			$db = new Db();
			$select = $db->select("SELECT * FROM department, section WHERE dept_id = section_dept_id ");
			
			if(!$select){
				$db->error();
			}else{
				echo '<table cellspacing="0" cellpadding="2" border="1" width="100%" id="table">';
				echo '<thead>';
				echo '<tr>';
				echo '<th width="30px">No.</th>';
				echo '<th>Section Name</th>';
				echo '<th>Department Name</th>';
				echo '<th>Total Staff</th>';
				echo '<th width="100px">Action</th>';
				echo '</tr>';
				echo '</thead>';
				
				$i=1;
				echo '<tbody>';
				foreach($select[0] as $row){
					extract($row);
					echo '<tr>';
					echo '<td><center>'.($i++).'.</center></td>';
					echo '<td>'.($section_name).'</td>';
					echo '<td>'.($dept_name).'</td>';
					echo '<td></td>';
					echo '<td>';
					echo $this->action('edit','department/edit-section/'.$section_id, 'Edit');
					echo $this->action('delete','department/delete-section/'.$section_id, 'Delete');
					echo '</td>';
					echo '</tr>';
				}
				echo '</tbody>';
				
				echo '</table>';
			}
			
			?>
		</div>
		<?php
	}
}
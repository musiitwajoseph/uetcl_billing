<?php
Class Services extends BeforeAndAfter{
	public $page = "OFFICE";
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
		return array($customer_id, date('Y'), date('m'));
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
				$tab .=  '<td><a href="'.return_url().'services/view-invoice/'.$customer_id.'/'.$year_uploaded.'/'.$month_uploaded.'" >'.$customer_short_name.'</a></td>';
				
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


	public function rateSum($customer_id, $year, $month){
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

		$tc1 = $tc2 = $tc3 = $tc4 = $tc5 = $tc6 = $tc7 = $tc8 = 0;

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
			
			if(date('Y', $rea_date)==$year && date('m', $rea_date)==$month){
				//selecting rates
				
				$db = new Db();
				$sel = $db->select("SELECT * FROM r_rate WHERE rate_reading_id = '$rea_id'");
				@extract($sel[0][0]);
				
				//calling the function
				
					$cal = rate_calculate($customer_id, ($rea_date), $mp_id);
				//echo '<pre>';
				//print_r($cal);
				//echo '</pre>';
				//echo "$count != $total_mp";
				//if($count != $total_mp)

				{
					if($customer_id == 2022){ //BECS

						$g1 = ($cal[0] - $cal[4]);
						$g2 = ($cal[1] - $cal[5]);
						$g3 = ($cal[2] - $cal[6]);

						$g4 = $g1 + $g2 + $g3;

						$tc1 += $g1;
						$tc2 += $g2;
						$tc3 += $g3;
						$tc4 += $g4;	

						$jm = $g1;	
				
					}elseif($mp_id == 5330 || $customer_id == 5328){ //BECS

						$g1 = ($cal[0] - $cal[4]);
						$g2 = ($cal[1] - $cal[5]);
						$g3 = ($cal[2] - $cal[6]);

						$g4 = $g1 + $g2 + $g3;

						$tc1 += 0;//+= $g1;
						$tc2 += 0;//+= $g2;
						$tc3 += 0;//+= $g3;
						$tc4 += 0;//+= $g4;	

						$jm = 0;	
				
					}elseif($customer_id == 1017){ //BECS

						$g1 = ($cal[0] - $cal[4]);
						$g2 = ($cal[1] - $cal[5]);
						$g3 = ($cal[2] - $cal[6]);

						$g4 = $g1 + $g2 + $g3;

						$tc1 += $g1;
						$tc2 += $g2;
						$tc3 += $g3;
						$tc4 += $g4;	

						$jm = $g1;	
				
					}elseif($customer_id == 2020 && $mp_id == 5331){ //UEDCL => ecopower
						
						$cal_2 = rate_calculate($customer_id, ($rea_date), 5278); //muhanga rugyeyo
						$cal_3 = rate_calculate($customer_id, ($rea_date), 5274); //nyakagyeme

						$g1 = ($cal[0] - $cal[4] - $cal_2[4] - $cal_3[4]);
						$g2 = ($cal[1] - $cal[5] - $cal_2[5] - $cal_3[5]);
						$g3 = ($cal[2] - $cal[6] - $cal_2[6] - $cal_3[6]);

						$g4 = $g1 + $g2 + $g3;

						$tc1 += $g1;
						$tc2 += $g2;
						$tc3 += $g3;
						$tc4 += $g4;

						$jm = $g1;

					}elseif($customer_id == 2020 && $mp_id == 9387){ //UEDCL => nkuusi
        
				        $cal_2 = rate_calculate($customer_id, ($rea_date), 5284); //munteme
				        $cal_3 = rate_calculate($customer_id, ($rea_date), 5275); //rugombe
				        $cal_4 = rate_calculate($customer_id, ($rea_date), 5276); //kakumiro

				        $g1 = ($cal[0] - $cal[4] - $cal_2[4] - $cal_3[4] - $cal_4[4]);
				        $g2 = ($cal[1] - $cal[5] - $cal_2[5] - $cal_3[5] - $cal_4[5]);
				        $g3 = ($cal[2] - $cal[6] - $cal_2[6] - $cal_3[6] - $cal_4[6]);

				        $g4 = $g1 + $g2 + $g3;

				        $tc1 += $g1;
				        $tc2 += $g2;
				        $tc3 += $g3;
				        $tc4 += $g4;

				        $jm = $g1;

				    }elseif($customer_id == 2020 && $mp_id == 5298){ //UEDCL => waki hydro max
						
						$cal_2 = rate_calculate($customer_id, ($rea_date), 5291); //burindi exports

						$g1 = ($cal[4] - $cal[0] - $cal_2[4] );
						$g2 = ($cal[5] - $cal[1] - $cal_2[5] );
						$g3 = ($cal[6] - $cal[2] - $cal_2[6] );

						$g4 = $g1 + $g2 + $g3;

						$tc1 += $g1;
						$tc2 += $g2;
						$tc3 += $g3;
						$tc4 += $g4;

						$jm = $g1;

					}elseif($customer_id == 2020 && $mp_id == 5299 ){ //UEDCL => kahungye kamwenge
						
						$cal_2 = rate_calculate(2020, ($rea_date), 5330); //UMEME Kamwenge

						$g1 = ($cal[4] - $cal_2[0] );
						$g2 = ($cal[5] - $cal_2[1] );
						$g3 = ($cal[6] - $cal_2[2] );

						$g4 = $g1 + $g2 + $g3;

						$tc1 += $g1;
						$tc2 += $g2;
						$tc3 += $g3;
						$tc4 += $g4;

						$jm = $g1;

					}elseif($customer_id == 2020 && $mp_id == 5326){ //UEDCL => kamwenge
						
						$tc1 += $cal[0];
						$tc2 += $cal[1];
						$tc3 += $cal[2];
						$tc4 += $cal[3];

						$jm =  $cal[0];

					}elseif($customer_id == 2020 && $mp_id == '5327'){ //UEDCL => siti 1 5327

        $cal_2 = rate_calculate(2020, ($rea_date), 5267); //kweene exports   

        $g1 = ($cal[0] - $cal[4]);
        $g2 = ($cal[1] - $cal[5]);
        $g3 = ($cal[2] - $cal[6]);
		
		
		/////////////////////////////////////////////////////////////////////////
		
		$cal_2a = rate_calculate(2020, ($rea_date), 5267);
        $cala = rate_calculate(2020, ($rea_date), 5328); //kweene exports   

        $g1a = ($cala[0] - $cala[4] - $cal_2a[4]);
        $g2a = ($cala[1] - $cala[5] - $cal_2a[5]);
        $g3a = ($cala[2] - $cala[6] - $cal_2a[6]);
		
		/////////////////////////////////////////////////////////////////////////

        $g4 = $g1 + $g2 + $g3 + $g1a + $g2a + $g3a;
        $tc1 += $g1 + $g1a;
        $tc2 += $g2 + $g2a;
        $tc3 += $g3 + $g3a;
        $tc4 += $g4 + $g4a;

        $jm =  ($g1+$g1a);

    }elseif($customer_id == 2020 && $mp_id == '5328'){ //UEDCL => siti 2 5328
				        $cal_2 = rate_calculate(2020, ($rea_date), 5267); //kweene exports   

				        $g1 = ($cal[0] - $cal[4] - $cal_2[4]);
				        $g2 = ($cal[1] - $cal[5] - $cal_2[5]);
				        $g3 = ($cal[2] - $cal[6] - $cal_2[6]);

				        $g4 = 0; //$g1 + $g2 + $g3;
				        $tc1 += 0;//$g1;
				        $tc2 += 0;//$g2;
				        $tc3 += 0;//$g3;
				        $tc4 += 0;//$g4;

				        $jm =  0;

				    }elseif($customer_id == 2022 && $mp_id == 5315){ //KRECs => kiseruka

						$g1 = ($cal[4]-$cal[0]);
						$g2 = ($cal[5]-$cal[1]);
						$g3 = ($cal[6]-$cal[2]);

						$g4 = $g1 + $g2 + $g3;

						$tc1 += $g1;
						$tc2 += $g2;
						$tc3 += $g3;
						$tc4 += $g4;

						$jm = $g1;

					}elseif($customer_id == 2021 && $mp_id == '9383'){ //KIL => Kyambura shp ziba
						//echo $mp_location.'==='.$cal[0].' ==== '.$cal[4];
				        $g1 = ($cal[0] - $cal[4]);
				        $g2 = ($cal[1] - $cal[5]);
				        $g3 = ($cal[2] - $cal[6]);

				        $g4 = $g1 + $g2 + $g3;
				        $tc1 += $g1;
				        $tc2 += $g2;
				        $tc3 += $g3;
				        $tc4 += $g4;

				        $jm = $g1;

				    }elseif($customer_id == 2021 && $mp_id == '5302'){ //KIL => Kyambura shp ziba
						//echo $mp_location.'==='.$cal[0].' ==== '.$cal[4];
				        $g1 = ($cal[0] - $cal[4]);
				        $g2 = ($cal[1] - $cal[5]);
				        $g3 = ($cal[2] - $cal[6]);

				        $g4 = $g1 + $g2 + $g3;
				        $tc1 += $g1;
				        $tc2 += $g2;
				        $tc3 += $g3;
				        $tc4 += $g4;

				        $jm =  $g1;

				    }elseif($customer_id == 2021 && $mp_id == 5310){ //KIL
												
						$cal_2 = rate_calculate($customer_id, ($rea_date), 5303); //Kikorongo
								$cal_3 = rate_calculate(2017, ($rea_date), 4009); //SNEL -> Mpondwe
								
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
						$jm =  $g0;
					}else{
					
						if($mp_region_id == 8 ){
						
						$tc1 += $cal[0]*-1;
						$tc2 += $cal[1]*-1;
						$tc3 += $cal[2]*-1;
						$tc4 += $cal[3]*-1;
						//echo $mp_location.'>>>>'.$cal[0].'<br/>';
						}else{
						
						$tc1 += $cal[0];
						$tc2 += $cal[1];
						$tc3 += $cal[2];
						$tc4 += $cal[3];
						//echo $mp_location.'>>>>'.$cal[0].'<br/>';
						}

						$jm = $cal[0];
					}
					
					if($mp_region_id == 8){
					$tc5 += $cal[4]*-1;
					$tc6 += $cal[5]*-1;
					$tc7 += $cal[6]*-1;
					$tc8 += $cal[7]*-1;
					}else{
					$tc5 += $cal[4];
					$tc6 += $cal[5];
					$tc7 += $cal[6];
					$tc8 += $cal[7];
					}

				}

				//echo $mp_location;
				//echo ' = ';
				//analysis
				//echo ($jm/1000);
				//echo ' + ';
				//echo $tc1;
				//echo '<br/>';
			}


		}

		//echo $tc4.'<=<br/>';

		if($customer_id == 2020){ //units from KRECS exports
			$t = rate_calculate(2022, ($rea_date), 5315);

			$a1 = $t[0];
			$a2 = $t[1];
			$a3 = $t[2];
			$a4 = $t[3];

			$tc1 -= $a1;
			$tc2 -= $a2;
			$tc3 -= $a3;
			$tc4 -= $a4;

		}

		$sum = array($tc1, $tc2, $tc3, $tc4, $tc5, $tc6, $tc7, $tc8);

		return $sum;

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

					

					$cal = rate_calculate2($customer_id, ($rea_date), $mp_id);


					//echo '<pre>';
					//print_r($cal);
					//echo '</pre>';
					//echo "$count != $total_mp";
					//if($count != $total_mp)

					{
						if($customer_id == 1017){ //BECS
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
								$tc1 += $cal[0];// * $mp_wheeling_charge/100;
								$tc2 += $cal[1];// * $mp_wheeling_charge/100;
								$tc3 += $cal[2];// * $mp_wheeling_charge/100;
								$tc4 += $cal[3];// * $mp_wheeling_charge/100;
							}else{
								$tc1 += $cal[0] * $mp_wheeling_charge/100;
								$tc2 += $cal[1] * $mp_wheeling_charge/100;
								$tc3 += $cal[2] * $mp_wheeling_charge/100;
								$tc4 += $cal[3] * $mp_wheeling_charge/100;
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

	public function energyLinks($id, $year, $month){
		echo '<div class="nav files pull-left">';
		$t=0;

		if($id == 2030)
			$wcs = 'Bill';
		else 
			$wcs = 'Wheeling Charge Schedule';


		if(!empty(portion(6))){

			if($this->isDNorCN($id, $year, $month) > 0){
				$word = "DN ";
				$word_full = "Dedit Note";
				$t=1;
			}else{
				$word = "CN ";
				$word_full = "Credit Note";
				$t=1;
			}

			$note = portion(6);

			if(portion(2) == "energy-customer-details" && $t){

			}elseif(portion(2) != "readings" && $t){ 
				echo '<a title="'.$word_full.' Readings" style="color:black;" accesskey="e" href="'.return_url().'services/readings/'.$id.'/'.$year.'/'.$month.'/'.$note.'"><i class="fa fa-edit fa-fw"></i> '.$word.' Readings</a>';
			} else { 
				echo '<span title="'.$word_full.' Readings" style="color:red; font-weight: bold;"><i class="fa fa-edit fa-fw"></i> '.$word.'  Readings</span>';
			}
			echo '&nbsp; &nbsp;'; 

			if(portion(2) != "schedule" && $t){ 		
				echo '<a title="'.$word_full.' Schedule" style="color:black;" accesskey="s" href="'.return_url().'services/schedule/'.$id.'/'.$year.'/'.$month.'/'.$note.'"><i class="fa fa-list fa-fw"></i> '.$word.' Schedule</a>'; 	
			}else{	
				echo '<span title="'.$word_full.' Schedule" style="color:red; font-weight: bold;"><i class="fa fa-list fa-fw"></i> '.$word.' Schedule</span>'; 
			}

			echo ' &nbsp; &nbsp;'; 

			if(portion(2) != "wheeling-charge-schedule" && $t){ 
				echo '<a title="'.$word_full.' Wheeling Charge Schedule" style="color:black;" accesskey="w" href="'.return_url().'services/wheeling-charge-schedule/'.$id.'/'.$year.'/'.$month.'/'.$note.'"><i class="fa fa-list fa-fw"></i> '.$word.' '.$wcs.'</a>';
			}else{	
				echo '<span title="'.$word_full.' Wheeling Charge Schedule" style="color:red; font-weight: bold;"><i class="fa fa-list fa-fw"></i> '.$word.' '.$wcs.'</span>';
			}

			if(portion(3) == 2019){
				echo ' &nbsp; &nbsp;'; 
				if(portion(2) != "backflow-schedule" && $t){ 
					echo '<a title="'.$word_full.' Backflows" style="color:black;" accesskey="b" href="'.return_url().'services/backflow-schedule/'.$id.'/'.$year.'/'.$month.'/'.$note.'"><i class="fa fa-list fa-fw"></i> '.$word.' Backflows</a>';
				}else{	
					echo '<span title="'.$word_full.' Backflows" style="color:red; font-weight: bold;"><i class="fa fa-list fa-fw"></i> '.$word.' Backflows</span>';
				}
			}

			if(portion(3) == 2029){//BILL AND AMOUNT FOR TANESCO
				echo ' &nbsp; &nbsp;'; 
				if(portion(2) != "generator-bill-and-amount" && $t){ 
					echo '<a title="'.$word_full.' Generator Bill and Amount" style="color:black;" accesskey="b" href="'.return_url().'services/generator-bill-and-amount/'.$id.'/'.$year.'/'.$month.'/'.$note.'"><i class="fa fa-list fa-fw"></i> '.$word.' Generator Bill</a>';
				}else{	
					echo '<span title="'.$word_full.' Generator Bill and Amount" style="color:red; font-weight: bold;"><i class="fa fa-list fa-fw"></i> '.$word.' Generator Bill</span>';
				}
			}

			echo ' &nbsp; &nbsp; ';

			if(portion(2) != "view-invoice" && $t){ 
				echo '<a title="'.$word_full.' Invoice" style="color:black;" accesskey="i" href="'.return_url().'services/view-invoice/'.$id.'/'.$year.'/'.$month.'/'.$note.'"><i class="fa fa-file fa-fw"></i> '.$word.' Invoice</a>';
			}else{	
				echo '<span title="'.$word_full.' Invoice" style="color:red; font-weight: bold;"><i class="fa fa-file fa-fw"></i> '.$word.' Invoice</span>';
			}

			echo '  &nbsp; &nbsp;';

			// if(portion(2) == "energy-customer-details"){

			// }else{
			// 	echo '<a href="'.return_url().'customers/energy-customer-details/'.$id.'/'.$year.'/'.$month.'/'.$note.'"><i class="fa fa-eye fa-fw"></i> Customer Details</a> &nbsp; &nbsp;'; 
			// }

			// echo '  &nbsp; &nbsp;'; 

			if(portion(2)=="readings"){

			}elseif(portion(2)=="view-invoice"||portion(2)=="wheeling-charge-schedule"){
				$d = new Db();
				$sel = $d->select("SELECT * FROM done_levels WHERE dl_app1 = 1 AND dl_app2 = 1 AND dl_app3 = 1 AND dl_customer = '$id' AND dl_year = '$year' AND dl_month = '$month' ");
				//if($d->num_rows()){
				if(1){
					echo '<a class="" href="" target="" onclick="return print(\'printMe\');"><i class="fa fa-print fa-fw"></i>  '.$word.' Print</a>';
				}

			}elseif(portion(6)=="credit-note" && portion(2) != "schedule"){
				//echo '<div class="clearfix"></div>';
				echo '<br/><br/>';
			}elseif(portion(2) != "energy-customer-details" 
				&& portion(2) != "readings"  
				//&& portion(2) != "debit-note"
				//&& portion(2) != "credit-note"  
				&& portion(2) != "delete-month-readings" 
				&& portion(2) != "new-month-readings" 
				&& portion(2) != "sales-trend"){ //sales-trend
				//echo '<a class="" href="" target="" onclick="return print(\'printMe\');"><i class="fa fa-print fa-fw"></i>  '.$word.' Print</a>';

				$d = new Db();
				$sel = $d->select("SELECT * FROM done_levels WHERE dl_app1 = 1 AND dl_app2 = 1 AND dl_app3 = 1 AND dl_customer = '$id' AND dl_year = '$year' AND dl_month = '$month' ");
				//if($d->num_rows()){
				if(1){
					echo '<a class="" href="" target="" onclick="return print(\'printMe\');"><i class="fa fa-print fa-fw"></i>  '.$word.' Print</a>';
				}
			}else{
				echo '<br/><br/>';
			}
			if(portion(2)!="readings"){
				echo '<div class="clearfix"></div>';
			}
		}else{
			$note = portion(6);

			if(portion(2) == "energy-customer-details"){

			}elseif(portion(2) != "readings"){ 
				echo '<a accesskey="e" href="'.return_url().'services/readings/'.$id.'/'.$year.'/'.$month.'/'.$note.'"><i class="fa fa-edit fa-fw"></i> Readings</a>';
			} else { 
				echo '<span style="color:red; font-weight: bold;"><i class="fa fa-edit fa-fw"></i> Readings</span>';
			}
			echo '&nbsp; &nbsp;'; 


			if(portion(3) == 2029){//BILL AND AMOUNT FOR TANESCO
				echo ' &nbsp; &nbsp;'; 
				if(portion(2) != "generator-bill-and-amount"){ 
					echo '<a title="'.$word_full.' Generator Bill and Amount" style="color:black;" accesskey="b" href="'.return_url().'services/generator-bill-and-amount/'.$id.'/'.$year.'/'.$month.'/'.$note.'"><i class="fa fa-list fa-fw"></i> '.$word.' Generator Bill</a>';
				}else{	
					echo '<span title="'.$word_full.' Generator Bill and Amount" style="color:red; font-weight: bold;"><i class="fa fa-list fa-fw"></i> '.$word.' Generator Bill</span>';
				}
			}

			echo ' &nbsp; &nbsp;'; 

			if(portion(2) != "schedule"){ 		
				echo '<a accesskey="s" href="'.return_url().'services/schedule/'.$id.'/'.$year.'/'.$month.'/'.$note.'"><i class="fa fa-list fa-fw"></i>Schedule</a>'; 	
			}else{	
				echo '<span style="color:red; font-weight: bold;"><i class="fa fa-list fa-fw"></i>Schedule</span>'; 
			}

			echo ' &nbsp; &nbsp;'; 


			if(portion(2) != "wheeling-charge-schedule"){ 
				echo '<a accesskey="w" href="'.return_url().'services/wheeling-charge-schedule/'.$id.'/'.$year.'/'.$month.'/'.$note.'"><i class="fa fa-list fa-fw"></i> '.$wcs.'</a>';
			}else{	
				echo '<span style="color:red; font-weight: bold;"><i class="fa fa-list fa-fw"></i> '.$wcs.'</span>';
			}

			if(portion(3) == 2019){
				echo ' &nbsp; &nbsp;'; 
				if(portion(2) != "backflow-schedule"){ 
					echo '<a accesskey="b" href="'.return_url().'services/backflow-schedule/'.$id.'/'.$year.'/'.$month.'/'.$note.'"><i class="fa fa-list fa-fw"></i>Backflows</a>';
				}else{	
					echo '<span style="color:red; font-weight: bold;"><i class="fa fa-list fa-fw"></i>Backflows</span>';
				}
			}

			echo ' &nbsp; &nbsp; ';

			if(portion(2) != "view-invoice"){ 
				echo '<a accesskey="i" href="'.return_url().'services/view-invoice/'.$id.'/'.$year.'/'.$month.'/'.$note.'"><i class="fa fa-file fa-fw"></i>Invoice</a>';
			}else{	
				echo '<span style="color:red; font-weight: bold;"><i class="fa fa-file fa-fw"></i> Invoice</span>';
			}

			echo '  &nbsp; &nbsp;'; 

			if(portion(2) == "energy-customer-details" && $t){

			}else{
				echo '<a href="'.return_url().'customers/energy-customer-details/'.$id.'/'.$year.'/'.$month.'/'.$note.'"><i class="fa fa-list fa-fw"></i> Customer Details</a> &nbsp; &nbsp;'; 
			}

			echo '  &nbsp; &nbsp;'; 

			if(portion(2) != "energy-customer-details" 
				&& portion(2) != "readings"  
				&& portion(2) != "debit-note"
				//&& portion(2) != "credit-note"  
				&& portion(2) != "delete-month-readings" 
				&& portion(2) != "new-month-readings" 
				&& portion(2) != "sales-trend"){ //sales-trend
				

				$d = new Db();
				$sql = "SELECT * FROM done_levels WHERE dl_app1 = 1 AND dl_app2 = 1 AND dl_app3 = 1 AND dl_customer = '$id' AND dl_year = '$year' AND dl_month = '$month' ";
				$sel = $d->select($sql);
				//if($d->num_rows()){
				if(1){
					echo '<span class="" style="background-color:#000; color:#fff; cursor:pointer; padding:2px 5px; border-radius:5px;" onclick="return print(\'printMe\');"><i class="fa fa-print fa-fw"></i> Print </span>';
				}
			}else{
				//echo '<br/><br/>';
			}
		}

		// if(portion(2) == "readings"){
		// 	//echo '<div style="margin-top:10px;">';
		// 	echo '<a href="'.return_url().'services/new-month-readings/'.$id.'/'.$year.'/'.$month.'/'.$note.'" class=""><i class="fa fa-fx fa-plus"></i> New</a> &nbsp;  &nbsp; ';
		// 	//New Month Readings

		// 	echo '<a href="'.return_url().'services/delete-month-readings/'.$id.'/'.$year.'/'.$month.'/'.$note.'" class="" ><i class="fa fa-fx fa-times"></i> Delete</a> &nbsp; &nbsp; ';
		// 	//Delete All Month Readings

		// 	echo '<a href="'.return_url().'services/debit-note/'.$id.'/'.$year.'/'.$month.'/'.$note.'" class="" ><i class="fa fa-fx fa-list"></i> Debit Note</a> &nbsp; &nbsp; ';
		// 	//Delete All Month Readings

		// 	echo '<a href="'.return_url().'services/credit-note/'.$id.'/'.$year.'/'.$month.'/'.$note.'" class="primary" > <i class="fa fa-fx fa-list"></i> Credit Note </a> &nbsp;  &nbsp; ';
		// 	//Delete All Month Readings
		// }

		echo '</div>';
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
				$select = $db->select("SELECT rea_id FROM r_reading WHERE rea_date = '$time_readings' AND rea_cus_id = '$customer'");
				
				if($db->num_rows()){
					$errors[] = "Readings already exit for  &nbsp;  &nbsp;  &nbsp; <b>Customer: </b>".$this->rgf("customer", $customer, "customer_id", "customer_short_name")." &nbsp;  &nbsp;  &nbsp; <b>Year: </b>".$filter_year."  &nbsp;  &nbsp;  &nbsp;  <b>Month: </b>".$filter_month;
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
									"n_rate_cus_id"=>$customer
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
				$select = $db->select("SELECT rea_id FROM r_reading WHERE rea_date = '$time_readings' AND rea_cus_id = '$customer'");
				
				if($db->num_rows()){
					$errors[] = "Readings already exit for  &nbsp;  &nbsp;  &nbsp; <b>Customer: </b>".$this->rgf("customer", $customer, "customer_id", "customer_short_name")." &nbsp;  &nbsp;  &nbsp; <b>Year: </b>".$filter_year."  &nbsp;  &nbsp;  &nbsp;  <b>Month: </b>".$filter_month;
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
				{
					
					$imports[] = (($cal[0]*$shoulder+$cal[1]*$peak+$cal[2]*$off_peak)/1000*$mp_wheeling_charge/100); //imports
					$exports[] = (($cal[4]*$shoulder+$cal[5]*$peak+$cal[6]*$off_peak)/1000*$mp_wheeling_charge/100); //exports

				}
			}
		}

		$sum = array($imports, $exports);

		return $sum;

	}

	public function customerAndSchedule($id, $year, $month){
		if(portion(2)=="energy-customer-details"){
			$p20 = $p21 = $p22 = "schedule";
		}else{
			$p20 = $p21 = $p22 = portion(2);
		}
		$customer_id = $id;
		$note = portion(6);

		echo '<div class="clearfix"></div>';
		$db = new Db();
		echo '<span class="" style="margin:2px 0; color:blue; font-weight: bold;">Customers > </span> &nbsp; ';	
		$select = $db->select("SELECT customer_id as c_id, customer_short_name as c_name FROM customer ORDER BY customer_short_name ASC");
		foreach($select[0] as $row){

			extract($row);
			if(portion(2)=="energy-customer-details"){
				if(portion(3) == $c_id){
					if($c_id == $id){
						echo '<span class="btn btn-xs btn-danger">'.$c_name.'</span>&nbsp;';
					}else{
						echo '<a href="'.return_url().'services/'.$p20.'/'.$c_id.'/'.$year.'/'.$month.'/'.$note.'" class="btn-xs btn-primary">'.$c_name.'</a>&nbsp;';
					}
				}
			}else{
				if($c_id == $id){
					echo '<span class="btn-xs btn-danger">'.$c_name.'</span>&nbsp;';
				}else{
					echo '<a href="'.return_url().'services/'.$p20.'/'.$c_id.'/'.$year.'/'.$month.'/'.$note.'" class="btn-xs btn-primary">'.$c_name.'</a>&nbsp;';
				}
			}
			
		}

		echo '<div class="clearfix"></div>';		
		echo '<div style="margin:5px;"></div>';
		echo '<div class="pull-right1">';
		$db = new Db();
		$select = $db->select("SELECT * FROM r_reading ORDER BY rea_date ASC");
				
		$rea_years = array();	

		echo '<span class="" style="color:red; font-weight: bold;">'.$this->rgf("customer", $customer_id, "customer_id", "customer_short_name").' Year> </span> &nbsp; ';		
		foreach($select[0] as $row){
			extract($row);

			if(!in_array(date('Y', $rea_date), $rea_years)){
				$rea_years[] = date('Y', $rea_date);
				if($year == date('Y', $rea_date)){
					echo '<span class="btn-danger btn-xs">'.date('Y', $rea_date).'</span>&nbsp;';
				}else{
					echo '<a href="'.return_url().'services/'.$p21.'/'.$id.'/'.date('Y', $rea_date).'/'.$month.'/'.$note.'" class="btn-xs" style="color:black; background-color: #ffbfbf;">'.date('Y', $rea_date).'</a>&nbsp;';
				}
			}
		}

		if(portion(2) == 'sales-trend'){
			echo ' &nbsp;<span class="btn-danger btn-xs">Sales Trend '.$year.'</span>&nbsp;';
		}else{
			echo '  &nbsp;<a href="'.return_url().'services/sales-trend/'.$id.'/'.$year.'/'.$month.'/'.$note.'" class="btn-xs" style="color:black; background-color: #ffbfff;">Sales Trend '.$year.'</a>&nbsp;';
		}		

		echo '</div>';		
		echo '<div style="margin:5px;"></div>';
		$db = new Db();
		$select = $db->select("SELECT * FROM metering_point, r_reading WHERE mp_customer_id = '$customer_id' AND rea_cus_id = '$customer_id' AND mp_id = rea_mp_id ORDER BY rea_date ASC");
				
		$rea_dates = array();	
		echo '<div class="pull-left1">';
		echo '<span class="" style="color:red; font-weight: bold;">'.$this->rgf("customer", $customer_id, "customer_id", "customer_short_name").' Month> </span> &nbsp; ';	

		//echo '<a href="'.return_url().'services/'.$p22.'/'.$id.'/'.($year-1).'/12/" class="btn-xs" style="color:black; background-color: #ccc;">'.month_name_short(12).' '.($year-1).'</a>&nbsp;';

		foreach($select[0] as $row){
			extract($row);

			if(!in_array($rea_date, $rea_dates) && $year == date('Y', $rea_date)){
				$rea_dates[] = $rea_date;


				$invoice_no = str_pad($rea_id, 5, '0', STR_PAD_LEFT);
				if($year == date('Y', $rea_date) && $month == date('m', $rea_date)  && empty(portion(6))){
					echo '<span class="btn-danger btn-xs btn-menu">';
					echo date('M Y', $rea_date);

					echo '<div class=" nav files month-inner-menu">';
					echo '<ul class="">';

					echo '<li>';
					echo '<a href="'.return_url().'services/new-month-readings/'.$id.'/'.$year.'/'.$month.'/'.$note.'" class=""> New Readings</a>';
					echo '</li>';

					echo '<li>';

					if(!$this->isLocked($id, $year, $month)){
						$e = "View / Edit ";
					}else{
						$e = "View ";
					}				
					echo '<a href="'.return_url().'services/readings/'.$id.'/'.$year.'/'.$month.'/'.$note.'" class=""> '.$e.' Readings</a>';
					echo '</li>';
					
					if(!$this->isLocked($id, $year, $month)){
						echo '<li>';
						echo '<a href="'.return_url().'services/delete-month-readings/'.$id.'/'.$year.'/'.$month.'/'.$note.'" class="" > Delete Readings</a>';
						echo '</li>';
					}
					
					echo '<li>';
					echo '<a href="'.return_url().'services/view-invoice/'.$id.'/'.$year.'/'.$month.'/'.$note.'" class="primary" >  Invoice </a>';
					echo '</li>';
					
					echo '<li>';

					if(!$this->isLocked($id, $year, $month)){
						echo '<a href="'.return_url().'services/credit-note/'.$id.'/'.$year.'/'.$month.'/'.$note.'" class="primary" >  Debit / Credit Note </a>';
						echo '</li>';
					}

					echo '</ul>';
					echo '</div>';

					echo '</span>&nbsp;';
				}else{
					echo '<a href="'.return_url().'services/'.$p22.'/'.$id.'/'.date('Y', $rea_date).'/'.date('m', $rea_date).'/" class="btn-xs" style="color:black; background-color: #ffbfbf;">'.date('M Y', $rea_date).'</a>&nbsp;';
				}
				//echo "SELECT * FROM metering_point, r_reading_cn WHERE mp_customer_id = '$customer_id' AND n_rea_cus_id = '$customer_id' AND mp_id = n_rea_mp_id AND n_rea_date = '$rea_date'".'>>>>';
				$t = new Db();
				$tt = $t->select("SELECT * FROM metering_point, r_reading_cn WHERE mp_customer_id = '$customer_id' AND n_rea_cus_id = '$customer_id' AND mp_id = n_rea_mp_id");
				$n_rea_dates = array();
				foreach($tt[0] as $ttt){
					extract($ttt);

					if(!in_array($n_rea_date, $n_rea_dates) && $year == date('Y', $n_rea_date) && date('m', $rea_date) == date('m', $n_rea_date) )
					{
						$n_rea_dates[] = $n_rea_date;
						if($year == date('Y', $n_rea_date) && $month == date('m', $n_rea_date) && portion(6)=="credit-note"){
							$iDC = ($this->isDNorCN($id, $year, $month) > 0)? "DN":"CN";
							echo '<span class="btn-danger btn-xs btn-menu">';

							echo $iDC.' '.date('M Y', $n_rea_date);

							echo '<div class="month-inner-menu">';
							echo '<ul>';

							echo '<li>';
							echo '<a href="'.return_url().'services/view-invoice/'.$id.'/'.date('Y', $n_rea_date).'/'.date('m', $n_rea_date).'/credit-note" class="primary" > View '.$iDC.' Note Invoice </a>';
							echo '</li>';							
							
							echo '<li>';

							if(!$this->isLocked($id, $year, $month)){
								$e = "View / Edit ";
							}else{
								$e = "View ";
							}
							echo '<a href="'.return_url().'services/readings/'.$id.'/'.$year.'/'.$month.'/'.$note.'" class="primary" > '.$e.' '.$iDC.' Note Readings </a>';
							echo '</li>';

							echo '<li>';

							if(!$this->isLocked($id, $year, $month)){
								echo '<a href="'.return_url().'services/delete-credit-note-month-readings/'.$id.'/'.date('Y', $n_rea_date).'/'.date('m', $n_rea_date).'/credit-note" class="primary" > Delete '.$iDC.' Note </a>';
								echo '</li>';
							}
							
							echo '</ul>';
							echo '</div>';

							echo '</span>&nbsp;';
						}else{
							$iDC = ($this->isDNorCN($id, date('Y', $n_rea_date), date('m', $n_rea_date)) > 0)? "DN":"CN";
							echo '<a href="'.return_url().'services/'.$p22.'/'.$id.'/'.date('Y', $n_rea_date).'/'.date('m', $n_rea_date).'/credit-note" class="btn-xs" style="color:black; background-color: #ffbfbf;">'.$iDC.' '.date('M Y', $n_rea_date);


							echo '</a>&nbsp;';
						}
					}
				}
			}
		}	

		//echo '<a href="'.return_url().'services/'.$p22.'/'.$id.'/'.($year+1).'/1/" class="btn-xs" style="color:black; background-color: #ccc;">'.month_name_short(1).' '.($year+1).'</a>&nbsp;';

		if(portion(2) == 'loss'){
			echo '&nbsp;<span class="btn-danger btn-xs">Energy Variance '.month_name_short($month).' '.$year.'</span>&nbsp;';
		}else{
			echo '&nbsp;<a href="'.return_url().'services/loss/'.$id.'/'.$year.'/'.$month.'/" class="btn-xs" style="color:black; background-color: #ffbfff;">Energy Variance '.month_name_short($month).' '.$year.'</a>&nbsp;';
		}	

		echo '</div>';

		echo '<div class="clearfix"></div>';

		echo '<div style="margin:5px;"></div>';

		if(portion(2) != "schedule" && portion(6) != "credit-note"){

		}elseif(portion(2) != "energy-customer-details" 
			&& portion(2) != "readings"  
			&& portion(2) != "debit-note"  
			&& portion(2) != "delete-month-readings" 
			&& portion(2) != "new-month-readings" 
			&& portion(2) != "sales-trend"){ 

			echo '<div class="pull-right" style=" margin-bottom: 20px;">';
			$this->rgf("customer", $id, "customer_id", "customer_default_units");
			echo '<form action="" method="post">';
			if(isset($_POST['change'])){
				$units = $_POST['rate'];
			}else{
				$units = 0.000001;
			}
			echo 'Change Units: ';
			echo '<select name="rate">';

			if($units == "1")
				echo '<option selected="selected" value="1">Wh</option>';
			else
				echo '<option value="1">Wh</option>';

			if($units == "0.001")
				echo '<option selected="selected" value="0.001">KWh</option>';
			else
				echo '<option value="0.001">KWh</option>';

			if($units == "0.000001")
				echo '<option selected="selected" value="0.000001">MWh</option>';
			else
				echo '<option value="0.000001">MWh</option>';

			echo '</select>';
			echo ' &nbsp; <button name="change">Change</button>';
			echo '</form>';

			echo '</div>';

		}

		return $units;
	}

	public function totalBackflow($id, $year, $month){

		$tc1 = 0;
	 	$tc2 = 0;
		$tc3 = 0;
		$tc4 = 0;
		$tc5 = 0;
		$tc6 = 0;
		$tc7 = 0;
		$tc8 = 0;
					
		if($id == 2019){	
			$l = $this->rateSum($id, $year, $month);

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
		return array($tc5, $tc6,  $tc7,  $tc8,  $tc6, ($tc5*$shoulder+$tc6*$peak+$tc7*$off_peak));

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
		$sql = "SELECT * FROM r_reading, r_rate, metering_point WHERE rea_id < '$rea_id' AND rea_id = rate_reading_id AND rea_cus_id = '2030' AND rea_mp_id = mp_id AND rate_cus_id = '2030' AND upper(mp_location) = 'MAIN 1' ORDER BY rea_date DESC";
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
		$sql = "SELECT * FROM r_reading, r_rate, metering_point WHERE rea_id <  '$rea_id' AND rea_id = rate_reading_id AND rea_cus_id = '2030' AND rea_mp_id = mp_id AND rate_cus_id = '2030' AND upper(mp_location) = 'MAIN 2' ORDER BY rea_date DESC";
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
		$sql = "SELECT * FROM r_reading, r_rate, metering_point WHERE rea_id <  '$rea_id' AND rea_id = rate_reading_id AND rea_cus_id = '2030' AND rea_mp_id = mp_id AND rate_cus_id = '2030' AND upper(mp_location) = 'CHECK 1' ORDER BY rea_date DESC";
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
		$sql = "SELECT * FROM r_reading, r_rate, metering_point WHERE rea_id <  '$rea_id' AND rea_id = rate_reading_id AND rea_cus_id = '2030' AND rea_mp_id = mp_id AND rate_cus_id = '2030' AND upper(mp_location) = 'CHECK 2' ORDER BY rea_date DESC";
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

	public function wheelingChargeScheduleAction(){

		
		$id = portion(3);
		$year = portion(4);
		$month = portion(5);
		$note = portion(6);

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
				$this->wheelingChargeScheduleCreditNote($id, $year, $month);
			}else{
				$this->wheelingChargeScheduleNormal($id, $year, $month);
			}
		}
	}

	public function wheelingChargeScheduleNormal($id, $year, $month){

		$db = new Db();
		$select = $db->select("SELECT * FROM customer WHERE customer_id = '$id'");
		extract($select[0][0]);

		// echo '<pre>';
		// print_r($this->totalBackflow($id, $year, $month));
		// echo '</pre>';
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
				echo '<title>'.strtoupper($customer_short_name.' WHEELING CHARGE').' - '.($month = portion(5)).' '.portion(4).'</title>';

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
			if($customer_id == 2030){
				$g = "<center><h4>";
				$g .= $customer_short_name;
				$g .= ' BILL FOR THE MONTH OF ';
				$g .= month_name($month = portion(5));
				$g .= ' ';
				$g .= $year = portion(4);

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
					$g .= ' WHEELING CHARGE ';
					$g .= $year = portion(4);
					$g .= ' - ';
					$g .= month_name($month = portion(5));
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
					<th colspan="3" style="border-top:2px solid black; border-right:2px solid black;border-left:2px solid black;">Wheeling Charge</th>			
				</tr>
				<tr valign="bottom">
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
					?>
					<?php $symbol = $this->rgf("currency",$customer_currency, "currency_id", "currency_symbol"); ?>
					<th style="border-left:2px solid black;border-bottom:2px solid black;">Shoulder<br/><span style="color:red;">@ <?php echo $shoulder.'<br/>';?></span><?php echo $symbol;?>/kWh</th>
					<th style="border-bottom:2px solid black;">Peak<br/><span style="color:red;">@ <?php echo $peak.'<br/>';?></span><?php echo $symbol;?>/kWh</th>
					<th style="border-bottom:2px solid black;">Off Peak<br/><span style="color:red;">@ <?php echo $off_peak.'<br/>';?></span><?php echo $symbol;?>/kWh</th>
					<th style="border-bottom:2px solid black;">Advance<br/>(kWh)</th>
					<th style="border-right:2px solid black;border-bottom:2px solid black;">Amount<br/>(<?php echo $symbol;?>)</th>

					<th style="border-left:2px solid black; border-bottom: 2px solid black;">#</th>
					<th style="border-bottom:2px solid black;">Imports<br/>(Payable)<br/><b>(<?php echo $symbol;?>)</b></th>
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
				$sel = $t->select("SELECT DISTINCT mp_region_id FROM metering_point, r_reading WHERE mp_customer_id = '$customer_id' AND rea_cus_id = '$customer_id' AND mp_id = rea_mp_id AND mp_region_id IS NOT NULL");

				foreach($sel[0] as $r){
				extract($r);
			
				if(!$t->num_rows()){
					continue;
				}

				if($customer_id == 2019){
					$select = $db->select("SELECT * FROM metering_point, r_reading WHERE mp_customer_id = '$customer_id' AND rea_cus_id = '$customer_id' AND mp_id = rea_mp_id AND mp_region_id = '$mp_region_id'");

				echo '<tr><td style="border-left:none; border-right:none; border-bottom: 2px solid #000;"></td>
					<td colspan="6" style="border-left:none; border-right:none; border-bottom: 2px solid #000;color:red;"><b>'.strtoupper($this->rgf("meter_region", $mp_region_id, "region_id", "region_name")).'</b></td>
					<td style="border:none;"></td>
					<td colspan="3" style="border-left:none; border-right:none; border-bottom: 2px solid #000;"></td></tr>';

				$db_values[] = array("", '&nbsp;<Br/><span style="color:red;font-weight: bold">'.strtoupper($this->rgf("meter_region", $mp_region_id, "region_id", "region_name")).'</span>',
				'','','','','','',
				//'&nbsp;<Br/><span style="color:red;font-weight: bold">'.strtoupper($this->rgf("meter_region", $mp_region_id, "region_id", "region_name")).'</span>',
				);
				}else{
					
					$select = $db->select("SELECT * FROM metering_point, r_reading WHERE mp_customer_id = '$customer_id' AND rea_cus_id = '$customer_id' AND mp_id = rea_mp_id AND mp_region_id = '$mp_region_id'");
				}

				$tw = 0;
				

				$total_amount = $total_amount2 = $total_wheeling = $total_wheeling2 = $wheeling2 = 0;
				$tc1 = $tc2 = $tc3 = $tc4= $tc5= $tc6= $tc7= $tc8 = 0;
				foreach($select[0] as $row){
					extract($row);

					if(date('Y', $rea_date)==$year && date('m', $rea_date)==$month){
						//selecting rates

						if($co%2 != 0){
							$df = "background-color: #e6e6e6;";
						}else{
							$df = "background-color:#fff;";
						}
						$co++;
						
						$db = new Db();
						$sel = $db->select("SELECT * FROM r_rate WHERE rate_reading_id = '$rea_id'");
						@extract($sel[0][0]);
						
						//calling the function
						
						if($customer_id == 1017){
							$cal = rate_calculate($customer_id, ($rea_date), $mp_id);
						}elseif($customer_id == 2021){
							$cal = rate_calculate($customer_id, ($rea_date), $mp_id);
						}else{
							$cal = rate_calculate2($customer_id, ($rea_date), $mp_id);
						}						
						
						
						//if($count != $total_mp){
						if(1){
							if($mp_id == 5327) //siti 2
								$mp_location = strtoupper("Siti Advance");
								
							if($mp_id == 5331) //siti 2
								$mp_location = strtoupper("KANUNGU KIHIHI");
							
							if($mp_id == 5328 || $mp_id == 5330){//siti 1
								echo '<tr id="'.($rea_id+5).'" style="display:none">';
								$co--;
							}else
								echo '<tr id="'.($rea_id+5).'">';
								
							
							echo '<td style="'.$df.' border-left:2px solid #000;">'.($co).'.</td>';
							echo '<td style="'.$df.' border-left:2px solid black;">'.$mp_location;

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
								
                    $current_customer = $this->rgf('customer', portion(3), 'customer_id', 'customer_short_name');
                    if($current_customer){
                        $type = portion(2);
                        $year = portion(4);
                        $month = portion(5);
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
										$show = array("schedule", "wheeling-charge-schedule", "view-invoice");
										
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

							if($customer_id == 2021 && $mp_id == 5310){ //KIL
								
								$cal_2 = rate_calculate($customer_id, ($rea_date), 5303); //Kikorongo
								$cal_3 = rate_calculate(2017, ($rea_date), 4009); //SNEL -> Mpondwe
								//print_r($cal);
								$g0 = ($cal[4] - $cal[0] - $cal_2[4] - $cal_3[0]);
								$g1 = ($cal[5] - $cal[1] - $cal_2[5] - $cal_3[1]);
								$g2 = ($cal[6] - $cal[2] - $cal_2[6] - $cal_3[2]);
								$g3 = $g1 + $g2 + $g0;

								$g4 = ($g0*$shoulder + $g1*$peak + $g2*$off_peak)/1000;

								$tw += $g3*$mp_wheeling_charge/100;
								$amount = ($g0*$shoulder+$g1*$peak+$g2*$off_peak)/1000; //imports
								$amount2 = ($cal[4]*$shoulder+$cal[5]*$peak+$cal[6]*$off_peak)/1000; //exports
								$total_amount += $amount;
								$total_wheeling += $amount*$mp_wheeling_charge/100;

								$wheeling2 = $amount2*$mp_wheeling_charge/100;
								$total_wheeling2 += $wheeling2;


								//$tc1 += $g1;
								//$tc2 += $g2;
								//$tc3 += $g3;
								//$tc4 += $g4;

								echo '<td style="'.$df.'border-left:2px solid black; text-align:right;">'.number_format($g0*$units).'</td>';
								echo '<td style="'.$df.' text-align:right;">'.number_format($g1*$units).'</td>';
								echo '<td style="'.$df.' text-align:right;">'.number_format($g2*$units).'</td>';

			
								echo '<td style="'.$df.' text-align:right;">'.number_format($g3*$units).'</td>';
								echo '<td style="'.$df.'border-right:2px solid black; text-align:right;">'.number_format($g4).'</td>';

							}elseif($customer_id == 2021 && $mp_id == 9383){ //KIL
								
								$cal = rate_calculate($customer_id, ($rea_date), 9383); 
								//print_r($cal);
								$g0 = ($cal[0] - $cal[4]);
								$g1 = ($cal[1] - $cal[5]);
								$g2 = ($cal[2] - $cal[6]);
								$g3 = $g1 + $g2 + $g0;

								$g4 = ($g0*$shoulder + $g1*$peak + $g2*$off_peak)/1000;

								$tw += $g3*$mp_wheeling_charge/100;
								$amount = ($g0*$shoulder+$g1*$peak+$g2*$off_peak)/1000; //imports
								$amount2 = ($cal[4]*$shoulder+$cal[5]*$peak+$cal[6]*$off_peak)/1000; //exports
								$total_amount += $amount;
								$total_wheeling += $amount*$mp_wheeling_charge/100;

								$wheeling2 = $amount2*$mp_wheeling_charge/100;
								$total_wheeling2 += $wheeling2;


								//$tc1 += $g1;
								//$tc2 += $g2;
								//$tc3 += $g3;
								//$tc4 += $g4;

								echo '<td style="'.$df.'border-left:2px solid black; text-align:right;">'.number_format($g0*$units).'</td>';
								echo '<td style="'.$df.' text-align:right;">'.number_format($g1*$units).'</td>';
								echo '<td style="'.$df.' text-align:right;">'.number_format($g2*$units).'</td>';

			
								echo '<td style="'.$df.' text-align:right;">'.number_format($g3*$units).'</td>';
								echo '<td style="'.$df.'border-right:2px solid black; text-align:right;">'.number_format($g4).'</td>';

							}elseif($customer_id == 2021 && $mp_id == 5302){ //KIL
								
								$cal = rate_calculate($customer_id, ($rea_date), 5302); 
								//print_r($cal);
								//echo $mp_location;
								$g0 = ($cal[0] - $cal[4]);
								$g1 = ($cal[1] - $cal[5]);
								$g2 = ($cal[2] - $cal[6]);
								$g3 = $g1 + $g2 + $g0;

								$g4 = ($g0*$shoulder + $g1*$peak + $g2*$off_peak)/1000;

								$tw += $g3*$mp_wheeling_charge/100;
								$amount = ($g0*$shoulder+$g1*$peak+$g2*$off_peak)/1000; //imports
								$amount2 = ($cal[4]*$shoulder+$cal[5]*$peak+$cal[6]*$off_peak)/1000; //exports
								$total_amount += $amount;
								$total_wheeling += $amount*$mp_wheeling_charge/100;

								$wheeling2 = $amount2*$mp_wheeling_charge/100;
								$total_wheeling2 += $wheeling2;


								//$tc1 += $g1;
								//$tc2 += $g2;
								//$tc3 += $g3;
								//$tc4 += $g4;

								echo '<td style="'.$df.'border-left:2px solid black; text-align:right;">'.number_format($g0*$units).'</td>';
								echo '<td style="'.$df.' text-align:right;">'.number_format($g1*$units).'</td>';
								echo '<td style="'.$df.' text-align:right;">'.number_format($g2*$units).'</td>';

			
								echo '<td style="'.$df.' text-align:right;">'.number_format($g3*$units).'</td>';
								echo '<td style="'.$df.'border-right:2px solid black; text-align:right;">'.number_format($g4).'</td>';

							}else{

								$g0 = ($cal[0]);
								$g1 = ($cal[1]);
								$g2 = ($cal[2]);
								$g3 = $g1 + $g2 + $g0;

								$g4 = ($g0*$shoulder + $g1*$peak + $g2*$off_peak)/1000;

								$tw += $g3*$mp_wheeling_charge/100;
								$amount = ($g0*$shoulder+$g1*$peak+$g2*$off_peak)/1000; //imports
								$amount2 = ($cal[4]*$shoulder+$cal[5]*$peak+$cal[6]*$off_peak)/1000; //exports
								$total_amount += $amount;
								$total_wheeling += $amount*$mp_wheeling_charge/100;

								$wheeling2 = $amount2*$mp_wheeling_charge/100;
								$total_wheeling2 += $wheeling2;

								echo '<td style="'.$df.'border-left:2px solid black; text-align:right;">'.number_format($g0*$units).'</td>';
								echo '<td style="'.$df.' text-align:right;">'.number_format($g1*$units).'</td>';
								echo '<td style="'.$df.' text-align:right;">'.number_format($g2*$units).'</td>';

			
								echo '<td style="'.$df.' text-align:right;">'.number_format($g3*$units).'</td>';
								echo '<td style="'.$df.'border-right:2px solid black; text-align:right;">'.number_format($g4).'</td>';
							}

							echo '<td style="border:none;"></td>';

							if( $this->payable($mp_id, $customer_id)){
								echo '<td style="border-left:2px solid #000; '.$df.'border-left:2px solid black;">'.number_format($mp_wheeling_charge,2).'&nbsp;%</td>';

								echo '<td style="border-left:2px solid #000;'.$df.'text-align: right;">
								'.number_format($amount*$mp_wheeling_charge/100).'</td>';

								echo '<td style="'.$df.'border-right:2px solid black;text-align: right;">'.$this->rgf("customer", $this->payable($mp_id, $customer_id), "customer_id", "customer_short_name").'</td>';	

							}else{

								echo '<td style="border-left:2px solid #000; '.$df.'border-left:2px solid black;">&nbsp;</td>';

								echo '<td style="border-left:2px solid #000;'.$df.'text-align: right;">
								</td>';

								echo '<td style="'.$df.'border-right:2px solid black;text-align: right;"></td>';	

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
						}else{			

							echo '<tr>';

							echo '<td style="'.$df.'border-left:2px solid #000;">'.($co).'.</td>';
							echo '<td style="'.$df.'border-left:2px solid black; border-bottom:2px solid black; ">'.$mp_location.'</td>';							
							echo '<td style="'.$df.'border-left:2px solid black; border-bottom:2px solid black;  text-align:right;">'.number_format($cal[0]*$units).'</td>';
							echo '<td style="'.$df.'border-bottom:2px solid black;  text-align:right;">'.number_format($cal[1]*$units).'</td>';
							echo '<td style="'.$df.'border-bottom:2px solid black;  text-align:right;">'.number_format($cal[2]*$units).'</td>';
							echo '<td style="'.$df.'border-bottom:2px solid black;  text-align:right;">'.number_format($cal[3]*$units).'</td>';
							echo '<td style="'.$df.'border-right:2px solid black; border-bottom:2px solid black; text-align:right; ">'.number_format($amount).'</td>';

							echo '<td style="border:none;"></td>';
							echo '<td style="'.$df.'border-bottom:2px solid black;border-left:2px solid black;">'.number_format($mp_wheeling_charge,2).' %</td>';

							echo '<td style="'.$df.'border-bottom:2px solid black;text-align: right;">'.number_format($amount*$mp_wheeling_charge/100).'</td>';
							echo '<td style="'.$df.'border-bottom:2px solid black;text-align: right;">'.$this->rgf("customer", $this->payable($mp_id, $customer_id), "customer_id", "customer_short_name").'</td>';
							echo '<td style="'.$df.'border-bottom:2px solid black;text-align: right;">'.number_format($amount*$mp_wheeling_charge/100).'</td>';

							echo '<td style="'.$df.'border-bottom:2px solid black;border-right:2px solid black;text-align: right;">'.number_format($wheeling2).'</td>';
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
				
				// $l = $this->rateSum($customer_id, $year, $month);

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
				
				$l = $this->rateSum($customer_id, $year, $month);

				$tc1 = $l[0];
			 	$tc2 = $l[1];
				$tc3 = $l[2];
				$tc4 = $l[3];
				$tc5 = $l[4];
				$tc6 = $l[5];
				$tc7 = $l[6];
				$tc8 = $l[7];

				$a1 = $a2 = $a3 = $a4 = 0;

				if(portion(3) == 2020){ //units from KRECS exports
					$reading_time = strtotime(date("$year-$month-15"));
					$t = rate_calculate(2022, $reading_time, 5315);

					$a1 = $t[0];
					$a2 = $t[1];
					$a3 = $t[2];
					$a4 = $t[3];
				}

				$tot = (($tc1+$a1)*$shoulder+($tc2+$a2)*$peak+($tc3+$a3)*$off_peak)/1000;

				if($co%2 != 0){
					$df = "background-color: #e6e6e6;";
				}else{
					$df = "background-color:#fff;";
				}
				//$co++;

				echo '<tr>';
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

				if(portion(3) == 2019){
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


				if(portion(3) == 2020){ //units from KRECS exports
					echo '<tr>';
					echo '<td style="'.$df.'border-top:2px solid #000; border-left:2px solid #000;border-bottom:2px solid black;border-top:2px solid #000;"></td>';
					echo '<td style="'.$df.'border-top:2px solid #000; border-left:2px solid black; border-bottom:2px solid black; font-weight: bold; ">Units from KRECS</td>';
					
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

				$payables = array();
				$db = new Db();
				$select = $db->select("SELECT distinct bn_customer_payable_to FROM boundary_node WHERE bn_customer = '$customer_id'");
				foreach($select[0] as $row){
					extract($row);

					$sel = $db->select("SELECT bn_metering_point FROM boundary_node WHERE bn_customer_payable_to = '$bn_customer_payable_to' AND bn_customer = '$customer_id' ");
					foreach($sel[0] as $ro){
						extract($ro);
						$payables[$bn_customer_payable_to][] = $bn_metering_point;
					}
				}

				//$payables[2022][] = 5315;

				$co++;

				$t = $t1 = $t2 = $t3 = $t4 = $t5 = 0;
				foreach($payables as $payable => $values){
					if($co%2 != 0){
						$df = "background-color: #e6e6e6;";
					}else{
						$df = "background-color:#fff;";
					}

					if($customer_id == 1017 && $payable == 2019 && $rea_date < strtotime(date('2019-08-18')) ){


					}elseif(0){
						$j = $this->krecsAndUedcl($payable, $customer_id, $year, $month);					
					}else{
						$j = $this->collectable($payable, $customer_id, $year, $month, $consider=0);
					}
					
					$t1 += $j[3]; $t2 += $j[4]; $t3 += $j[5]; $t4 += $j[6];
					echo '<tr>';
					echo '<td style="border-left:2px solid #000;'.$df.'">'.($co++).'.</td>';
					echo '<td style="border-left:2px solid #000;'.$df.' text-align-right;">'.$this->rgf("customer", $payable, "customer_id", "customer_short_name").'</td>';
					echo '<td style="border-left:2px solid #000;'.$df.' text-align:right;">'.number_format($j[3]).'</td>';
					echo '<td style="'.$df.' text-align:right;">'.number_format($j[4]).'</td>';
					echo '<td style="'.$df.' text-align:right;">'.number_format($j[5]).'</td>';
					echo '<td style="'.$df.' text-align:right;">'.number_format($j[6]).'</td>';

					echo '<td style="'.$df.'border-right:2px solid #000; text-align:right;">';
					
					$t += $j[2];
					echo number_format($j[2]);
					
					echo '</td>';

					
					echo '<td style="border:none;"></td>';
					echo '<td style="border:none;"></td>';
					echo '<td style="border:none;"></td>';
					echo '<td style="border:none;"></td>';
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
				echo '<td align="right" style="'.$df.' border-bottom: 2px solid #000; border-top:2px solid #000;  border-left: 2px solid #000; font-weight: bold;">'.number_format($t1).'</td>';
				echo '<td align="right" style="'.$df.' border-bottom: 2px solid #000; border-top:2px solid #000; font-weight: bold; ">'.number_format($t2).'</td>';
				echo '<td align="right" style="'.$df.' border-bottom: 2px solid #000; border-top:2px solid #000; font-weight: bold; ">'.number_format($t3).'</td>';
				echo '<td align="right" style="'.$df.' border-bottom: 2px solid #000;  border-top:2px solid #000; font-weight: bold;">'.number_format($t4).'</td>';				
				echo '<td align="right" style="'.$df.' border-bottom: 2px solid #000; border-right:2px solid #000; font-weight: bold; border-top:2px solid #000; ">'.number_format($t).'</td>';
				
				echo '<td style="border:none;"></td>';
				echo '<td style="border:none;"></td>';
				echo '<td style="border:none;"></td>';
				echo '<td style="border:none;"></td>';
				echo '</tr>';

				echo '<tr>';
				echo '<td colspan="11" style="border: none; display: none;">';
					/// hidden details
				echo 'sdf';	
					$i = 0;
					foreach($payables as $pay => $values){
						//AT KIL
						if($customer_id == 1017 && $pay == 2019 && $rea_date < strtotime(date('2019-08-18'))){

						}elseif(0){
							$j = $this->krecsAndUedcl($pay, $customer_id, $year, $month);					
						}elseif($customer_id == 2021 && $pay==2017 ){
							$j = $this->collectable2($pay, $customer_id, $year, $month, $consider=0);
						}else{	

							$j = $this->collectable($pay, $customer_id, $year, $month, $consider=0);
						}						
						echo '<div class="col-md-3">';
						echo $v = '<b>'.$this->rgf("customer", $pay, "customer_id", "customer_short_name").' ('.number_format($j[2]).'):</b>';

						echo '<div style="font-size:12px; margin-bottom: 10px;"> ';

						echo $v = "<ol><li>".implode("</li><li>", $j[1]).'</li></ol>  </div>';
						echo '</div>';
						$i++;
						if($i%4==0){
							//echo '<div class="clearfix"></div>';
						}
					}

				echo '</td>';
				echo '</tr>';

				echo '<tr>';
				echo '<td style="border:none; border-bottom:2px solid #000;"></td>';
				echo '<td colspan="6" style="border:none; border-bottom:2px solid #000;">&nbsp;<br/><b>WHEELING GHARGE COLLECTABLE FROM</b></td>';
				
				echo '<td colspan="5" style="border:none;"></td>';
				echo '</tr>';

				$payables = array();
				$db = new Db();

				$select = $db->select("SELECT distinct bn_customer FROM boundary_node WHERE bn_customer_payable_to = '$customer_id'");

				foreach($select[0] as $row){
					extract($row);

					$sel = $db->select("SELECT bn_metering_point FROM boundary_node WHERE bn_customer_payable_to = '$customer_id' AND bn_customer = '$bn_customer' ");
					foreach($sel[0] as $ro){
						extract($ro);
						$payables[$bn_customer][] = $bn_metering_point;
					}

				}

				
				if($customer_id == 2022){
					$payables[2020] = 5312;
				}

				//$co++;

				$t = $t1 = $t2 = $t3 = $t4 = $t5 = 0;
				if($customer_id == 2022){
					$payables[2020] = 5312;
				}

				echo '<tr style="border:2px solid #000">';
				echo '<td style="border-right:2px solid #000; text-align:center; font-weight:bold;">No.</td>';
				echo '<td style="border-right:2px solid #000; text-align:center; font-weight:bold;">Customer</td>';
				echo '<td style="text-align:center; font-weight:bold;">Shoulder</td>';
				echo '<td style="text-align:center; font-weight:bold;">Peak</td>';
				echo '<td style="text-align:center; font-weight:bold;">Off Peak</td>';
				echo '<td style="text-align:center; font-weight:bold;">Advance</td>';
				echo '<td style="text-align:center; font-weight:bold;">Amount</td>';
				echo '</tr>';

				foreach($payables as $payable => $values){
					if($co%2 != 0){
						$df = "background-color: #e6e6e6;";
					}else{
						$df = "background-color:#fff;";
					}
					
					//AT KIL
					if($customer_id == 1017 && $pay == 2019 && $rea_date < strtotime(date('2019-08-18'))){

					}elseif(0){
						$j = $this->krecsAndUedcl($customer_id, $payable, $year, $month);					
					}elseif($customer_id == 2021 && $payable==2017  ){
						$j = $this->collectable2($customer_id, $payable, $year, $month, $consider=2);
					}else{						
						$j = $this->collectable($customer_id, $payable, $year, $month, $consider=2);
					}

					$t1 += $j[3]; $t2 += $j[4]; $t3 += $j[5]; $t4 += $j[6];
					echo '<tr>';
					echo '<td style="border-left:2px solid #000;'.$df.'">'.($co++).'.</td>';
					echo '<td style="border-left:2px solid #000;'.$df.' text-align-right;">'.$this->rgf("customer", $payable, "customer_id", "customer_short_name").'</td>';
					echo '<td style="border-left:2px solid #000;'.$df.' text-align:right;">'.number_format($j[3]).'</td>';
					echo '<td style="'.$df.' text-align:right;">'.number_format($j[4]).'</td>';
					echo '<td style="'.$df.' text-align:right;">'.number_format($j[5]).'</td>';
					echo '<td style="'.$df.' text-align:right;">'.number_format($j[6]).'</td>';

					echo '<td style="'.$df.'border-right:2px solid #000; text-align:right;">';
					
					$t += $j[2];
					echo number_format($j[2]);
					
					echo '</td>';

					
					echo '<td style="border:none;"></td>';
					echo '<td style="border:none;"></td>';
					echo '<td style="border:none;"></td>';
					echo '<td style="border:none;"></td>';
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
				echo '<td align="right" style="'.$df.' border-bottom: 2px solid #000; border-top:2px solid #000;  border-left: 2px solid #000; font-weight: bold;">'.number_format($t1).'</td>';
				echo '<td align="right" style="'.$df.' border-bottom: 2px solid #000; border-top:2px solid #000; font-weight: bold; ">'.number_format($t2).'</td>';
				echo '<td align="right" style="'.$df.' border-bottom: 2px solid #000; border-top:2px solid #000; font-weight: bold; ">'.number_format($t3).'</td>';
				echo '<td align="right" style="'.$df.' border-bottom: 2px solid #000;  border-top:2px solid #000; font-weight: bold;">'.number_format($t4).'</td>';				
				echo '<td align="right" style="'.$df.' border-bottom: 2px solid #000; border-right:2px solid #000; font-weight: bold; border-top:2px solid #000; ">'.number_format($t).'</td>';
				
				echo '<td style="border:none;"></td>';
				echo '<td style="border:none;"></td>';
				echo '<td style="border:none;"></td>';
				echo '<td style="border:none;"></td>';
				echo '</tr>';


				echo '<tr>';
				echo '<td colspan="11" style="border: none; display: none;">';
					/// hidden details
					$i = 0;
					foreach($payables as $pay => $values){
						echo '<div class="col-md-3">';
						//AT KIL
						if($customer_id == 1017 && $pay == 2019 && $rea_date < strtotime(date('2019-08-18'))){

						}elseif(0){
							$j = $this->krecsAndUedcl($customer_id, $pay, $year, $month);					
						}elseif($customer_id == 2021 && $pay==2017 ){
							$j = $this->collectable2($customer_id, $pay, $year, $month, $consider=0);
						}else{						
							$j = $this->collectable($customer_id, $pay, $year, $month, $consider=0);
						}						

						echo $v = '<b>'.$this->rgf("customer", $pay, "customer_id", "customer_short_name").' ('.number_format($j[2]).'):</b>';

						echo '<div style="font-size:12px; margin-bottom: 10px;"> ';

						echo $v = "<ol><li>".implode("</li><li>", $j[1]).'</li></ol>  </div>';
						echo '</div>';
						$i++;
						if($i%4==0){
							//echo '<div class="clearfix"></div>';
						}
					}

				echo '</td>';
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

	public function wheelingChargeScheduleCreditNote($id, $year, $month){

		$db = new Db();
		$select = $db->select("SELECT * FROM customer WHERE customer_id = '$id'");
		extract($select[0][0]);

		// echo '<pre>';
		// print_r($this->totalBackflow($id, $year, $month));
		// echo '</pre>';
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
				echo '<title>'.strtoupper($customer_short_name.' WHEELING CHARGE').' - '.($month = portion(5)).' '.portion(4).'</title>';

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
					<th colspan="7" style="border:2px solid black;">
					<?php

					if($this->isDNorCN($id, $year, $month) > 0)
						$not = 'DEBIT NOTE';
					else
						$not = 'CREDIT NOTE';

					echo '<b class="badge"> '.$not.' </b><br/>';
					$g = "";
					$g .= $customer_full_name;
					$g .= ' WHEELING CHARGE ';
					$g .= $year = portion(4);
					$g .= ' - ';
					$g .= month_name($month = portion(5));
					echo $g;
					$g = "$not ".$g;
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
					<th colspan="3" style="border-top:2px solid black; border-right:2px solid black;border-left:2px solid black;">Wheeling Charge</th>			
				</tr>
				<tr valign="bottom">
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
					?>
					<?php $symbol = $this->rgf("currency",$customer_currency, "currency_id", "currency_symbol"); ?>
					<th style="border-left:2px solid black;border-bottom:2px solid black;">Shoulder<br/><span style="color:red;">@ <?php echo $shoulder1.'<br/>';?></span><?php echo $symbol;?>/kWh</th>
					<th style="border-bottom:2px solid black;">Peak<br/><span style="color:red;">@ <?php echo $peak1.'<br/>';?></span><?php echo $symbol;?>/kWh</th>
					<th style="border-bottom:2px solid black;">Off Peak<br/><span style="color:red;">@ <?php echo $off_peak1.'<br/>';?></span><?php echo $symbol;?>/kWh</th>
					<th style="border-bottom:2px solid black;">Advance<br/>(kWh)</th>
					<th style="border-right:2px solid black;border-bottom:2px solid black;">Amount<br/>(<?php echo $symbol;?>)</th>

					<th style="border-left:2px solid black; border-bottom: 2px solid black;">#</th>
					<th style="border-bottom:2px solid black;">Imports<br/>(Payable)<br/><b>(<?php echo $symbol;?>)</b></th>
					<th style="border-right:2px solid black;border-bottom:2px solid black;">Payable<br/>Customer</th>
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

				$total_mp;
				$db = new Db();

				$co = 0;
				$t = new Db();
				$sel = $t->select("SELECT DISTINCT mp_region_id FROM metering_point, r_reading_cn WHERE mp_customer_id = '$customer_id' AND n_rea_cus_id = '$customer_id' AND mp_id = n_rea_mp_id AND mp_region_id IS NOT NULL");

				foreach($sel[0] as $r){
				extract($r);
			
				if(!$t->num_rows()){
					continue;
				}

				$select = $db->select("SELECT * FROM metering_point, r_reading_cn, r_rate_cn WHERE mp_customer_id = '$customer_id' AND n_rea_cus_id = '$customer_id' AND mp_id = n_rea_mp_id AND mp_region_id = '$mp_region_id' AND n_rate_reading_id = n_rea_id AND (n_rate_import_wh_1 > 0 OR n_rate_import_wh_3 > 0 OR n_rate_export_wh_4 > 0 OR n_rate_export_wh_5 > 0 OR n_rate_export_wh_6 > 0 )");
				$tw = 0;
				
				echo '<tr><td style="border-left:none; border-right:none; border-bottom: 2px solid #000;"></td>
					<td colspan="6" style="border-left:none; border-right:none; border-bottom: 2px solid #000;color:red;"><b>'.strtoupper($this->rgf("meter_region", $mp_region_id, "region_id", "region_name")).'</b></td>
					<td style="border:none;"></td>
					<td colspan="3" style="border-left:none; border-right:none; border-bottom: 2px solid #000;"></td></tr>';

				$db_values[] = array("", '&nbsp;<Br/><span style="color:red;font-weight: bold">'.strtoupper($this->rgf("meter_region", $mp_region_id, "region_id", "region_name")).'</span>',
				'','','','','','',
				//'&nbsp;<Br/><span style="color:red;font-weight: bold">'.strtoupper($this->rgf("meter_region", $mp_region_id, "region_id", "region_name")).'</span>',
				);

				$total_amount = $total_amount2 = $total_wheeling = $total_wheeling2 = $wheeling2 = 0;
				$tc1 = $tc2 = $tc3 = $tc4= $tc5= $tc6= $tc7= $tc8 = 0;
				foreach($select[0] as $row){
					extract($row);

					if(date('Y', $n_rea_date)==$year && date('m', $n_rea_date)==$month){
						//selecting rates

						if($co%2 != 0){
							$df = "background-color: #e6e6e6;";
						}else{
							$df = "background-color:#fff;";
						}
						$co++;
						
						$db = new Db();
						$sel = $db->select("SELECT * FROM r_rate_cn WHERE n_rate_reading_id = '$n_rea_id'");
						@extract($sel[0][0]);
						
						//calling the function
						
						if($customer_id == 1017){
							$cal = rate_calculate_cn($customer_id, ($n_rea_date), $mp_id);
						}elseif($customer_id == 2021){
							$cal = rate_calculate_cn($customer_id, ($n_rea_date), $mp_id);
						}else{
							$cal = rate_calculate2_cn($customer_id, ($n_rea_date), $mp_id);
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
						//if($count != $total_mp){
						if(1){
							echo '<tr id="'.($n_rea_id+5).'">';
							echo '<td style="'.$df.' border-left:2px solid #000;">'.($co).'.</td>';
							echo '<td style="'.$df.' border-left:2px solid black;">'.$mp_location;

							// //rc_year = '$year' AND rc_month = '$month' AND 
							// $d =  new Db();
							// $se = $d->select("SELECT * FROM readings_comment WHERE rc_mp_id = '$mp_id' AND rc_customer_id = '$customer_id' AND rc_month = '$month' AND rc_year = '$year'");
						

							// if($d->num_rows()){
							// 	echo '<form action="" method="post" style="padding:0; margin:0;">';

							// 	if(isset($_POST['delete'.$mp_id])){

							// 		 $t = $_POST["rc$mp_id"];
							// 		 $customer = $_POST["customer$mp_id"];
							// 		 $readings = $_POST["rea_id$mp_id"];


							// 		$db = new Db();
							// 		$sql = "DELETE FROM readings_comment WHERE rc_id = '$t'";
							// 		$delete = $db->query($sql);

							// 		$select = $db->update("r_rate_cn",[
							// 			"n_rate_advance_1"=>NULL,
							// 			"n_rate_advance_2"=>NULL,
							// 			"n_rate_advance_3"=>NULL,
							// 			"n_rate_advance_4"=>NULL,
							// 			"n_rate_advance_5"=>NULL,
							// 			"n_rate_advance_6"=>NULL
							// 		], ["n_rate_cus_id"=>$customer, "n_rate_reading_id"=>$readings]);

							// 		FeedBack::redirect(return_url()."/".portion(1)."/".portion(2)."/".portion(3)."/".portion(4)."/".portion(5)."#$readings");
							// 	}

							// 	echo '<div id="hint" style=""><div class="message">';
							// 	echo '<p style="font-weight:bold;">'.$mp_location.'</p>';
							// 	foreach($se[0] as $r){
							// 		extract($r);
							// 		echo '<b>'.$this->full_name($rc_added_by).':</b><br/>';
							// 		echo nl2br($rc_comment);
							// 		echo '<br/>';
							// 		echo '<span style="color:#aaa;">'.Feedback::date_fm($rc_date_added).'</span>';
							// 	}
							// 	echo '<input type="hidden" name="rc'.$mp_id.'" value="'.$rc_id.'" />';
							// 	echo '<input type="hidden" name="customer'.$mp_id.'" value="'.$customer_id.'" />';
							// 	echo '<input type="hidden" name="rea_id'.$mp_id.'" value="'.$rea_id.'" />';
							// 	echo '<div id="add-new-hint"><button type="submit" name="delete'.$mp_id.'"><i class="fa fa-fw fa-times"></i></button></div>';
							// 	echo '</div></div>';
							// 	echo '</form>';
							// }else{ 
							// 	echo '<div>'; 

							// 	echo '<form action="" method="post" style="padding:0; margin:0;">';

							// 	if(isset($_POST['addComment'.$mp_id])){
							// 		$comment = $_POST['comment'];

							// 		$is = $_POST['is'];
							// 		$ip = $_POST['ip'];
							// 		$io = $_POST['io'];
							// 		$es = $_POST['es'];
							// 		$ep = $_POST['ep'];
							// 		$eo = $_POST['eo'];

							// 		if(empty($comment)){
							// 			FeedBack::error("Enter Comment");
							// 		}else{
							// 			$db = new Db();
							// 			$select = $db->insert("readings_comment", ["rc_comment"=>$comment, "rc_date_added"=>time(), "rc_added_by"=>user_id(), "rc_customer_id"=>$customer_id, "rc_year"=>$year, "rc_month"=>$month, "rc_mp_id"=>$mp_id, "rc_type"=>portion(2)]);

							// 			if($is != "" || $ip != "" || $io != "" || $es != "" || $ep != "" || $eo != ""){
							// 				$select = $db->update("r_rate",[
							// 					"rate_advance_1"=>$is*1000,
							// 					"rate_advance_2"=>$ip*1000,
							// 					"rate_advance_3"=>$io*1000,
							// 					"rate_advance_4"=>$es*1000,
							// 					"rate_advance_5"=>$ep*1000,
							// 					"rate_advance_6"=>$eo*1000
							// 				], ["rate_cus_id"=>$customer_id, "rate_reading_id"=>$rea_id]);
							// 			}

							// 			FeedBack::redirect(return_url()."/".portion(1)."/".portion(2)."/".portion(3)."/".portion(4)."/".portion(5)."#$rea_id");
							// 		}
							// 	}

							// 	echo '<div id="add-hint" style="color:white;"><i class="fa fa-fw fa-plus"></i>';
							// 	echo '<div class="add-comment" style="color:black;">';
							// 	echo '<div style="padding:10px;">';
							// 	echo '<p style="font-weight:bold; color:blue;">'.$mp_location.'</p>';
							// 	echo '<b>Add Comment</b><Br/> ';
							// 	echo '<textarea required style="width:100%; padding:10px;" placeholder="comment" name="comment"></textarea>';
							// 	echo '<br/>Advance Imports: <b>(KWh)</b><br/>';
							// 	echo '<input required style="width:30%; padding:1%; margin:1%;" placeholder="Shoulder" name="is" autocomplete="off" />';
							// 	echo '<input required style="width:30%; padding:1%; margin:1%;" placeholder="Peak" name="ip" autocomplete="off" />';
							// 	echo '<input required style="width:30%; padding:1%; margin:1%;" placeholder="Off Peak" name="io" autocomplete="off" />';

							// 	echo '<br/>Advance Exports: <b>(KWh)</b><br/>';
							// 	echo '<input required style="width:30%; padding:1%; margin:1%;" placeholder="Shoulder" name="es" autocomplete="off"/>';
							// 	echo '<input required style="width:30%; padding:1%; margin:1%;" placeholder="Peak" name="ep" autocomplete="off" />';
							// 	echo '<input required style="width:30%; padding:1%; margin:1%;" placeholder="Off Peak" name="eo" autocomplete="off" />';

							// 	echo '<br/><input type="submit" value="Save" name="addComment'.$mp_id.'">';
							// 	echo '</div>';
							// 	echo '</div>';
							// 	echo '</div>';
								
							// 	echo '</div>';

							// 	echo '</form>';
							// }

							

							echo '</td>';

							if($customer_id == 2021 && $mp_id == 5310){ //KIL
								
								$cal_2 = rate_calculate_cn($customer_id, ($rea_date), 5303); //Kikorongo
								$cal_3 = rate_calculate_cn(2017, ($rea_date), 4009); //SNEL -> Mpondwe
								//print_r($cal);
								$g0 = ($cal[4] - $cal[0] - $cal_2[4] - $cal_3[0]);
								$g1 = ($cal[5] - $cal[1] - $cal_2[5] - $cal_3[1]);
								$g2 = ($cal[6] - $cal[2] - $cal_2[6] - $cal_3[2]);
								$g3 = $g1 + $g2 + $g0;

								$g4 = ($g0*$shoulder + $g1*$peak + $g2*$off_peak)/1000;

								$tw += $g3*$mp_wheeling_charge/100;
								$amount = ($g0*$shoulder+$g1*$peak+$g2*$off_peak)/1000; //imports
								$amount2 = ($cal[4]*$shoulder+$cal[5]*$peak+$cal[6]*$off_peak)/1000; //exports
								$total_amount += $amount;
								$total_wheeling += $amount*$mp_wheeling_charge/100;

								$wheeling2 = $amount2*$mp_wheeling_charge/100;
								$total_wheeling2 += $wheeling2;


								//$tc1 += $g1;
								//$tc2 += $g2;
								//$tc3 += $g3;
								//$tc4 += $g4;

								echo '<td style="'.$df.'border-left:2px solid black; text-align:right;">'.number_format($g0*$units).'</td>';
								echo '<td style="'.$df.' text-align:right;">'.number_format($g1*$units).'</td>';
								echo '<td style="'.$df.' text-align:right;">'.number_format($g2*$units).'</td>';

			
								echo '<td style="'.$df.' text-align:right;">'.number_format($g3*$units).'</td>';
								echo '<td style="'.$df.'border-right:2px solid black; text-align:right;">'.number_format($g4).'</td>';

							}else{

								$g0 = ($cal[0]);
								$g1 = ($cal[1]);
								$g2 = ($cal[2]);
								$g3 = $g1 + $g2 + $g0;

								$g4 = ($g0*$shoulder + $g1*$peak + $g2*$off_peak)/1000;

								$tw += $g3*$mp_wheeling_charge/100;
								$amount = ($g0*$shoulder+$g1*$peak+$g2*$off_peak)/1000; //imports
								$amount2 = ($cal[4]*$shoulder+$cal[5]*$peak+$cal[6]*$off_peak)/1000; //exports
								$total_amount += $amount;
								$total_wheeling += $amount*$mp_wheeling_charge/100;

								$wheeling2 = $amount2*$mp_wheeling_charge/100;
								$total_wheeling2 += $wheeling2;

								echo '<td style="'.$df.'border-left:2px solid black; text-align:right;">'.number_format($g0*$units).'</td>';
								echo '<td style="'.$df.' text-align:right;">'.number_format($g1*$units).'</td>';
								echo '<td style="'.$df.' text-align:right;">'.number_format($g2*$units).'</td>';

			
								echo '<td style="'.$df.' text-align:right;">'.number_format($g3*$units).'</td>';
								echo '<td style="'.$df.'border-right:2px solid black; text-align:right;">'.number_format($g4).'</td>';
							}

							echo '<td style="border:none;"></td>';

							if( $this->payable($mp_id, $customer_id)){
								echo '<td style="border-left:2px solid #000; '.$df.'border-left:2px solid black;">'.number_format($mp_wheeling_charge,2).'&nbsp;%</td>';

								echo '<td style="border-left:2px solid #000;'.$df.'text-align: right;">
								'.number_format($amount*$mp_wheeling_charge/100).'</td>';

								echo '<td style="'.$df.'border-right:2px solid black;text-align: right;">'.$this->rgf("customer", $this->payable($mp_id, $customer_id), "customer_id", "customer_short_name").'</td>';	

							}else{

								echo '<td style="border-left:2px solid #000; '.$df.'border-left:2px solid black;">&nbsp;</td>';

								echo '<td style="border-left:2px solid #000;'.$df.'text-align: right;">
								</td>';

								echo '<td style="'.$df.'border-right:2px solid black;text-align: right;"></td>';	

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
						}else{			

							echo '<tr>';

							echo '<td style="'.$df.'border-left:2px solid #000;">'.($co).'.</td>';
							echo '<td style="'.$df.'border-left:2px solid black; border-bottom:2px solid black; ">'.$mp_location.'</td>';							
							echo '<td style="'.$df.'border-left:2px solid black; border-bottom:2px solid black;  text-align:right;">'.number_format($cal[0]*$units).'</td>';
							echo '<td style="'.$df.'border-bottom:2px solid black;  text-align:right;">'.number_format($cal[1]*$units).'</td>';
							echo '<td style="'.$df.'border-bottom:2px solid black;  text-align:right;">'.number_format($cal[2]*$units).'</td>';
							echo '<td style="'.$df.'border-bottom:2px solid black;  text-align:right;">'.number_format($cal[3]*$units).'</td>';
							echo '<td style="'.$df.'border-right:2px solid black; border-bottom:2px solid black; text-align:right; ">'.number_format($amount).'</td>';

							echo '<td style="border:none;"></td>';
							echo '<td style="'.$df.'border-bottom:2px solid black;border-left:2px solid black;">'.number_format($mp_wheeling_charge,2).' %</td>';

							echo '<td style="'.$df.'border-bottom:2px solid black;text-align: right;">'.number_format($amount*$mp_wheeling_charge/100).'</td>';
							echo '<td style="'.$df.'border-bottom:2px solid black;text-align: right;">'.$this->rgf("customer", $this->payable($mp_id, $customer_id), "customer_id", "customer_short_name").'</td>';
							echo '<td style="'.$df.'border-bottom:2px solid black;text-align: right;">'.number_format($amount*$mp_wheeling_charge/100).'</td>';

							echo '<td style="'.$df.'border-bottom:2px solid black;border-right:2px solid black;text-align: right;">'.number_format($wheeling2).'</td>';
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

					

					}					
				}
				
				// $l = $this->rateSum($customer_id, $year, $month);

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
				
				$l = $this->rateSumCN($customer_id, $year, $month);

				$tc1 = $l[0];
			 	$tc2 = $l[1];
				$tc3 = $l[2];
				$tc4 = $l[3];
				$tc5 = $l[4];
				$tc6 = $l[5];
				$tc7 = $l[6];
				$tc8 = $l[7];

				$tot = ($tc1*$shoulder+$tc2*$peak+$tc3*$off_peak)/1000;

				if($co%2 != 0){
					$df = "background-color: #e6e6e6;";
				}else{
					$df = "background-color:#fff;";
				}
				//$co++;

				echo '<tr>';
				echo '<td style="'.$df.'border-top:2px solid #000; border-left:2px solid #000;border-bottom:2px solid black;border-top:2px solid #000;"></td>';
				echo '<td style="'.$df.'border-top:2px solid #000; border-left:2px solid black; border-bottom:2px solid black; text-align:right;font-weight: bold; ">Gross Units</td>';
				
				echo '<td style="'.$df.'border-top:2px solid #000; border-bottom:2px solid black;  border-left:2px solid black;  text-align:right;font-weight: bold;">'.number_format($tc1*$units,2).'</td>';
				echo '<td style="'.$df.'border-top:2px solid #000; border-bottom:2px solid black;  text-align:right;font-weight: bold;">'.number_format($tc2*$units,2).'</td>';
				echo '<td style="'.$df.'border-top:2px solid #000; border-bottom:2px solid black;  text-align:right;font-weight: bold;">'.number_format($tc3*$units,2).'</td>';
				echo '<td style="'.$df.'border-top:2px solid #000; border-bottom:2px solid black;  text-align:right;font-weight: bold;">'.number_format($tc4*$units,2).'</td>';
				echo '<td style="'.$df.'border-top:2px solid #000; border-right:2px solid black; border-bottom:2px solid black; text-align:right; font-weight: bold; ">'.number_format($tot).'</td>';

				echo '<td colspan="4" style="border-top:2px solid #000; border:none;"></td>';
				
				echo '</tr>';

				echo '<tr><td colspan="7" style="border:none;"></td></tr>';

				if(portion(3) == 2019){
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

				}
				


				echo '<tr>';
				echo '<td style="border:none; border-bottom:2px solid #000;"></td>';
				echo '<td colspan="6" style="border:none; border-bottom:2px solid #000;">&nbsp;<Br/><b>WHEELING CHARGE PAYABLE TO</b></td>';
			
				
				echo '<td colspan="5" style="border:none;"></td>';
				echo '</tr>';

				$payables = array();
				$db = new Db();
				$select = $db->select("SELECT distinct bn_customer_payable_to FROM boundary_node WHERE bn_customer = '$customer_id'");
				foreach($select[0] as $row){
					extract($row);

					$sel = $db->select("SELECT bn_metering_point FROM boundary_node WHERE bn_customer_payable_to = '$bn_customer_payable_to' AND bn_customer = '$customer_id' ");
					foreach($sel[0] as $ro){
						extract($ro);
						$payables[$bn_customer_payable_to][] = $bn_metering_point;
					}
				}

				$payables[2022][] = 5315;

				$co++;

				$t = $t1 = $t2 = $t3 = $t4 = $t5 = 0;
				foreach($payables as $payable => $values){
					if($co%2 != 0){
						$df = "background-color: #e6e6e6;";
					}else{
						$df = "background-color:#fff;";
					}

					if($customer_id == 1017 && $payable == 2019 && $rea_date < strtotime(date('2019-08-18')) ){


					}elseif(0){
						$j = $this->krecsAndUedcl($payable, $customer_id, $year, $month);					
					}else{

						$j = $this->collectable($payable, $customer_id, $year, $month, $consider=0);
					}
					
					$t1 += $j[3]; $t2 += $j[4]; $t3 += $j[5]; $t4 += $j[6];
					echo '<tr>';
					echo '<td style="border-left:2px solid #000;'.$df.'">'.($co++).'.</td>';
					echo '<td style="border-left:2px solid #000;'.$df.' text-align-right;">'.$this->rgf("customer", $payable, "customer_id", "customer_short_name").'</td>';
					echo '<td style="border-left:2px solid #000;'.$df.' text-align:right;">'.number_format($j[3]).'</td>';
					echo '<td style="'.$df.' text-align:right;">'.number_format($j[4]).'</td>';
					echo '<td style="'.$df.' text-align:right;">'.number_format($j[5]).'</td>';
					echo '<td style="'.$df.' text-align:right;">'.number_format($j[6]).'</td>';

					echo '<td style="'.$df.'border-right:2px solid #000; text-align:right;">';
					
					$t += $j[2];
					echo number_format($j[2]);
					
					echo '</td>';

					
					echo '<td style="border:none;"></td>';
					echo '<td style="border:none;"></td>';
					echo '<td style="border:none;"></td>';
					echo '<td style="border:none;"></td>';
					echo '</tr>';

				}

				if($co%2 != 0){
					$df = "background-color: #e6e6e6;";
				}else{
					$df = "background-color:#fff;";
				}
				$co++;

				echo '<tr>';
				echo '<td style="'.$df.' border-bottom: 2px solid #000; border-left: 2px solid #000; border-top:2px solid #000; "></td>';
				echo '<td style="'.$df.' border-bottom: 2px solid #000; border-left: 2px solid #000; border-top:2px solid #000; text-align: right; font-weight: bold; ">Total</td>';
				echo '<td align="right" style="'.$df.' border-bottom: 2px solid #000; border-top:2px solid #000;  border-left: 2px solid #000; font-weight: bold;">'.number_format($t1).'</td>';
				echo '<td align="right" style="'.$df.' border-bottom: 2px solid #000; border-top:2px solid #000; font-weight: bold; ">'.number_format($t2).'</td>';
				echo '<td align="right" style="'.$df.' border-bottom: 2px solid #000; border-top:2px solid #000; font-weight: bold; ">'.number_format($t3).'</td>';
				echo '<td align="right" style="'.$df.' border-bottom: 2px solid #000;  border-top:2px solid #000; font-weight: bold;">'.number_format($t4).'</td>';				
				echo '<td align="right" style="'.$df.' border-bottom: 2px solid #000; border-right:2px solid #000; font-weight: bold; border-top:2px solid #000; ">'.number_format($t).'</td>';
				
				echo '<td style="border:none;"></td>';
				echo '<td style="border:none;"></td>';
				echo '<td style="border:none;"></td>';
				echo '<td style="border:none;"></td>';
				echo '</tr>';

				echo '<tr>';
				echo '<td colspan="11" style="border: none; display: none;">';
					/// hidden details
					$i = 0;
					foreach($payables as $pay => $values){
						//AT KIL
						if($customer_id == 1017 && $pay == 2019 && $rea_date < strtotime(date('2019-08-18'))){

						}elseif(0){
							$j = $this->krecsAndUedcl($pay, $customer_id, $year, $month);					
						}elseif($customer_id == 2021 && $pay==2017 ){
							$j = $this->collectable2($pay, $customer_id, $year, $month, $consider=0);
						}else{						
							$j = $this->collectable($pay, $customer_id, $year, $month, $consider=0);
						}						
						echo '<div class="col-md-3">';
						echo $v = '<b>'.$this->rgf("customer", $pay, "customer_id", "customer_short_name").' ('.number_format($j[2]).'):</b>';

						echo '<div style="font-size:12px; margin-bottom: 10px;"> ';

						echo $v = "<ol><li>".implode("</li><li>", $j[1]).'</li></ol>  </div>';
						echo '</div>';
						$i++;
						if($i%4==0){
							//echo '<div class="clearfix"></div>';
						}
					}

				echo '</td>';
				echo '</tr>';

				echo '<tr>';
				echo '<td style="border:none; border-bottom:2px solid #000;"></td>';
				echo '<td colspan="6" style="border:none; border-bottom:2px solid #000;">&nbsp;<br/><b>WHEELING GHARGE COLLECTABLE FROM</b></td>';
				
				echo '<td colspan="5" style="border:none;"></td>';
				echo '</tr>';

				$payables = array();
				$db = new Db();

				$select = $db->select("SELECT distinct bn_customer FROM boundary_node WHERE bn_customer_payable_to = '$customer_id'");

				foreach($select[0] as $row){
					extract($row);

					$sel = $db->select("SELECT bn_metering_point FROM boundary_node WHERE bn_customer_payable_to = '$customer_id' AND bn_customer = '$bn_customer' ");
					foreach($sel[0] as $ro){
						extract($ro);
						$payables[$bn_customer][] = $bn_metering_point;
					}

				}

				
				if($customer_id == 2022){
					$payables[2020] = 5312;
				}

				$co++;

				$t = $t1 = $t2 = $t3 = $t4 = $t5 = 0;
				if($customer_id == 2022){
					$payables[2020] = 5312;
				}
				foreach($payables as $payable => $values){
					if($co%2 != 0){
						$df = "background-color: #e6e6e6;";
					}else{
						$df = "background-color:#fff;";
					}
					
					//AT KIL
					if($customer_id == 1017 && $pay == 2019 && $rea_date < strtotime(date('2019-08-18'))){

					}elseif(0){
						$j = $this->krecsAndUedcl($customer_id, $payable, $year, $month);					
					}elseif($customer_id == 2021 && $payable==2017  ){
						$j = $this->collectable2($customer_id, $payable, $year, $month, $consider=2);
					}else{						
						$j = $this->collectable($customer_id, $payable, $year, $month, $consider=2);
					}

					$t1 += $j[3]; $t2 += $j[4]; $t3 += $j[5]; $t4 += $j[6];
					echo '<tr>';
					echo '<td style="border-left:2px solid #000;'.$df.'">'.($co++).'.</td>';
					echo '<td style="border-left:2px solid #000;'.$df.' text-align-right;">'.$this->rgf("customer", $payable, "customer_id", "customer_short_name").'</td>';
					echo '<td style="border-left:2px solid #000;'.$df.' text-align:right;">'.number_format($j[3]).'</td>';
					echo '<td style="'.$df.' text-align:right;">'.number_format($j[4]).'</td>';
					echo '<td style="'.$df.' text-align:right;">'.number_format($j[5]).'</td>';
					echo '<td style="'.$df.' text-align:right;">'.number_format($j[6]).'</td>';

					echo '<td style="'.$df.'border-right:2px solid #000; text-align:right;">';
					
					$t += $j[2];
					echo number_format($j[2]);
					
					echo '</td>';

					
					echo '<td style="border:none;"></td>';
					echo '<td style="border:none;"></td>';
					echo '<td style="border:none;"></td>';
					echo '<td style="border:none;"></td>';
					echo '</tr>';


				}

				if($co%2 != 0){
					$df = "background-color: #e6e6e6;";
				}else{
					$df = "background-color:#fff;";
				}
				$co++;

				echo '<tr>';
				echo '<td style="'.$df.' border-bottom: 2px solid #000; border-left: 2px solid #000; border-top:2px solid #000; "></td>';
				echo '<td style="'.$df.' border-bottom: 2px solid #000; border-left: 2px solid #000; border-top:2px solid #000; text-align: right; font-weight: bold; ">Total</td>';
				echo '<td align="right" style="'.$df.' border-bottom: 2px solid #000; border-top:2px solid #000;  border-left: 2px solid #000; font-weight: bold;">'.number_format($t1).'</td>';
				echo '<td align="right" style="'.$df.' border-bottom: 2px solid #000; border-top:2px solid #000; font-weight: bold; ">'.number_format($t2).'</td>';
				echo '<td align="right" style="'.$df.' border-bottom: 2px solid #000; border-top:2px solid #000; font-weight: bold; ">'.number_format($t3).'</td>';
				echo '<td align="right" style="'.$df.' border-bottom: 2px solid #000;  border-top:2px solid #000; font-weight: bold;">'.number_format($t4).'</td>';				
				echo '<td align="right" style="'.$df.' border-bottom: 2px solid #000; border-right:2px solid #000; font-weight: bold; border-top:2px solid #000; ">'.number_format($t).'</td>';
				
				echo '<td style="border:none;"></td>';
				echo '<td style="border:none;"></td>';
				echo '<td style="border:none;"></td>';
				echo '<td style="border:none;"></td>';
				echo '</tr>';


				echo '<tr>';
				echo '<td colspan="11" style="border: none; display: none;">';
					/// hidden details
					$i = 0;
					foreach($payables as $pay => $values){
						echo '<div class="col-md-3">';
						//AT KIL
						if($customer_id == 1017 && $pay == 2019 && $rea_date < strtotime(date('2019-08-18'))){

						}elseif(0){
							$jm = $this->krecsAndUedcl( $customer_id, $pay, $year, $month);					
						}elseif($customer_id == 2021 && $pay==2017 ){
							$j = $this->collectable2($customer_id, $pay, $year, $month, $consider=0);
						}else{						
							$j = $this->collectable($customer_id, $pay, $year, $month, $consider=0);
						}						

						echo $v = '<b>'.$this->rgf("customer", $pay, "customer_id", "customer_short_name").' ('.number_format($j[2]).'):</b>';

						echo '<div style="font-size:12px; margin-bottom: 10px;"> ';

						echo $v = "<ol><li>".implode("</li><li>", $j[1]).'</li></ol>  </div>';
						echo '</div>';
						$i++;
						if($i%4==0){
							//echo '<div class="clearfix"></div>';
						}
					}

				echo '</td>';
				echo '</tr>';
				

				?>
			</tbody></table>
			<?php 
				$this->showReadingComments();
			?>
		</div>
			<?php 

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

			?>		
	
	</div>
	<?php
	}

	public function backflowScheduleAction(){

		
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
		<br/>
		<style type="text/css">.schedule{font-size: 12px !important;}</style>
		<div id="printMe">
						
				<?php 
				echo '<title>'.strtoupper($customer_short_name.' BACKFLOW SCHEDULE').' - '.($month = portion(5)).' '.portion(4).'</title>';

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
					$g .= $year = portion(4);
					$g .= ' - ';
					$g .= month_name($month = portion(5));
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
				$sel = $t->select("SELECT DISTINCT mp_region_id FROM metering_point, r_reading WHERE mp_customer_id = '$customer_id' AND rea_cus_id = '$customer_id' AND mp_id = rea_mp_id AND mp_region_id IS NOT NULL");

				foreach($sel[0] as $r){
				extract($r);
			
				if(!$t->num_rows()){
					continue;
				}

				$select = $db->select("SELECT * FROM metering_point, r_reading WHERE mp_customer_id = '$customer_id' AND rea_cus_id = '$customer_id' AND mp_id = rea_mp_id AND mp_region_id = '$mp_region_id'");
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

					if(date('Y', $rea_date)==$year && date('m', $rea_date)==$month && portion(3)==2019){
						//selecting rates

						
						$db = new Db();
						$sel = $db->select("SELECT * FROM r_rate WHERE rate_reading_id = '$rea_id'");
						@extract($sel[0][0]);
						
						//calling the function
						$cal = rate_calculate2($customer_id, ($rea_date), $mp_id);
						//echo $mp_location.' == '.$cal[4].'<br/>';
						$tw += $cal[3]*$mp_wheeling_charge/100;
						$amount2 = ($cal[4]*$shoulder+$cal[5]*$peak+$cal[6]*$off_peak)/1000; //exports

						if($amount2){
						

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
							echo '<td style="'.$df.' border-left:2px solid black;">'.$mp_location;

							{
								echo '<td style="'.$df.'border-left:2px solid black; text-align:right;">'.number_format($cal[4]*$units).'</td>';
								echo '<td style="'.$df.'border-right:2px solid black; text-align:right;">'.number_format($cal[4]*$units*$shoulder).'</td>';

								echo '<td style="'.$df.' text-align:right;">'.number_format($cal[5]*$units).'</td>';
								echo '<td style="'.$df.'border-right:2px solid black; text-align:right;">'.number_format($cal[5]*$units*$peak).'</td>';

								echo '<td style="'.$df.'text-align:right;">'.number_format($cal[6]*$units).'</td>';
								echo '<td style="'.$df.'border-right:2px solid black;  text-align:right;">'.number_format($cal[6]*$units*$off_peak).'</td>';

			
								echo '<td style="'.$df.'border-right:2px solid black;  text-align:right;">'.number_format($cal[7]*$units).'</td>';
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
					$l = $this->rateSum($customer_id, $year, $month);

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
						echo '<td style="border-top:2px solid black; border-left:2px solid black; border-bottom:2px solid black;  text-align:right;">'.number_format($tc5*$units).'</td>';

						echo '<td style="border-top:2px solid black; border-right:2px solid black; border-bottom:2px solid black;  text-align:right;">'.number_format($tc5*$units*$shoulder).'</td>';

						echo '<td style="border-top:2px solid black; border-bottom:2px solid black;  text-align:right;">'.number_format($tc6*$units).'</td>';
						echo '<td style="border-top:2px solid black;border-right:2px solid black; border-bottom:2px solid black;  text-align:right;">'.number_format($tc6*$units*$peak).'</td>';

						echo '<td style="border-top:2px solid black; border-bottom:2px solid black;  text-align:right;">'.number_format($tc7*$units).'</td>';
						echo '<td style="border-top:2px solid black;border-right:2px solid black; border-bottom:2px solid black;  text-align:right;">'.number_format($tc7*$units*$off_peak).'</td>';

						echo '<td style="border-top:2px solid black; border-right:2px solid black; border-bottom:2px solid black; text-align:right; ">'.number_format($tc8*$units).'</td>';
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
			$all_points_ids[] = $bn_metering_point;

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

			$l = $this->rateSum($customer_collectable, $year, $month);
						
		}else
		*/

		if($customer_payable == 2019 && $customer_collectable == 1017){ //BECS - kiitumba to UMEME;
			$tou = $this->tou($customer_collectable, $month, $year);
			
			$peak = $tou[0];
			$shoulder = $tou[1];
			$off_peak = $tou[2];

			$select = "SELECT mp_wheeling_charge FROM metering_point WHERE mp_id = 3009";
			$db = new Db();
			$select = $db->select($select);
			extract($select[0][0]);
						
			$k = rate_calculate($customer_collectable, strtotime(date("$year-$month-15")), 3009);
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

		}elseif($customer_payable == 2019 && $customer_collectable == 1017){ //BECS - kiitumba to UMEME;

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

				$l = $this->rateSum($customer_collectable, $year, $month);
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
			
				echo '>>>'.$peak = $tou[0];
				echo '>>>'.$shoulder = $tou[1];
				echo '>>>'.$off_peak = $tou[2];
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
	public function payable($mp_id, $customer_id, $consider = 0){
		$db = new Db();
		$select = $db->select("SELECT bn_customer_payable_to FROM boundary_node WHERE bn_metering_point = '$mp_id' AND bn_customer = '$customer_id'");
		extract($select[0][0]);

		return $bn_customer_payable_to;
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

		
		$id = portion(3);
		$year = portion(4);
		$month = portion(5);
		$note = portion(6);

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

		$curr = $this->rgf("generators", $customer_id, "ge_id", "ge_currency");

		$db = new Db();
		$select = $db->select("SELECT ex_usd, ex_euro FROM exchange_rate WHERE ex_year = '$year' AND ex_month = '$month'");
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
			$value['shs'] = $amount;
			$value['usd'] = $amount/$ex_usd;
			$value['euro'] = $amount/$ex_euro;
		}elseif(strtoupper($curr) == "EUR"){
			$value['euro'] = $amount;
			$value['shs'] = $amount*$ex_euro;
			$value['usd'] = $amount*$ex_euro/$ex_usd;
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
			}elseif($customer_id == 10){//jacobsen
				$value['price'] = ($value['usd']+($value['euro']*$ex_euro/$ex_usd))/$units;
			}elseif($customer_id == 3)
				$value['price'] = $v+0.03;
			else
				$value['price'] = $v;

			$value['units'] = $units;
		}

		return $value;
	}

	public function sum($year, $month, $type){
		$db = new Db();
		
		if($type==5){
			$sql = "SELECT ge_name, ge_id, ge_category FROM generators WHERE ge_id = 38 or ge_id = 10 ORDER BY ge_name ASC";
			$select = $db->select($sql);
		}else{
			//$sql = "SELECT ge_name, ge_id,ge_category FROM generators WHERE ge_id != 38 AND ge_id != 10 ORDER BY ge_name ASC";
			$sql = "SELECT ge_name, ge_id,ge_category FROM generators";
			$select = $db->select($sql);
		}

		$top = 0;
		$bottom =  0;

		foreach($select[0] as $row){
			extract($row);
			$v = $this->values($year, $month, $ge_id);

			if($ge_id == 38)//KPLC
				$val = ($v['units']/1000)*$v['price'];
			elseif($ge_id==10)				
				$val = ($v['units']/1000)*$v['price'];
			else				
				$val = ($v['units']/1000)*$v['price'];

			$val = (is_nan($val))?0:$val;
			$val = ($val==INF)?0:$val;
			//echo '<br/>..'.$ge_name.'='.($v['units']/1000).'='.$v['price'];
			$top += $val;
			$bottom += $v['units']/1000;


		//echo '<p style="text-align:left;"><b>'.$ge_name.'</b><br/>'.'['.($v['units']/1000).'/<b>'.$v['price'].'</b>='.($val).']</p>';

			//echo $ge_name.'='.$v['units'].'='.$v['price'].'='.$top.'<br/>';

		}

		//echo 't: '.number_format($top, 4).'<br/>';

		//echo 'b: '.number_format($bottom, 4).'<br/>';

		if($type!=5){
			$select = $db->select("SELECT ex_loss_factor FROM exchange_rate WHERE ex_year = '$year' AND ex_month = '$month'");
			extract($select[0][0]);
			$bottom = (1-$ex_loss_factor)*$bottom;
		}

		$value = number_format(($top/$bottom)*100, 5);
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

		if($id == 2029)
{			echo '<div class="clearfix"></div>';
			echo '<div id="printMe">';
			echo '<style>@media print{table{font-size:10px}} @media print{ @page {size: A4; font-size:10px;  @top-left {content: "Ghost Stories";}} </style>';
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
			echo '<table id="sortSpan" border="1" cellpadding="2" cellspacing="0" style="font-family:arial;">';
			echo '<thead>';
			echo '<tr align="center" valign="bottom" style=" font-weight:bold;">';			
			echo '<td style="height:120px; border-top:2px solid #000; border-bottom:2px solid #000; border-left:2px solid #000; " rowspan="3">No.</td>';
			echo '<td style="border-top:2px solid #000; border-bottom:2px solid #000; border-right:2px solid #000; border-left:2px solid #000; " rowspan="3">Customer</td>';

			foreach($$use as $m){
				if(1)
						echo '<td colspan="3" style="border-top:2px solid #000; border-right:2px solid #000;"><b>'.month_name($m).'</b></td>';
				else
						echo '<td colspan="5" style="border-top:2px solid #000; border-right:2px solid #000;"><b>'.month_name($m).'</b></td>';
			}
			echo '</tr>';

			echo '<tr align="center" valign="bottom" style="font-weight:bold;">';
			foreach($$use as $m){
				echo '<td rowspan="2" style="border-bottom:2px solid #000;">Units<b><br/><span style="color:red">(KWH)</span></td>';
				if(1)
				echo '<td>Bill</td>';
				else
				echo '<td colspan="3">Bill</td>';
				echo '<td rowspan="2" style="border-bottom:2px solid #000; border-right:2px solid #000;">Price<br/><span style="color:red;">(USD/kWh)</span></td>';
			}
			echo '</tr>';

			echo '<tr align="center" valign="bottom" style="font-weight:bold;">';
			foreach($$use as $m)
			{
				if(1){
					echo '<td style="border-bottom:2px solid #000;">USD:<span style="color:red;">';
				
					echo '@'.number_format($this->ex_rate($year, $m, 'usd'), 2).'/shs</span>';
					echo '<br/>Euro:<span style="color:red;">@'.number_format($this->ex_rate($year, $m, 'euro'), 2).'/shs</span>';
					//echo '<br/>Shs:<span style="color:red;"></span>';
					echo '</td>';
				}else{					
					echo '<td style="border-bottom:2px solid #000;">USD<br/><span style="color:red;">@'.number_format($this->ex_rate($year, $m, 'usd'), 2).'/shs</span></td>';
					echo '<td style="border-bottom:2px solid #000;">Euro<br/><span style="color:red;">@'.number_format($this->ex_rate($year, $m, 'euro'), 2).'/shs</span></td>';
					echo '<td style="border-bottom:2px solid #000;">Shs<br/><span style="color:red;"></span>';
				}

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
						$v = $this->values($year, $m, $ge_id);
						echo '<td style="text-align:right; min-width:80px;">'.number_format($this->mixPrice($year, $m, $ge_id, 'units')/1000).'</td>';

						if(1){
						echo '<td style="text-align:left; min-width:80px;">';
						echo '<b>USD:</b>&nbsp;'.number_format($v['usd']/1000).'<br/>';
						echo '<b>EUR:</b>&nbsp;'.number_format($v['euro']/1000).'<br/>';
						echo '<b>SHS:</b>&nbsp;'.number_format($v['shs']/1000);
						echo '</td>';

						}else{
						echo '<td style="text-align:right; min-width:80px;">'.number_format($v['usd']/1000).'</td>';
						echo '<td style="text-align:right; min-width:80px;">'.number_format($v['euro']/1000).'</td>';
						echo '<td style="text-align:right; min-width:80px;">'.number_format($v['shs']/1000).'</td>';

						}
						echo '<td style="text-align:right; border-right:2px solid #000;">'.number_format($v['price'],4).'</td>';
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
					$e += $this->sum($year, $m, 0);
					echo $this->sum($year, $m, 0);
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
		}else{
		?>
		<!-- <input type="search" class="search" data-column="all" placeholder="Search all columns">
		<button type="button" class="reset">Reset</button> -->
		<style>.tg{width:700px; margin:auto;}</style>
		<div id="printMe">
			<style>table{border:none;}table tr td, table tr th{ height: 10px !important;} table tr th{ text-align: center;} <?php if($id != 2019 && $id != 2030 && $id != 2028) { echo '@media print{ #rt{display:none;} @page {size: A4 landscape;  @top-left {content: "Ghost Stories";}} .net-advance-form{display:none;}}'; }else{ echo '@media print{.net-advance-form{display:none;}}'; } ?> </style>
			<?php 
			echo '<title>'.strtoupper($customer_short_name.' Meter Readings').' - '.($month = portion(5)).' '.portion(4).'</title>';
			?>
			<center class="hide"><h3>UGANDA ELECTRICITY TRANSMISSION COMPANY LTD</h3></center>

			<?php 
				echo '<div style="clear:both;"></div>';

				if(portion(3)==2028){

					$db = new Db();
					//current main 1
					$customer_id = portion(3);
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
				<h3 style="text-align: right123"> MONTH: <span style="border-bottom:2px solid #000; padding:0 10px; "><?php echo month_name(portion(5)).' - '.portion(4); ?></span></h3>
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
				}elseif(portion(3)==2030){
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
					$g .= $year = portion(4);
					$g .= ' - ';
					$g .= month_name($month = portion(5));

					echo $g;
					echo '</b>';
					$customer_id = portion(3);
					$reading_time = strtotime(date("$year-$month-15"));

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
					$sql = "SELECT TOP 1 * FROM r_reading, r_rate, metering_point WHERE rea_id <  '$rea_id' AND rea_id = rate_reading_id AND rea_cus_id = '2030' AND rea_mp_id = mp_id AND rate_cus_id = '2030' AND upper(mp_location) = 'MAIN 1' ORDER BY rea_date DESC";
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
					$sql = "SELECT TOP 1 * FROM r_reading, r_rate, metering_point WHERE rea_id < '$rea_id' AND rea_id = rate_reading_id AND rea_cus_id = '2030' AND rea_mp_id = mp_id AND rate_cus_id = '2030' AND upper(mp_location) = 'MAIN 2' ORDER BY rea_date DESC";
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
					$sql = "SELECT * FROM r_reading, r_rate, metering_point WHERE rea_id < '$rea_id' AND rea_id = rate_reading_id AND rea_cus_id = '2030' AND rea_mp_id = mp_id AND rate_cus_id = '2030' AND upper(mp_location) = 'CHECK 1' ORDER BY rea_date DESC";
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
					$sql = "SELECT * FROM r_reading, r_rate, metering_point WHERE rea_id < '$rea_id' AND rea_id = rate_reading_id AND rea_cus_id = '2030' AND rea_mp_id = mp_id AND rate_cus_id = '2030' AND upper(mp_location) = 'CHECK 2' ORDER BY rea_date DESC";
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
					echo '<td style="border-bottom: 2px solid #000; min-width:150px; font-weight:bold; text-align:center; ">Main 1</td>';
					echo '<td style="border-bottom: 2px solid #000; min-width:150px; font-weight:bold; text-align:center; border-right:2px solid #000;">Main 2</td>';
					echo '<td style="border-bottom: 2px solid #000; min-width:150px; font-weight:bold; text-align:center; ">Main 1</td>';
					echo '<td style="border-bottom: 2px solid #000; min-width:150px; font-weight:bold; text-align:center; border-right:2px solid #000;">Main 2</td>';
					echo '<td style="border-bottom: 2px solid #000; min-width:150px; font-weight:bold; text-align:center; ">Main 1</td>';
					echo '<td style="border-bottom: 2px solid #000; min-width:150px; font-weight:bold; text-align:center; border-right:2px solid #000;">Main 2</td>';					
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
					echo '<td style="border-bottom: 2px solid #000; min-width:150px; font-weight:bold; text-align:center; ">Main 1</td>';
					echo '<td style="border-bottom: 2px solid #000; min-width:150px; font-weight:bold; text-align:center; border-right:2px solid #000;">Main 2</td>';
					echo '<td style="border-bottom: 2px solid #000; min-width:150px; font-weight:bold; text-align:center; ">Main 1</td>';
					echo '<td style="border-bottom: 2px solid #000; min-width:150px; font-weight:bold; text-align:center; border-right:2px solid #000;">Main 2</td>';
					echo '<td style="border-bottom: 2px solid #000; min-width:150px; font-weight:bold; text-align:center; ">Main 1</td>';
					echo '<td style="border-bottom: 2px solid #000; min-width:150px; font-weight:bold; text-align:center; border-right:2px solid #000;">Main 2</td>';					
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
					$g .= $year = portion(4);
					$g .= ' - ';
					$g .= month_name($month = portion(5));

					echo $g;

					?>

					
					
					</th>				
				</tr>
				<tr valign="bottom">
					<th rowspan="3" style="border:2px solid black;" class="sorter-false">No.</th>
					<th rowspan="3" style="border:2px solid black;" class="">Metering Points</th>
					<th colspan="3" style="border:2px solid black;" class="sorter-false">Main Meter - Current Readings 
					
					</th>					
					<th colspan="3" style="border:2px solid black;color:red;" class="sorter-false">Main Meter - Previous Readings</th>
					<?php if($customer_id != 2019){ ?>
					<th colspan="3" style="border:2px solid black;" class="sorter-false">Main Meter - Current Readings</th>
					<th colspan="3" style="border:2px solid black;color:red;" class="sorter-false">Main Meter - Previous Readings</th>
					<?php } ?>										
					<?php if($customer_id != 2019){ ?>
					<th style="border:2px solid black;" colspan="8" style="border:2px solid black;" class="sorter-false">Advance</th>
					<?php } else { ?>
					<th style="border:2px solid black;" colspan="4" style="border:2px solid black;" class="sorter-false">Advance</th>
					<?php } ?>

				</tr>
				<tr class="tablesorter-ignoreRow">
					<th style="border:2px solid black;" colspan="3">Imports</th>
					<th style="border:2px solid black; color:red;" colspan="3">Imports</th>
					<?php if($customer_id != 2019){ ?>
					<th style="border:2px solid black;" colspan="3">Exports</th>
					<th style="border:2px solid black; color:red;" colspan="3">Exports</th>
					<?php } ?>
					<th style="border:2px solid black;" colspan="4">Imports</th>
					<?php if($customer_id != 2019){ ?>
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

					<?php if($customer_id != 2019){ ?>
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

				$co = 0;
				$t = new Db();
				$sel = $t->select("SELECT DISTINCT mp_region_id FROM metering_point, r_reading WHERE mp_customer_id = '$customer_id' AND rea_cus_id = '$customer_id' AND mp_id = rea_mp_id AND mp_region_id IS NOT NULL");

				$n = 1;
				foreach($sel[0] as $r){
				extract($r);
				$tc1 = $tc2 = $tc3 = $tc4= $tc5= $tc6= $tc7= $tc8 = 0;
				if(!$t->num_rows()){
					continue;
				}

				$total_mp;
				$db = new Db();

				if($customer_id == 2019){
					$select = $db->select("SELECT * FROM metering_point, r_reading WHERE mp_customer_id = '$customer_id' AND rea_cus_id = '$customer_id' AND mp_id = rea_mp_id AND mp_region_id = '$mp_region_id'");
				}else{
					$select = $db->select("SELECT * FROM metering_point, r_reading WHERE mp_customer_id = '$customer_id' AND rea_cus_id = '$customer_id' AND mp_id = rea_mp_id AND mp_region_id = '$mp_region_id'");
				}

				echo '</thead>';
				echo '<tbody>';

				if($customer_id != 2019){
					// echo '<tr>
					// <td colspan="19" style="border-left:none; border-right:none; border-top:2px solid #000; border-bottom: 2px solid #000;color:red;"><b>'.strtoupper($this->rgf("meter_region", $mp_region_id, "region_id", "region_name")).'</b></td>
					// </tr>';
				}else{
					echo '<tr>
					<td colspan="12" style="border-left:none; border-right:none; border-top:2px solid #000; border-bottom: 2px solid #000;color:red;"><b>'.strtoupper($this->rgf("meter_region", $mp_region_id, "region_id", "region_name")).'</b></td>
					</tr>';
				}
					
				$db_values[] = array('', '&nbsp;<br/><b>'.strtoupper($this->rgf("meter_region", $mp_region_id, "region_id", "region_name")).'</b>');
				foreach($select[0] as $row){
					extract($row);
					$j++;
					
					if(date('Y', $rea_date)==$year && date('m', $rea_date)==$month){
						//selecting rates
						
						$db = new Db();
						$sel = $db->select("SELECT * FROM r_rate WHERE rate_reading_id = '$rea_id'");
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

							if(!$this->isLocked($customer_id, $year, $month)){
							echo '<div class="add-hint" title="Edit '.$mp_location.' Readings" id="add-hint'.$n.'" style="color:white;"><i class="fa fa-fw fa-pencil"></i></div>';
							}

							?>

							<div id="myModal<?php echo $n; ?>" class="net-advance-form modal">
								<div class="modal-content" style="width:40%;">
									<span id="close<?php echo $n; ?>" class="close" style="color:red;">&times;</span>
									<h5>Edit Reading for: <span class="text-primary"><?php echo $mp_location; ?></h5>
									<?php
										//$select = $db->select("SELECT * FROM rate"); 

										$is = $this->decimalFixer(@$rate_import_wh_1/1000);
										$ip = $this->decimalFixer(@$rate_import_wh_2/1000);
										$io = $this->decimalFixer(@$rate_import_wh_3/1000);
										$es = $this->decimalFixer(@$rate_export_wh_4/1000);
										$ep = $this->decimalFixer(@$rate_export_wh_5/1000);
										$eo = $this->decimalFixer(@$rate_export_wh_6/1000);

										if(isset($_POST['save'.$mp_id])){
											$is = $_POST['is'];
											$ip = $_POST['ip'];
											$io = $_POST['io'];
											$es = $_POST['es'];
											$ep = $_POST['ep'];
											$eo = $_POST['eo'];

											$advance = $this->rgf("metering_point", $mp_id, "mp_id", "mp_advance");
											$swap = ($advance == 2)?1:0;
											//rate_reading_id = '$rea_id'
											$update = $db->update("r_rate", [
												"rate_import_wh_1"=>str_replace(',', '', $is)*1000,
												"rate_import_wh_2"=>str_replace(',', '', $ip)*1000,
												"rate_import_wh_3"=>str_replace(',', '', $io)*1000,
												"rate_export_wh_4"=>str_replace(',', '', $es)*1000,
												"rate_export_wh_5"=>str_replace(',', '', $ep)*1000,
												"rate_export_wh_6"=>str_replace(',', '', $eo)*1000,
												"rate_swap"=>$swap,
											], 
											["rate_reading_id"=>$rea_id]
										);

											FeedBack::redirect(return_url().'services/schedule/'.$customer_id.'/'.$year.'/'.$month);
										
										}

										echo '<form action="" method="post">';

										echo '<br/><br/>Advance Imports: <b>(KWh)</b><br/>';
										echo '<input class="number" style="width:30%; padding:1%; margin:1%;" placeholder="Shoulder" name="is" autocomplete="off" value="'.$is.'"/>';
										echo '<input class="number" style="width:30%; padding:1%; margin:1%;" placeholder="Peak" name="ip" autocomplete="off" value="'.$ip.'"/>';
										echo '<input class="number" style="width:30%; padding:1%; margin:1%;" placeholder="Off Peak" name="io" autocomplete="off" value="'.$io.'"/>';

										echo '<br/>Advance Exports: <b>(KWh)</b><br/>';
										echo '<input class="number" style="width:30%; padding:1%; margin:1%;" placeholder="Shoulder" name="es" autocomplete="off" value="'.$es.'"/>';
										echo '<input class="number" style="width:30%; padding:1%; margin:1%;" placeholder="Peak" name="ep" autocomplete="off" value="'.$ep.'" />';
										echo '<input class="number" style="width:30%; padding:1%; margin:1%;" placeholder="Off Peak" name="eo" autocomplete="off" value="'.$eo.'" />';

										echo '<br/><br/><input class="number" type="submit" class="btn btn-danger btn-block" value="Save" name="save'.$mp_id.'">';

										echo '</form>';

									?>
									</span></h5>
								</div>
							</div>
							<script>
								document.getElementById("<?php echo 'add-hint'.$n; ?>").onclick = function() {
								  document.getElementById("<?php echo 'myModal'.$n; ?>").style.display = "block";
								}
								document.getElementById("<?php echo 'close'.$n; ?>").onclick = function() {
								  document.getElementById("<?php echo 'myModal'.$n; ?>").style.display = "none";
								}
								window.onclick = function(event) {
								  if (event.target == document.getElementById("<?php echo 'myModal'.$n; ?>")) {
								    document.getElementById("<?php echo 'myModal'.$n; ?>").style.display = "none";
								  }
								}
							</script>
							<?php


							echo '</td>';

							echo '<td style="'.$df.' text-align:right;">'.number_format(@$rate_import_wh_1*$units,2).'</td>';
							echo '<td style="'.$df.' text-align:right;">'.number_format(@$rate_import_wh_2*$units,2).'</td>';
							echo '<td style="'.$df.'border-right:2px solid black; text-align:right;">'.number_format(@$rate_import_wh_3*$units,2).'</td>';


							echo '<td style="'.$df.' text-align:right;color:red;">'.number_format(@$cal[12]*$units,2).'</td>';
							echo '<td style="'.$df.' text-align:right;color:red;">'.number_format(@$cal[13]*$units,2).'</td>';
							echo '<td style="'.$df.'border-right:2px solid black; text-align:right;color:red;">'.number_format(@$cal[14]*$units,2).'</td>';

							if($customer_id != 2019){
								echo '<td style="'.$df.' text-align:right;">'.number_format(@$rate_export_wh_4*$units,2).'</td>';
								echo '<td style="'.$df.' text-align:right;">'.number_format(@$rate_export_wh_5*$units,2).'</td>';
								echo '<td style="'.$df.' text-align:right;">'.number_format(@$rate_export_wh_6*$units,2).'</td>';	
							
								echo '<td style="'.$df.'border-left:2px solid black; text-align:right;color:red;">'.number_format(@$cal[15]*$units,2).'</td>';
								echo '<td style="'.$df.' text-align:right;color:red;">'.number_format(@$cal[16]*$units,2).'</td>';
								echo '<td style="'.$df.'border-right:2px solid black; text-align:right;color:red;">'.number_format(@$cal[17]*$units,2).'</td>';
							
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


							echo '<td style="'.$df.'border-left:2px solid black; border-bottom:2px solid black;  text-align:right;">'.number_format(@$rate_import_wh_1*$units,2).'</td>';
							echo '<td style="'.$df.'border-bottom:2px solid black;  text-align:right;">'.number_format(@$rate_import_wh_2*$units,2).'</td>';
							echo '<td style="'.$df.'border-bottom:2px solid black;  text-align:right;">'.number_format(@$rate_import_wh_3*$units,2).'</td>';



							echo '<td style="'.$df.'border-left:2px solid black; border-bottom:2px solid black;  text-align:right;color:red;">'.number_format(@$cal[12]*$units,2).'</td>';
							echo '<td style="'.$df.'border-bottom:2px solid black;  text-align:right;color:red;">'.number_format(@$cal[13]*$units,2).'</td>';
							echo '<td style="'.$df.'border-bottom:2px solid black;  text-align:right;color:red;">'.number_format(@$cal[14]*$units,2).'</td>';


							if($customer_id != 2019){
								echo '<td style="'.$df.'border-left:2px solid black; border-bottom:2px solid black;  text-align:right;">'.number_format(@$rate_export_wh_4*$units,2).'</td>';
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

				//if($count == $total_mp+1)
				{
					$l = $this->rateSum($customer_id, $year, $month);

					//$tc1 = $l[0];
					//$tc2 = $l[1];
					//$tc3 = $l[2];
					//$tc4 = $l[3];
					//$tc5 = $l[4];
					//$tc6 = $l[5];
					//$tc7 = $l[6];
					//$tc8 = $l[7];
					if($customer_id == 2019){
					echo '<tr>';
					echo '<td style="border-top:2px solid black; border-top:2px solid #000;border-bottom:2px solid #000;border-left:2px solid #000;"></td>';

					if($customer_id != 2019){
						echo '<td colspan="13" style="border-top:2px solid black; border-left:2px solid black; border-bottom:2px solid black; border-right:2px solid black; text-align:right;font-weight: bold; ">Total</td>';
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

				}
				if(1){

					$l = $this->rateSum($customer_id, $year, $month);

					$tc1 = $l[0];
					$tc2 = $l[1];
					$tc3 = $l[2];
					$tc4 = $l[3];
					$tc5 = $l[4];
					$tc6 = $l[5];
					$tc7 = $l[6];
					$tc8 = $l[7];
					echo '<tr>';
					echo '<td style="border-top:2px solid black; border-top:2px solid #000;border-bottom:2px solid #000;border-left:2px solid #000;"></td>';

					if($customer_id != 2019){
						echo '<td colspan="13" style="border-top:2px solid black; border-left:2px solid black; border-bottom:2px solid black; border-right:2px solid black; text-align:right;font-weight: bold; ">Total</td>';
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
			echo '<title>'.strtoupper($customer_short_name.' Meter Readings').' - '.($month = portion(5)).' '.portion(4).'</title>';
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
					$g .= month_name($month = portion(5));

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
					$l = $this->rateSum($customer_id, $year, $month);

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
		if($n == 2){
			return '<span class="pull-right" style="color:red; font-weight:bold;"><strong>(S)</strong></span>';
		}else{
			return "";
		}
	}

	public function readingsAction(){

		$id = portion(3);
		$year = portion(4);
		$month = portion(5);
		$note = portion(6);

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

	public function generatorBillAndAmount(){

		$id = portion(3);
		$year = portion(4);
		$month = portion(5);
		$note = portion(6);

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
		
		if($id == 2029){ // tanseco
			if(isset($_POST['saveValues'])){
				$price = $_POST['price'];
				$price2 = $_POST['price2'];
				$price3 = $_POST['price3'];
				$units = $_POST['units'];
				$generator = $_POST['generator'];
				$currency = $_POST['currency'];

				// print_r($price);
				// echo '<br/>';echo '<br/>';
				// print_r($price2);
				// echo '<br/>';echo '<br/>';
				// print_r($price3);

				$loss_factor = str_replace(',', '', $_POST['loss_factor']);
				$dollars = str_replace(',', '', $_POST['dollars']);
				$euros = str_replace(',', '', $_POST['euros']);

				$db = new Db();

				$sel = $db->select("SELECT ex_id FROM exchange_rate WHERE ex_year = '$year' AND ex_month = '$month' ");

				if($db->num_rows()){
					extract($sel[0][0]);
					$update = $db->update("exchange_rate", [
						"ex_usd"=>$dollars,
						"ex_euro"=>$euros,
						"ex_loss_factor"=>$loss_factor,
						"ex_added_by"=>user_id(),
						"ex_date_added"=>time(),
						"ex_month"=>$month,
						"ex_year"=>$year,
					], ["ex_id"=>$ex_id]);

					echo $db->error();
				}else{
					$insert = $db->insert("exchange_rate", [
						"ex_usd"=>$dollars,
						"ex_euro"=>$euros,
						"ex_loss_factor"=>$loss_factor,
						"ex_added_by"=>user_id(),
						"ex_date_added"=>time(),
						"ex_month"=>$month,
						"ex_year"=>$year,
					]);
				}

				echo $db->error();
				
				for($i=0; $i<count($generator); $i++){

					$g = $generator[$i];
					$u = str_replace(',', '', $units[$i])*1000;
					$p = str_replace(',', '', $price[$i])*1000;
					$p2 = str_replace(',', '', $price2[$i])*1000;
					$p3 = str_replace(',', '', $price3[$i])*1000;
					$c = $currency[$i];

					$select = $db->select("SELECT mix_id FROM mix_price WHERE mix_generator_id = '$g' AND mix_year = '$year' AND mix_month = '$month'");
					
					if($db->num_rows()){
						extract($select[0][0]);
						$update = $db->update("mix_price", [
							"mix_generator_id"=>$g,
							"mix_units"=>$u,
							"mix_amount"=>$p,
							"mix_amount2"=>$p2,
							"mix_amount3"=>$p3,
							"mix_date_added"=>time(),
							"mix_added_by"=>user_id(),
							"mix_month"=>$month,
							"mix_year"=>$year,
							"mix_currency"=>$c,
						], ["mix_id"=>$mix_id]);

					}else{
						$insert = $db->insert("mix_price", [
							"mix_generator_id"=>$g,
							"mix_units"=>$u,
							"mix_amount"=>$p,
							"mix_amount2"=>$p2,
							"mix_amount3"=>$p3,
							"mix_date_added"=>time(),
							"mix_added_by"=>user_id(),
							"mix_month"=>$month,
							"mix_year"=>$year,
							"mix_currency"=>$c,
						]);
					}

					if($db->error()){
						Feedback::error($db->error());
					}
				}
			}

			$select = $db->select("SELECT * FROM exchange_rate WHERE ex_year = '$year' AND ex_month = '$month'");
			extract($select[0][0]);

			if(empty($dollars)){
				$dollars = $this->decimalFixer($ex_usd);
			}
			if(empty($euro)){
				$euros = $this->decimalFixer($ex_euro);
			}
			if(empty($loss_factor)){
				$loss_factor = $this->decimalFixer($ex_loss_factor);
			}
			echo '<form action="" method="post">';
			//echo '<h4>Exchange Rate Applied</h4>';
			echo '<br/>';
			echo '<div style="padding-right:40px; float:left;"><b>Dollars</b></b>: <input style="width:100px;" type="text"  value="'.$dollars.'" name="dollars" class="number"/></div>';
			echo '<div style="padding-right:40px; float:left;"><b>Euros</b>: <input style="width:100px;" type="text" value="'.$euros.'"  name="euros" class="number"/></div>';
			echo '<div style="padding-right:40px; float:left;">Transmission Loss Factor <b>(Ref ERA)</b>: <input type="text" value="'.$loss_factor.'"  name="loss_factor" class="number" style="width:100px;"/></div>';
			echo '<div style="padding-right:40px; float:left;"><input type="submit" name="saveValues" value="Save"/></div>';


			echo '<style>input{ background-color: #f3f3f394; }</style>';

			echo '<div style="clear:both;"><br/></div>';
			echo '<h4>Generators</h4>';

			echo '<table id="sortSpan" border="1" cellpadding="2">';
			echo '<thead>';
			echo '<tr style="background-color:white;">';
			echo '<th class="text-center" style="margin-top:180px;">No.</th>';
			echo '<th class="text-center">Customer Name</th>';
			echo '<th class="text-center">Bill Units<br/>(kWh)</th>';
			echo '<th class="text-center">Bill Amount(USD)</th>';
			echo '<th class="text-center">Bill Amount(EUR)</th>';
			echo '<th class="text-center">Bill Amount(UGX)</th>';
			echo '</tr>';
			echo '</thead>';
			echo '<tbody>';

			$no=1;
			$select = $db->select("SELECT * FROM generators ORDER BY ge_name ASC ");
			foreach($select[0] as $row){
				extract($row);

				$sel = $db->select("SELECT * FROM mix_price WHERE mix_generator_id = '$ge_id' AND mix_year = '$year' AND mix_month = '$month'");
				extract($sel[0][0]);

				if($units[$i]){
					$mix_units = ($units[$i]);
				}
				if($price[$i]){
					$mix_amount = ($price[$i]);
				}

				if($price2[$i]){
					$mix_amount2 = ($price2[$i]);
				}

				if($price3[$i]){
					$mix_amount3 = ($price3[$i]);
				}

				$mix_units /= (pow(1000,1));
				$mix_amount /= (pow(1000,1));
				$mix_amount2 /= (pow(1000,1));
				$mix_amount3 /= (pow(1000,1));

				$mix_units = $this->decimalFixer($mix_units);
				$mix_amount = $this->decimalFixer($mix_amount);
				$mix_amount2 = $this->decimalFixer($mix_amount2);
				$mix_amount3 = $this->decimalFixer($mix_amount3);

				echo '<input type="hidden" value="'.$ge_id.'" name="generator[]"/>';
				echo '<input type="hidden" value="'.$ge_currency.'" name="currency[]"/>';

				echo '<tr>';
				echo '<td>'.($no++).'.</td>';
				echo '<td>'.$ge_name.'</td>';
				echo '<td><input type="text" value="'.$mix_units.'" autocomplete="off" name="units[]" class="number text-right"/></td>';

				if($ge_id == 4){
					$u = " type='text'  placeholder='In USD' ";
					$e = " type='hidden' ";
					$x = " type='text'   placeholder='In UGX' ";
				}elseif($ge_id == 10){
					$u = " type='text'  placeholder='In USD' ";
					$e = " type='text'  placeholder='In EUR' ";
					$x = " type='hidden' ";
				}elseif($ge_currency == "UGX"){
					$u = " type='hidden'  placeholder='In USD' ";
					$e = " type='hidden'  placeholder='In EUR' ";
					$x = " type='text'  placeholder='In EUR' ";
				}elseif($ge_currency == "EUR"){
					$u = " type='hidden'  placeholder='In USD' ";
					$e = " type='text'  placeholder='In EUR' ";
					$x = " type='hidden'  placeholder='In EUR' ";
				}elseif($ge_currency == "USD"){
					$u = " type='text'  placeholder='In USD' ";
					$e = " type='hidden'  placeholder='In EUR' ";
					$x = " type='hidden'  placeholder='In EUR' ";
				}else{
					$u = ' type="text" placeholder="In USD" ';
					$e = ' type="hidden" ';
					$x = ' type="hidden" ';
				}

				echo '<td><input '.$u.' value="'.$mix_amount.'" autocomplete="off" name="price[]" class="number text-right"/></td>';

				echo '<td><input '.$e.' value="'.$mix_amount2.'" autocomplete="off" name="price2[]" class="number text-right"/></td>';

				echo '<td><input '.$x.' value="'.$mix_amount3.'" autocomplete="off" name="price3[]" class="number text-right"/></td>';

				echo '</tr>';
			}

			echo '</tbody>';
			echo '</table>';

			echo '</form>';
		}
	}

	public function missingPoint(){
		$customer_id = portion(3);
		$year = portion(4);
		$month = portion(5);

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

		echo '<a href="'.return_url().'services/missing-point/'.$id.'/'.$year.'/'.$month.'"><i class="fa fa-fw fa-plus"></i> Attach Missing Metering Point</a>';

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
			
			Expiry Date <input disabled title="<?php echo $d; ?>" <?php echo $uu; ?> type="date" id="expiryDate" value="<?php echo $d; ?>" style="text-align:right; padding:0 2px; width:130px; margin-bottom: 10px;" name="search" autocomplete="off" class="">
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
								echo '<td style="border-right:2px solid black;"><span class="btn-xs btn-danger clear-row" onclick="clearRow(\''.$j.'\')">CV</span></td>';							
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

		$iDC = $this->isDNorCN($id, $year, $month);
		if($iDC > 0){
			$not = "Debit Note";
		}else{
			$not = "Credit Note";
		}

		if($this->isLocked($id, $year, $month)){
			echo $e = "readonly";
		}else{
			echo $e = "";
		}

		echo '<div class="clearfix"></div>';

		echo '<h4><span class="badge"> '.$not.' </span> Readings in: <b>KWh</b></h4>';
		
		?>
		<div id="printMe">
			<style>table tr td, table tr th{ height: 10px !important;} table tr th{ text-align: center;} table tr:hover td{ background-color: #efefff; } table tr td input{background-color: inherit;} table tr td input:hover, table tr td input:active, table tr td input:focus{background-color: #ccc;}</style>
			
			<?php 
			$db = new Db();
			$select = $db->select("SELECT cn_id, cn_peak, cn_off_peak, cn_shoulder, cn_details FROM credit_note WHERE cn_year='$year' AND cn_month='$month' AND cn_customer_id='$id'");
			extract($select[0][0]);


			?>

			<div class="pull-left" style="display:none; padding:2px;"><b>Peak</b><br/><input type="text" value="<?php echo $cn_peak; ?>" class="number" style="text-align:right; width:80px;" id="peak"></div>
			<div class="pull-left" style="display:none; padding:2px;"><b>Shoulder</b><br/><input type="text" value="<?php echo $cn_shoulder; ?>" class="number" style="text-align:right; width:80px;" id="shoulder"></div>
			<div class="pull-left" style="display:none; padding:2px;"><b>Off Peak</b><br/><input type="text" value="<?php echo $cn_off_peak; ?>" class="number" style="text-align:right; width:80px;" id="offPeak"></div>
			<input type="hidden" id="peak" value="">
			<input type="hidden" id="shoulder" value="">
			<input type="hidden" id="off_peak" value="">
			
			<div class="pull-left" style="padding:2px;"><b>Details</b><br/>
				<textarea <?php echo $e; ?> style="width:860px;" id="noteDetails"><?php echo $cn_details; ?></textarea></div>
			<div class="clearfix"></div>


			<input type="text" style="padding:0 10px; margin-bottom: 10px;" name="search" autocomplete="off" placeholder="Search Metering Point" id="searchTerm" onkeyup="doSearch()" onClick="this.select();">
			<?php if(!$e){ ?>
			<span id="saveID" onclick="return saveBtnCN('<?php echo return_url(); ?>');" style="margin-left:595px;margin-bottom: 10px;" class="btn-primary btn-sm"><i class="fa fa-fw fa-save"></i> Save</span>
			<?php }else{
				echo '<span class="" style="margin-left:595px;margin-bottom: 10px;background-color:red;color:#FFF;padding: 5px;font-size:1em;"><i class="fa fa-lock fa-fx"></i> Locked</span>';
			} ?>
			<table id="dataTable" border="1" cellspacing="0" cellpadding="2" class="table schedule" style="font-size: 12px; width:300px !important; margin:auto;">
				<thead>	
											
				<tr>
					<th style="width:20px;border-top:2px solid black;  border-left:2px solid black;border-bottom:2px solid black;">No.</th>
					<th style="min-width:150px; border-top:2px solid black; border-left:2px solid black;border-bottom:2px solid black;">Metering Point.</th>
					<th style="min-width:80px; border-top:2px solid black; border-left:2px solid black;border-bottom:2px solid black;">Imports<br/>Shoulder <br/>( Rate 1) </th>
					<th style="min-width:80px; border-top:2px solid black; border-bottom:2px solid black;">Imports<br/>Peak <br/>( Rate 2) </th>
					<th style="min-width:80px; border-top:2px solid black; border-bottom:2px solid black;">Imports<br/>Off Peak <br/>( Rate 3) </th>

					<th style="min-width:80px; border-top:2px solid black; border-left:2px solid black;border-bottom:2px solid black;color:black;">Exports<br/>Shoulder <br/>( Rate 4) </th>
					<th style="min-width:80px; border-top:2px solid black; border-bottom:2px solid black;color:black;">Exports<br/>Peak <br/>( Rate 5) </th>
					<th style="min-width:80px; border-top:2px solid black; border-right:2px solid black; border-bottom:2px solid black;color:black;">Exports<br/>Off Peak <br/>( Rate 6) </th>
					<th style="border-top:2px solid black; border-right:2px solid black; border-bottom:2px solid black;color:black;">
					<?php
					if($e){
						echo '<i class="fa fa-fw fa-lock" style="color:red"></i>';
					}else{
						echo '<span class="btn-danger btn-xs clear-all" onclick="return clearAll();">CA</span>';
					}
					?>
					</th>
				</tr>
			</thead>
			<tbody>
				<?php 
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

				$total_mp;
				$db = new Db();
				$select = $db->select("SELECT * FROM metering_point, r_reading_cn WHERE mp_customer_id = '$customer_id' AND n_rea_cus_id = '$customer_id' AND mp_id = n_rea_mp_id");
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
					if(date('Y', $n_rea_date)==$year && date('m', $n_rea_date)==$month){
						//selecting rates
						
						$db = new Db();
						$sel = $db->select("SELECT * FROM r_rate_cn WHERE n_rate_reading_id = '$n_rea_id'");
						@extract($sel[0][0]);
						
						//calling the function
						$cal = rate_calculate($customer_id, ($n_rea_date), $mp_id);

						$j++;

						if($j != $total_mp){							


							echo '<tr>';

							echo '<td style="border-left:2px solid black; ">'.$count.'.</td>';
							echo '<td style="border-left:2px solid black; border-right:2px solid black;">'.strtoupper(strtolower($mp_location)).$this->mpAdvance($mp_advance).'<input '.$e.' type="hidden" id="mp'.$j.'" value="'.$mp_location.'"/></td>';
							
							if($j==1){
								echo '<td style=" text-align:right;"><input '.$e.' id="id'.($j).($v).'" onClick="this.select();" onfocus="return saveReadingCN(\''.return_url().'\',\''.($j).($v++).'\');" type="text" class="number" value="'.$this->decimalFixer(@$n_rate_import_wh_1*$units).'" style="text-align:right; width:100%; padding:0 2px; margin:0;border:none;" /></td>';
							}else{
								echo '<td style=" text-align:right;"><input '.$e.' id="id'.($j).($v).'" onClick="this.select();" onfocus="return saveReadingCN(\''.return_url().'\',\''.($j).($v++).'\');" type="text" class="number" value="'.$this->decimalFixer(@$n_rate_import_wh_1*$units).'" style="text-align:right; width:100%; padding:0 2px; margin:0;border:none;" /></td>';					
							}

							echo '<td style=" text-align:right;"><input '.$e.' id="id'.($j).($v).'" onClick="this.select();" onfocus="return saveReadingCN(\''.return_url().'\',\''.($j).($v++).'\');" type="text" class="number" value="'.$this->decimalFixer(@$n_rate_import_wh_2*$units).'" style="text-align:right; width:100%; padding:0 2px; margin:0;border:none;" /></td>';

							echo '<td style="border-right:2px solid black; text-align:right;"><input '.$e.' id="id'.($j).($v).'" onClick="this.select();" onfocus="return saveReadingCN(\''.return_url().'\',\''.($j).($v++).'\');" type="text" class="number" value="'.$this->decimalFixer(@$n_rate_import_wh_3*$units).'" style="text-align:right; width:100%; padding:0 2px; margin:0;border:none;" /></td>';

							echo '<td style=" text-align:right;"><input '.$e.' id="id'.($j).($v).'" onClick="this.select();" onfocus="return saveReadingCN(\''.return_url().'\',\''.($j).($v++).'\');" type="text" class="number" value="'.$this->decimalFixer(@$n_rate_export_wh_4*$units).'" style="text-align:right; width:100%; padding:0 2px; margin:0;border:none;" /></td>';
							echo '<td style=" text-align:right;"><input '.$e.' id="id'.($j).($v).'" onClick="this.select();" onfocus="return saveReadingCN(\''.return_url().'\',\''.($j).($v++).'\');" type="text" class="number" value="'.$this->decimalFixer(@$n_rate_export_wh_5*$units).'" style="text-align:right; width:100%; padding:0 2px; margin:0;border:none;" /></td>';
							echo '<td style="border-right:2px solid black; text-align:right;"><input '.$e.' id="id'.($j).($v).'" onClick="this.select();" onfocus="return saveReadingCN(\''.return_url().'\',\''.($j).($v++).'\');" type="text" class="number" value="'.$this->decimalFixer(@$n_rate_export_wh_6*$units).'" style="text-align:right; width:100%; padding:0 2px; margin:0;border:none;" /></td>';
							echo '<td style="border-right:2px solid black;">';								
							if($e){
								echo '<i class="fa fa-fw fa-lock" style="color:red"></i>';
							}else{
								echo '<span class="btn-danger btn-xs clear-row" onclick="clearRow(\''.$j.'\')">CV</span>';
							}
							echo '</td>';
											
							echo '</tr>';
						}else{		

							echo '<tr>';

							echo '<td style="border-bottom:2px solid black; border-left:2px solid black; ">'.$count.'.</td>';
							echo '<td style="border-bottom:2px solid black; border-left:2px solid black; border-right:2px solid black;">'.strtoupper(strtolower($mp_location)).$this->mpAdvance($mp_advance).'<input type="hidden" id="mp'.$j.'" value="'.$mp_location.'"/></td>';
							
							if($j==1){
								echo '<td style="border-bottom:2px solid black;  text-align:right;"><input '.$e.' id="id'.($j).($v).'" onClick="this.select();" onfocus="return saveReadingCN(\''.return_url().'\',\''.($j).($v++).'\');" type="text" class="number" value="'.$this->decimalFixer(@$rate_import_wh_1*$units).'" style="text-align:right; width:100%; padding:0 2px; margin:0;border:none;" /></td>';
							}else{
								echo '<td style="border-bottom:2px solid black;  text-align:right;"><input '.$e.' id="id'.($j).($v).'" onClick="this.select();" onfocus="return saveReadingCN(\''.return_url().'\',\''.($j).($v++).'\');" type="text" class="number" value="'.$this->decimalFixer(@$rate_import_wh_1*$units).'" style="text-align:right; width:100%; padding:0 2px; margin:0;border:none;" /></td>';					
							}

							echo '<td style="border-bottom:2px solid black;  text-align:right;"><input '.$e.' id="id'.($j).($v).'" onClick="this.select();" onfocus="return saveReadingCN(\''.return_url().'\',\''.($j).($v++).'\');" type="text" class="number" value="'.$this->decimalFixer(@$rate_import_wh_2*$units).'" style="text-align:right; width:100%; padding:0 2px; margin:0;border:none;" /></td>';

							echo '<td style="border-bottom:2px solid black; border-right:2px solid black; text-align:right;"><input '.$e.' id="id'.($j).($v).'" onClick="this.select();" onfocus="return saveReadingCN(\''.return_url().'\',\''.($j).($v++).'\');" type="text" class="number" value="'.$this->decimalFixer(@$rate_import_wh_3*$units).'" style="text-align:right; width:100%; padding:0 2px; margin:0;border:none;" /></td>';

							echo '<td style="border-bottom:2px solid black;  text-align:right;"><input '.$e.' id="id'.($j).($v).'" onClick="this.select();" onfocus="return saveReadingCN(\''.return_url().'\',\''.($j).($v++).'\');" type="text" class="number" value="'.$this->decimalFixer(@$rate_export_wh_4*$units).'" style="text-align:right; width:100%; padding:0 2px; margin:0;border:none;" /></td>';
							echo '<td style="border-bottom:2px solid black;  text-align:right;"><input '.$e.' id="id'.($j).($v).'" onClick="this.select();" onfocus="return saveReadingCN(\''.return_url().'\',\''.($j).($v++).'\');" type="text" class="number" value="'.$this->decimalFixer(@$rate_export_wh_5*$units).'" style="text-align:right; width:100%; padding:0 2px; margin:0;border:none;" /></td>';
							echo '<td style="border-bottom:2px solid black; border-right:2px solid black; text-align:right;"><input '.$e.' id="id'.($j).($v).'" onClick="this.select();" onfocus="return saveReadingCN(\''.return_url().'\',\''.($j).($v++).'\');" type="text" class="number" value="'.$this->decimalFixer(@$rate_export_wh_6*$units).'" style="text-align:right; width:100%; padding:0 2px; margin:0;border:none;" /></td>';
							echo '<td style="border-bottom:2px solid black; border-right:2px solid black;">';
							if($e){
								echo '<i class="fa fa-fw fa-lock" style="color:red"></i>';
							}else{
								echo '<span class="btn-danger btn-xs clear-row" onclick="clearRow(\''.$j.'\')">CV</span>';
							}						
							echo '</td>';
											
							echo '</tr>';							

						}


						$count++;	

					}					
				}	

				echo '<input type="hidden" value="'.$j.'" id="allRows"/>';			
				?>
			</tbody></table>

		</div>
	
	</div>
	<?php
	}

	public function decimalFixer($number){
		$num = explode('.', $number);

		if(empty($num[1])){
			return number_format($num[0]);
		}else{			
			return number_format($num[0]).'.'.$num[1];
		}
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
				print_r($this->rateSum($customer_id, $year, $month));
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
								<td colspan="2">Total Amount(Energy)</td>
								<td align="right"><?php echo number_format($ttu); ?></td>
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
								<td colspan="2">Total Amount(Energy)</td>
								<td align="right"><?php echo number_format($ttu); ?></td>
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
		foreach($select[0] as $row){
			extract($row);
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
				$sel = $d->select("SELECT * FROM done_levels WHERE dl_year = '$year' AND dl_month = '$month' AND dl_app$tc = 1 AND dl_app$tcn = 1 AND dl_customer = '$customer'");
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
					
					$sel = $d->select("SELECT * FROM all_readings_comments WHERE arc_year = '$year' AND arc_month = '$month' AND arc_part = '$tc' AND arc_customer = '$customer' ");
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
					$sel = $d->select("SELECT * FROM done_levels WHERE dl_year = '$year' AND dl_month = '$month' AND dl_customer = '$customer' AND dl_app$tcn IS NOT NULL");
					$tot = $d->num_rows();
				}

				if($tot){
					extract($sel[0][0]);
					if($invoice == 0 && (($tc == 1 && user_id() == static_generator())|| ${"dl_selected".$tc} == $this->rgf("sysuser", user_id(), "user_id", "user_designation"))){						
				
						echo '<form action="" method="post" style="background-color: #fbe2e2;padding:10px; border:1px solid #000; ">';
						

						if(isset($_POST['approve']) && $_POST['part']== $tc){
							echo '';
							$ind = array();
							$comment = $_POST['comment'];
							$part = $_POST['part'];
							$parc_id = $_POST['parc_id'];

							$customername = $this->rgf("customer", $customer, "customer_id", "customer_short_name");

							if($tc == 1){

								$this->assignInvoiceNumber($year, $month);

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

								$message .= "<br/><br/>The invoices for <b>".$customername. " $year-".month_name($month)."</b> are ready for approval.<br/>";
								$message .= "\r\nYou can use this link: <a href='$link'>$link</a>";	
								$message .= $ms;

							    $subject = "Energy Invoice";
							    Feedback::sendmail($to,$subject,$message,$name);
							}else{

								if($tc == 2){
									$next = $this->next($tc, $year, $month);
									$to = $next['email'];
									$message = "Hello ".$next['fname'].",";
									$message .= "<br/><br/>The invoice for <b>$customername $year-".month_name($month)."</b> are ready for approval.<br/>";
								}elseif($tc == 3){
									$next = $this->next($tc, $year, $month);
									$to = $next['email'];
									$message = "Hello ".$next['fname'].",";	
									$message .= "<br/><br/>The invoice for <b>$customername $year-".month_name($month)."</b> are Successfully approved.<br/>";						
								}
									
								
								$message .= "\r\nYou can use this link: <a href='$link'>$link</a>";	

								$message .= $ms;
							    $subject = "Energy Invoice Approval";
							    Feedback::sendmail($to,$subject,$message,$name);

								$insert = $d->update("done_levels", $ind, ["dl_id"=>$parc_id]);
							}

							FeedBack::redirect(return_url().portion(1)."/".portion(2)."/".portion(3)."/".portion(4)."/".portion(5));
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

						echo '</form>';

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
							
	}

	public function getAssignedInvoiceNumber($customer, $year, $month){
		$j = new Db();
		$customer = $this->rgf("customer", $customer, "customer_id", "customer_short_name");
		$sql = "SELECT * FROM invoice_date WHERE id_customer = '$customer' AND id_year = '$year' AND id_month = '$month' ORDER BY id_number DESC";
		$sel = $j->select($sql);
        if($j->num_rows()){
        	extract($sel[0][0]);         	
	    }

	    return array($id_customer.'/'.substr($id_number, 0, 4).'/'.substr($id_number, 4), $id_date_added);
	}

	public function assignInvoiceNumber($year, $month){

		$time = strtotime(date($year.'-'.$month.'-15'));
		$dm = date('y', $time).date('m', $time);
		$suffix = $dm;

		$id_number = 0;
		$customers = array();
        $db = new Db();
        $j = new Db();
        $select = $db->select("SELECT customer_id FROM customer ORDER BY customer_short_name ASC");
        foreach($select[0] as $row){
            extract($row);
            $this->lockReadings($customer_id, $year, $month);
            $c = $this->rgf("customer", $customer_id, "customer_id", "customer_short_name");

            $sel = $j->select("SELECT TOP 1 id_number FROM invoice_date ORDER BY id_number DESC");
            if($j->num_rows()){
            	extract($sel[0][0]);            	
		    }
            $f = (substr($id_number, 4))+1;
			echo '<br/>'.$number = $suffix.str_pad($f, 3, "0", STR_PAD_LEFT);

			$sel = $j->select("SELECT id_id FROM invoice_date WHERE id_number = '$number' AND id_customer_id = '$customer_id'");

            if(1){
            	$insert = $j->insert("invoice_date", [
            		"id_number"=>$number,
            		"id_year"=>$year,
            		"id_month"=>$month,
            		"id_date_added"=>time(),
            		"id_added_by"=>user_id(),
            		"id_customer"=>$c,
            	]);           	
		    }

		    echo $j->error();

        }
	}

	public function isOnListAndIsPending(){

		$db = new Db();
		$user_id = user_id();
		$designation = $this->rgf("sysuser", $user_id, "user_id", "user_designation");

		$sql = "SELECT * FROM done_levels WHERE (dl_selected2 = '$designation' AND dl_app1 = 1 AND dl_app2 IS NULL) OR (dl_selected3 = '$designation' AND dl_app2 = 1 AND dl_app3 IS NULL) ORDER BY dl_date_added";
		$select = $db->select($sql);

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

		$select = $db->select("SELECT * FROM done_levels WHERE (dl_selected2 = '$designation' AND dl_app1 = 1) OR (dl_selected3 = '$designation' AND dl_app2 = 1) OR dl_selected1 = '$designation' ORDER BY dl_date_added");

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

	public function lockReadings($customer_id, $year, $month){
		$db = new Db();

		$insert = $db->insert("lock", [
			"lock_status"=>1,	
			"lock_year"=>$year,	
			"lock_month"=>$month,	
			"lock_customer_id"=>$customer_id,	
		]);
	}

	public function next($level, $year, $month){
		if($level == 3){
			$level = 1;
		}else{
			$level = $level+1;
		}
		$db = new Db();
		$select = $db->select("SELECT dl_selected".($level)." as did FROM done_levels WHERE dl_year = '$year' AND dl_month = '$month' ");
		extract($select[0][0]);

		$user_id = $this->rgf("sysuser", $did, "user_designation", "user_id");

		$fname = $this->rgf("sysuser", $user_id, "user_id", "user_surname");
		$sname = $this->rgf("sysuser", $user_id, "user_id", "user_othername");
		$email = $this->rgf("sysuser", $user_id, "user_id", "user_email");

		return array('fname'=>$fname, 'sname'=>$sname, 'email'=>$email);

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
		$m = $this->rateSum($customer_id, $year, $month);
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
				$payables[$bn_customer][] = $bn_metering_point;
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
				$payablez[$bn_customer_payable_to][] = $bn_metering_point;
			}
		}

		$payable = 0;
		foreach($payablez as $pay => $values){

			//AT KIL
			if($customer_id == 1017 && $pay == 2019 && $rea_date < strtotime(date('2019-08-18'))){

			}elseif($customer_id == 2021 && $pay==2017 ){
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
	
	public function lossAction(){
		
		$id = portion(3);
		$month = portion(5);
		$year = portion(4);
		$note = portion(6);

		if($month == 1){
			$prev_year = $year-1;
			$prev_month = 12;
		}else{
			$prev_year = $year;
			$prev_month = $month-1;
		}

		echo '<div id="side">';

		$units = $this->customerAndSchedule($id, $year, $month);
		$this->energyLinks($id, $year, $month);

		echo '<div class="clearfix"></div><br/>';
		echo '<div id="printMe">';
		//echo '<style>@media print{@page {size: landscape}</style>';

		echo '<h4><center class="pull-left">STATEMENT OF TRANSMISSION VARIANCE FOR '.strtoupper(month_name($month)).' '.$year.'</center></h4>';
		echo '<div class="clearfix"></div><hr/>';

		echo '<table style="font-family:arial;"><tr valign="top"><td>';
		echo '<h5>Purchases</h5>';
		echo '<table id="table" cellspacing="0" cellpadding="2" border="1" style="width:auto;font-size:8px;">';

		echo '<tr valign="bottom">';
		echo '<th rowspan="2"><center>No</center></th>';
		echo '<th rowspan="2" colspan="2"><center>GENERATOR<br/>/ Purchases</center></th>';
		echo '<th rowspan="2"><center>Category</center></th>';
		echo '<th colspan="2"><center>'.month_name_short($month).' '.$year.'</center></th>';
		echo '<th colspan="2"><center>'.month_name_short($prev_month).' '.$prev_year.'</center></th>';
		echo '</tr>';

		echo '<tr>';
		echo '<th><center>UNITS</center></th>';
		echo '<th><center>%</center></th>';
		echo '<th><center>UNITS</center></th>';
		echo '<th><center>%</center></th>';
		echo '</tr>';
		$db = new Db();
		$select = $db->select("SELECT * FROM generator, metering_point WHERE ge_metering_point = mp_id ORDER BY mp_location ASC ");
		$no = 1;
		$tc = array();
		$tp = array();
		foreach($select[0] as $row){
			extract($row);
			$c = rate_calculate($mp_customer_id, strtotime(date("$year-$month-15")), $mp_id);
			$p = rate_calculate($mp_customer_id, strtotime(date("$prev_year-$prev_month-15")), $mp_id);
			
			$tc[$mp_id] = $c[3];
			$tp[$mp_id] = $p[3];

		}
		$ttc = array_sum($tc);
		$ttp = array_sum($tp);
		$cus = array();
		$db = new Db();
		$select = $db->select("SELECT * FROM generator, metering_point WHERE ge_metering_point = mp_id ORDER BY mp_customer_id ASC ");
		foreach($select[0] as $row){
			extract($row);

			$cus_name = $this->rgf("customer", $mp_customer_id, "customer_id", "customer_short_name");
			if(in_array($cus_name, $cus)){
				$cus_name = "";
			}else{
				array_push($cus, $cus_name);
				$cus_name = "<b>$cus_name</b>";
			}
			
			if(!empty($cus_name)){
				$b = "border-top:2px solid #000;";
			}else{
				$b = "";
			}

			echo '<tr>';
			echo '<td style="'.$b.'">'.($no++).'.</td>';
			echo '<td style="'.$b.'">'.$cus_name.'</td>';
			echo '<td style="'.$b.'">'.$mp_location.'</td>';
			echo '<td style="'.$b.'">'.$this->rgf('generator_category', $ge_category, 'gc_id', 'gc_name').'</td>';
			echo '<td style="text-align:right;'.$b.'">'.number_format($tc[$mp_id]/1000).'</td>';
			echo '<td style="text-align:right;'.$b.'">'.number_format($tc[$mp_id]/$ttc*100, 1).' %</td>';
			echo '<td style="text-align:right;'.$b.'">'.number_format($tp[$mp_id]/1000).'</td>';
			echo '<td style="text-align:right;'.$b.'">'.number_format($tp[$mp_id]/$ttp*100, 1).' %</td>';
			echo '</tr>';
		}

		echo '<tr>';
		echo '<td></td>';
		echo '<td colspan="3" style="text-align:left; font-weight:bold;">Total Sales</td>';
		echo '<td style="text-align:right; font-weight:bold;">'.number_format($ttc/1000).'</td>';
		echo '<td style="text-align:right; font-weight:bold;">'.number_format(100).' %</td>';
		echo '<td style="text-align:right; font-weight:bold;">'.number_format($ttp/1000).'</td>';
		echo '<td style="text-align:right; font-weight:bold;">'.number_format(100).' %</td>';
		echo '</tr>';

		echo '</table>';


		echo '</td><td style="width:30px;">';
		echo '</td><td>';
		echo '<h5>Sales</h5>';
		echo '<table id="table" cellspacing="0" cellpadding="2" border="1" style="width:auto;font-size:8px;">';

		echo '<tr valign="bottom">';
		echo '<th rowspan="2"><center>No</center></th>';
		echo '<th rowspan="2"><center>CUSTOMER</center></th>';
		echo '<th colspan="2"><center>'.month_name_short($month).' '.$year.'</center></th>';
		echo '<th colspan="2"><center>'.month_name_short($prev_month).' '.$prev_year.'</center></th>';
		echo '</tr>';

		echo '<tr>';
		echo '<th><center>UNITS</center></th>';
		echo '<th><center>%</center></th>';
		echo '<th><center>UNITS</center></th>';
		echo '<th><center>%</center></th>';
		echo '</tr>';

		$tot_pre = 0;
		$tot_cur = 0;

		$p = array();
		$c = array();

		$db = new Db();
		$select = $db->select("SELECT * FROM customer ORDER BY customer_short_name ASC");
		foreach($select[0] as $row){
			extract($row);		

			$h = $this->invoiceTotalSummary($customer_id, $year, $month);
			//$tot_cur += $h[0];
			$c[$customer_id] = $h[0];

			$h = $this->invoiceTotalSummary($customer_id, $prev_year, $prev_month);
			//$tot_pre += $h[0];
			$p[$customer_id] = $h[0];
		}

		$tc = array_sum($c);
		$tp = array_sum($p);

		$db = new Db();
		$select = $db->select("SELECT * FROM customer ORDER BY customer_short_name ASC");
		$n=0;
		foreach($select[0] as $row){
			extract($row);

			echo '<tr>';
			echo '<td>'.(++$n).'.</td>';
			echo '<td style="text-align:left;">'.$customer_short_name.'</td>';
			echo '<td style="text-align:right;">'.number_format($c[$customer_id]).'</td>';
			echo '<td style="text-align:right;">'.number_format($c[$customer_id]/$tc*100, 1).' %</td>';
			echo '<td style="text-align:right;">'.number_format($p[$customer_id]).'</td>';
			echo '<td style="text-align:right;">'.number_format($p[$customer_id]/$tp*100, 1).' %</td>';
			echo '</tr>';
		}

		echo '<tr>';
		echo '<td></td>';
		echo '<td style="text-align:left; font-weight:bold;">Total Sales</td>';
		echo '<td style="text-align:right; font-weight:bold;">'.number_format($tc).'</td>';
		echo '<td style="text-align:right; font-weight:bold;">'.number_format(100).' %</td>';
		echo '<td style="text-align:right; font-weight:bold;">'.number_format($tp).'</td>';
		echo '<td style="text-align:right; font-weight:bold;">'.number_format(100).' %</td>';
		echo '</tr>';

		echo '</table>';

		echo '<br/>';

		echo '<h5>Total Purchases, Sales and Losses</h5>';

		echo '<table id="table" border="1" cellspacing="0" cellpadding="2" style="font-size:10px;">';

		echo '<tr>';
		echo '<th style="text-align:left;">Period</th>';
		echo '<th><center>'.month_name_short($month).' '.$year.'</center></th>';
		echo '<th><center>'.month_name_short($prev_month).' '.$prev_year.'</center></th>';
		echo '</tr>';

		echo '<tr>';
		echo '<th style="text-align:left;">Total Purchases</th>';
		echo '<th style="text-align:right;">'.number_format($ttc).'</th>';
		echo '<th style="text-align:right;">'.number_format($ttp).'</th>';
		echo '</tr>';

		echo '<tr>';
		echo '<th style="text-align:left;">Total Sales</th>';
		echo '<th style="text-align:right;">'.number_format($tc).'</th>';
		echo '<th style="text-align:right;">'.number_format($tp).'</th>';
		echo '</tr>';

		echo '<tr>';
		echo '<th style="text-align:left;">Losses</th>';
		echo '<th style="text-align:right;">'.number_format($ttc - $tc).'</th>';
		echo '<th style="text-align:right;">'.number_format($ttc - $tp).'</th>';
		echo '</tr>';

		echo '<tr>';
		echo '<th style="text-align:left;">Losses (%)</th>';
		echo '<th style="text-align:right;">'.number_format(($ttc - $tc)/$ttc*100, 2).' %</th>';
		echo '<th style="text-align:right;">'.number_format(($ttp - $tp)/$ttc*100, 2).' %</th>';
		echo '</tr>';

		echo '</table>';

		echo '</td></tr></table>';


		echo '</div>';
		

	}

	public function viewInvoiceAction(){
		
		$id = portion(3);
		$month = portion(5);
		$year = portion(4);
		$note = portion(6);

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
				$this->viewInvoiceCreditNote($id, $month, $year);
			}else{
				$this->viewInvoiceNormal($id, $month, $year);
			}
		}

	}

	public function viewInvoiceNormal($id, $month, $year){
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
			<?php echo '<title>'.strtoupper($customer_short_name.' INVOICE').' - '.($month = portion(5)).' '.portion(4).'</title>'; ?>
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
							if(date('Y', $rea_date)==portion(4) && date('m', $rea_date)==portion(5)){
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
								<?php $year = portion(4); $month = portion(5); ?>
								<td>Detail:</td>
								<td><u><b><?php echo $this->noSpace('Energy Bill '.month_name($month).' '.$year); ?></b></u></td>
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




						if($customer_id ==2028){
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

							echo '<tr style="height:90px;">';
							echo '<td>Total</td>';
							echo '<td align="right">'.number_format($ta).'</td>';
							echo '<td align="right"></td>';
							echo '<td align="right">'.number_format($shoulder*$ta, 2).'</td>';
							echo '</tr>';

							?>

							<tr>
								<td colspan="4" style="font-size:12px;">
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
								?>
								</td>
								<?php } ?>
							</tr>

							<?php

							echo '</table>';
						}else{	

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
						$m = $this->rateSum($customer_id, $year, $month);
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
								<td colspan="2">Total Amount(Energy)</td>
								<td align="right"><?php echo number_format($ttu,2); ?></td>
								<?php 
									if($symbol == '$'){
										$a = number_format($tta,2);
									}else{
										$a = number_format($tta);
									}
								?>
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
									$payables[$bn_customer][] = $bn_metering_point;
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
									$payablez[$bn_customer_payable_to][] = $bn_metering_point;
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

										echo $v = '<b>'.$this->rgf("customer", $pay, "customer_id", "customer_short_name").'</b> ';

										if($customer_id != 2020){
											echo $v = '('.implode(", ", $j[1]).') ';
										}else{ 
											echo '<br/>'; 
										}

										$payable += $j[2];
										$i++;
									}

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
										}elseif($customer_id == 2021 && $pay==2017 ){
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

	public function isDNorCN($customer_id, $year, $month){
		$m = $this->rateSumCN($customer_id, $year, $month);
		//echo '->'.$m[0].'->'.$units;
		//$to1 = ($m[0]*$units);
		//$to2 = ($m[1]*$units);
		//$to3 = ($m[2]*$units);
		$to4 = ($m[3]);

		$reading_time = strtotime(date("$year-$month-15"));
		$mo = array();

		foreach($m[8] as $mpid){
			$g = rate_calculate2($customer_id, $reading_time, $mpid);
			//$mo[0] += $g[0];
			//$mo[1] += $g[1];
			//$mo[2] += $g[2];
			$mo[3] += $g[3];
		}

		//echo '->'.$m[0].'->'.$units;
		//$t1 = ($mo[0]*$units);
		//$t2 = ($mo[1]*$units);
		//$t3 = ($mo[2]*$units);
		$t4 = ($mo[3]);

		//$v1 = $to1 - $t1;
		//$v2 = $to2 - $t2;
		//$v3 = $to3 - $t3;
		$v4 = $to4 - $t4;

		return $v4;
	}

	public function viewInvoiceCreditNote($id, $month, $year){
		$note = portion(6);
		if($this->isDNorCN($id, $year, $month) > 0){
			$n = "DN ";
			$nn = "Debit Note";					
		}else{
			$n = "CN ";
			$nn = "Credit Note";	
		}

		$db = new Db();
		$select = $db->select("SELECT * FROM customer WHERE customer_id = '$id'");
		extract($select[0][0]);

		//cn_id, cn_peak, cn_off_peak, cn_shoulder, 
		$select = $db->select("SELECT cn_details FROM credit_note WHERE cn_year='$year' AND cn_month='$month' AND cn_customer_id='$id'");
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
			<?php echo '<title>'.strtoupper($customer_short_name.' INVOICE').' - '.($month = portion(5)).' '.portion(4).'</title>'; ?>
			<style type="text/css">
				table tr td{
					padding:5px 20px;
				}
			</style>
			<table border="0" cellspacing="0" cellpadding="2" id="y" style="width:100%; font-family: arial; margin:auto;">
				<tbody>
				<tr valign="bottom">
					<td colspan="11">
					<img class="hide" src="<?php display_url();?>images/uetcl_header.png" width="100%">
					<center class="hide"><h2 style="color:blue;">TAX <?php echo strtoupper($nn); ?></h2></center>
						<!-- <hr style="border:1px solid black;"/> -->
						<?php
						$db = new Db();
						$select = $db->select("SELECT * FROM metering_point, r_reading_cn WHERE mp_customer_id = '$customer_id' AND n_rea_cus_id = '$customer_id' AND mp_id = n_rea_mp_id");
						foreach($select[0] as $row){
							extract($row);
							if(date('Y', $n_rea_date)==portion(4) && date('m', $n_rea_date)==portion(5)){
								$invoice_no = str_pad($n_rea_id, 5, '0', STR_PAD_LEFT);;	
							}
						}

						?>
						<p align="right" style="font-size:10pt; padding:0;margin:0; font-weight: bold; color:red;"><span style="color:black;">Tax <?php echo $nn; ?> No.:</span> &nbsp; <?php echo $customer_short_name.'/'.$invoice_no; ?></p>
						<table border="0" class="" cellspacing="10" style="width:100% !important;font-family: arial; font-size: 12pt;">
							
							<tr valign="top">
								<td>Date:</td>
								<td><?php echo date('dS F Y '); ?></td>
								<td rowspan="4" style="width:1%;">
									
								</td>
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
								?>								
								</td>
							</tr>
							<tr>
								<?php $year = portion(4); $month = portion(5); ?>
								<td>Detail:</td>
								<td><b><?php echo nl2br($cn_details); ?></b></td>
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
						$m = $this->rateSumCN($customer_id, $year, $month);
						//echo '->'.$m[0].'->'.$units;
						$to1 = ($m[0]*$units);
						$to2 = ($m[1]*$units);
						$to3 = ($m[2]*$units);
						$to4 = ($m[3]*$units);

						$reading_time = strtotime(date("$year-$month-15"));
						$mo = array();

						foreach($m[8] as $mpid){
							$g = rate_calculate2($customer_id, $reading_time, $mpid);
							$mo[0] += $g[0];
							$mo[1] += $g[1];
							$mo[2] += $g[2];
							$mo[3] += $g[3];
						}

						//echo '->'.$m[0].'->'.$units;
						$t1 = ($mo[0]*$units);
						$t2 = ($mo[1]*$units);
						$t3 = ($mo[2]*$units);
						$t4 = ($mo[3]*$units);

						
						$v1 = $to1 - $t1;
						$v2 = $to2 - $t2;
						$v3 = $to3 - $t3;
						$v4 = $to4 - $t4;
						if($this->isDNorCN($customer_id, $year, $month) > 0){
							$sub = "(a - b)";
						}else{
							$sub = "(b - a)";
							$v1 *= -1;
							$v2 *= -1;
							$v3 *= -1;
							$v4 *= -1;
						}

						$h = $this->wheelingChargeSum($customer_id, $year, $month);
						
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
								<th rowspan="2">RATE - <?php echo "(kWh/$symbol)"; ?></th>
								<th colspan="3">UNITS - 
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
								<th rowspan="2">AMOUNT<?php echo " ($symbol)"; ?></th>
							</tr>
							<tr valign="bottom">
								<th>Ref: <?php echo month_name_short($month).' '.$year; ?><br/>(a)</th>
								<th>Corrected<br/>(b)</th>
								<th>Variance<br/><?php echo $sub; ?></th>
							</tr>
							<tr>
								<?php 
								$o = $peak;
								$ttu += $t2;
								$ttuo += $to2;
								$tta += $o*$v2;
								?>
								<td>Peak</td>
								<td align="right"><?php echo $o. ""; ?></td>
								<td align="right"><?php echo number_format($t2,2); ?></td>
								<td align="right"><?php echo number_format($to2,2); ?></td>
								<td align="right"><?php echo number_format($v2,2); ?></td>
								<td align="right"><?php echo number_format($o*$v2). ""; ?></td>
							</tr>
							<tr>
								<?php 
								$o = $shoulder;
								$ttu += $t1;
								$ttuo += $to1;
								$tta += $o*$v1;
								?>
								<td>Shoulder</td>
								<td align="right"><?php echo ($o). ""; ?></td>
								<td align="right"><?php echo number_format($t1,2); ?></td>
								<td align="right"><?php echo number_format($to1,2); ?></td>
								<td align="right"><?php echo number_format($v1,2); ?></td>
								<td align="right"><?php echo number_format($o*$v1). ""; ?></td>
							</tr>
							<tr>
								<td>Off Peak</td>
								<?php 
								$o = $off_peak;
								$ttu += $t3;
								$ttuo += $to3;
								$tta += $o*$v3;
								?>
								<td align="right"><?php echo ($o). " "; ?></td>
								<td align="right"><?php echo number_format($t3,2); ?></td>
								<td align="right"><?php echo number_format($to3,2); ?></td>
								<td align="right"><?php echo number_format($v3,2); ?></td>
								<td align="right"><?php echo number_format($o*$v3). " "; ?></td>
							</tr>
							<tr>
								<td colspan="2">Total Amount(Energy)</td>
								<td align="right"><?php echo number_format($ttu,2); ?></td>
								<td align="right"><?php echo number_format($ttuo,2); ?></td>
								<td align="right"><?php echo number_format(abs($ttuo-$ttu),2); ?></td>
								<td align="right"><?php echo number_format($tta). " "; ?></td>
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
									$payables[$bn_customer][] = $bn_metering_point;
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
									$payablez[$bn_customer_payable_to][] = $bn_metering_point;
								}
							}

							?>
							<tr>
								<td rowspan="2">Wheeling Charges</td>
								<td>Payable To <b>(Add)</b></td>
								<td colspan="3"><?php 
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

										echo $v = '<b>'.$this->rgf("customer", $pay, "customer_id", "customer_short_name").'</b> ';

										if($customer_id != 2020){
											echo $v = '('.implode(", ", $j[1]).') ';
										}else{ 
											echo '<br/>'; 
										}

										$payable += $j[2];
										$i++;
									}

									$payable = 0; //put to zero;

								?></td>
								<td align="right"><?php  echo ''.number_format($payable). ""; ?></td>
								<!-- <td colspan="2">Collectable(Less)</td> -->
								<!-- <td align="right"><?php // $payable = array_sum($h[0]); echo number_format($payable). " $symbol"; ?></td> -->

							</tr>
							<tr>
								<td>Collectable From <b>(Less)</b></td>
								<td colspan="3" style="width:40%;"><?php 
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
											$jm = $this->krecsAndUedcl($customer_id, $pay,  $year, $month);					
										}elseif($customer_id == 2021 && $pay==2017 ){
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

									$collectable = 0; //put to zero;
									?></td>
								<td align="right"><?php echo '('.number_format($collectable). ") "; ?></td>
							</tr>
							<?php 
							$tbf = $this->totalBackflow($id, $year, $month);
							$tbf[4] = 0; //put to zero;
							$tbf[5] = 0; //put to zero;
							if($customer_id == 2019){ 

							?>
							<tr>
								<td colspan="2">Backflows</td>
								<td colspan="3" align="right"><?php echo number_format($tbf[4],2); ?></td>
								<td align="right">(<?php echo number_format($tbf[5]); ?>)</td>
							</tr>
							<?php
							} 
							?>
							<?php $tta += $payable+($collectable*-1)+($tbf[5]*-1); ?>
							<tr>
								<td colspan="5">Total (VAT Exclusive)</td>
								<td align="right"><?php echo number_format($tta). " "; ?></td>
							</tr>							
							<tr>
								<td colspan="5">Add <?php echo number_format($customer_vat); ?>% VAT</td>
								<?php $tot = $customer_vat/100*$tta; ?>
								<td align="right"><?php echo number_format($tot). " "; ?></td>
							</tr>							
							<tr>
								<td colspan="5"><b>Total Amount (VAT Incl)</b></td>
								<?php $tot = $customer_vat/100*$tta; ?>
								<td align="right"><b><?php echo number_format($tta*($customer_vat/100+1)). " "; ?></b></td>							
							<tr>
								<td colspan="6">
								<?php $tot = $tta*($customer_vat/100+1); if($tot<200000000000){?>
								Total in words:<br/><?php echo '<b>'.number_to_words($tta*($customer_vat/100+1)). " $currency".'</b>'; ?></td>
								<?php } ?>
							</tr>
						</table>
						<span style="color:red; font-weight: bold; font-size: 12px; text-align: center; ">Accounts are due within 45days of receipt of this invoice</span>
						<?php $this->showReadingComments(); ?>	
						
						<?php $this->energyApprovals(user_id(), $year, $month, $invoice=1); ?>
					</td>				
				</tr>
				
			</tbody></table>
		</div>
		<?php echo '<div style="width:900px; margin:auto;">'; $this->addReadingComment(); echo '</div>';?>
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

	public function tou($customer_id, $month, $year){
		$db = new Db();
		$select = $db->select("SELECT * FROM time_band WHERE tb_customer_id = '$customer_id' ORDER BY tb_id DESC");
		$assigned = 0;
		
		if($assigned == 0){
			foreach($select[0] as $row){
				extract($row);
				if($tb_year==$year && $tb_month==$month)
				{
					$peak = $tb_peak;
					$shoulder = $tb_shoulder;
					$off_peak = $tb_off_peak;
					$effective_date = $tb_effective_date;
					$expiry_date = $tb_expiry_date;
					$assigned = 1;
					break;
				}
			}

		}
		
		if($assigned == 0){
			foreach($select[0] as $row){
				extract($row);
				if(date('Y', $tb_date_added)==$year && date('m', $tb_date_added)==$month)
				{
					$peak = $tb_peak;
					$shoulder = $tb_shoulder;
					$off_peak = $tb_off_peak;
					$effective_date = $tb_effective_date;
					$expiry_date = $tb_expiry_date;
					$assigned = 1;
					break;
				}
			}
		}

		if($assigned == 0){
			$db = new Db();
			$select = $db->select("SELECT * FROM time_band WHERE tb_customer_id = '$customer_id' ORDER BY tb_id DESC");
			$assigned = 0;
			foreach($select[0] as $row){
				extract($row);
				if(date('Y', $tb_date_added)<=$year && date('m', $tb_date_added) <= $month)
				{
					$peak = $tb_peak;
					$shoulder = $tb_shoulder;
					$off_peak = $tb_off_peak;
					$effective_date = $tb_effective_date;
					$expiry_date = $tb_expiry_date;
					$assigned = 1;
					break;
				}
			}
		}

		if($assigned == 0){
			$db = new Db();
			$select = $db->select("SELECT * FROM time_band WHERE tb_customer_id = '$customer_id' ORDER BY tb_id DESC");
			$assigned = 0;
			foreach($select[0] as $row){
				extract($row);
				//if(date('Y', $tb_date_added)<=$year && date('m', $tb_date_added) <= $month)
				{
					$peak = $tb_peak;
					$shoulder = $tb_shoulder;
					$off_peak = $tb_off_peak;
					$effective_date = $tb_effective_date;
					$expiry_date = $tb_expiry_date;
					$assigned = 1;
					break;
				}
			}
		}

		return array($peak, $shoulder, $off_peak, $effective_date, $expiry_date);
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
										]);
									}else{ //exports as advance
										$select = $db->insert("r_rate",[
											"rate_import_wh_1"=>$rate4,
											"rate_import_wh_2"=>$rate5,
											"rate_import_wh_3"=>$rate6,
											"rate_export_wh_4"=>$rate1,
											"rate_export_wh_5"=>$rate2,
											"rate_export_wh_6"=>$rate3,
											"rate_reading_id"=>$rrr,
											"rate_added_by"=>user_id(),
											"rate_date_added"=>time(),
											"rate_cus_id"=>$customer_id
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
										}else{ //exports as advance
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
							$sql = "SELECT * FROM r_reading WHERE rea_date = '$time_readings' AND rea_mp_id = '$metering_point_id' AND rea_cus_id = '$customer_id'";
							$check = $dx->select($sql);
							if($dx->num_rows() > 0 ){
								$error[] = "Readings already exit for  &nbsp;  &nbsp;  &nbsp; <b>Customer: </b>".$this->rgf("customer", $customer_id, "customer_id", "customer_short_name")." &nbsp;  &nbsp;  &nbsp; <b>Year: </b>".$year_uploaded."  &nbsp;  &nbsp;  &nbsp;  <b>Month: </b>".$month_uploaded;
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
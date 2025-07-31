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
	public function AddCustomerServiceAction(){
		global $con;
		?>
		<div id="side">
		<div class="col-md-4">
			<form id="form" action="" method="post" style="background-color: #f9f9f9; border:1px solid black; margin-top:20px; padding:10px; ">
				<div id="form_title">Service Form</div>
				<div style="padding:10px;">
					<?php 
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
							$insert = mysqli_query($con, "INSERT INTO `service`(`se_id`, `service_number`, `service_name`, `service_rate`, `se_date`, `se_added_by`) VALUES (NULL,'$service_number','$service_name','$service_rate','$time','$user_id')");
							if($insert){
								Feedback::success();
								Feedback::refresh();
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
		<div class="col-lg-8"><br/>
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
	public function editCustomerServiceAction(){
		global $con;
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
	
	public function import(){
	
		global $con;
	?>
	<div id="side">
		<h2 style="border-bottom:2px solid #ccc;margin-bottom:10px;"><i class="fa fa-fw fa-table"></i>Import Reading</h2>
		
		<form class="search" action="" method="post" enctype="multipart/form-data">
			<fieldset>
			<?php
			if(isset($_POST['import'])){
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
						
						//echo $count." ".$custno."==>$meterno==>$previous==>$previous1<br/>";
						
						//checking file required file
						$string = strtoupper("Elster Metering Systems Power Master Unit");
						
						if($count == 1){
							if($col_2 != $string){
								$error[] = "Invalid File";
							}
						}						
						
					}
					
					//start From Unit
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
								$customer_id = $_POST['customer_id'];
								$metering_point_id = $_POST['metering_point_id'];
								$year_uploaded = $_POST['year'];
								$month_uploaded = $_POST['month'];
								
								$time_readings = strtotime(date($year_uploaded.'-'.$month_uploaded.'-15'));
								
								//$time_readings = date('Y-m-d H:i:s a', strtotime($date.' '.$time.' '.$time_format));
								$tim = time();
								$user_id = user_id();
								
								$sql = "SELECT * FROM r_reading WHERE rea_date = '$time_readings' AND rea_mp_id = '$metering_point_id' AND rea_cus_id = '$customer_id'";
								$check = mysqli_query($con, $sql);
								if(mysqli_num_rows($check) > 0 ){
									$error[] = "File already exists";
									break;
								}

								$sql = "INSERT INTO `r_reading`(`rea_id`, `rea_number`, `rea_date`, `rea_cus_id`, `rea_added_by`, `rea_date_added`, `rea_mp_id`) VALUES (NULL,'$file_name','$time_readings','$customer_id','$user_id','$tim','$metering_point_id')";
								
								$insert = mysqli_query($con, $sql);
								
								if(!$insert){									
									$error[] = "Not inserted";
								}else{
									$rrr = mysqli_insert_id($con);
								}
								
								break;
							}
						}
						if(!empty($error)){
							error($error);
						}
					}//ending FROM unit
					
					//starting cumulative totals
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
							
							$col_1 = @(trim(str_replace("'","\'",strip_tags($emapData[0]))));
							$col_2 = @(trim(str_replace("'","\'",strip_tags($emapData[1]))));
							$col_3 = @(trim(str_replace("'","\'",strip_tags($emapData[2])))); 
							$col_4 = @(trim(str_replace("'","\'",strip_tags($emapData[3]))));
							
							if($col_1 == $columns[1]){									
								$isReached = True;
								$section = $col_1;
								continue;
							}else{
								if(!$isReached) continue;
								if(!empty($col_2)&&!empty($col_3)){
									
									if($col_2 != "Value" && $col_2 != "Unit"){
										//echo $col_1.' => '.((number_format((double)str_replace(',', '', @$col_2),3))).' => '.$col_3.' => '.$col_4.'<br/>';
										if(empty($col_2)||empty($col_3)){
											$error[] = "Check Line $count. Its should not be empty.";
											break;
										}
										$unit[] = strtolower(str_replace(' ', '_', @$col_3));
										$value[] = ((((double)str_replace(',', '', @$col_2))));
									}
								}
							}
							//break after another section has started.
							if($col_1 == $columns[2]){
								$sql = 'INSERT INTO r_cumulative_totals ';
								$sql .= '( `ct_'.implode('`, `ct_', $unit).'`, `ct_reading_id`)';
								$sql .= ' VALUES ';
								$sql .=  '("'.implode('","', $value).'", "'.$rrr.'")';
								
								$insert = mysqli_query($con, $sql);
								if(!$insert){
									$error[] = "Not inserted";
									echo mysqli_error($con);
								}
								
								break;
							}
						}
						if(!empty($error)){
							error($error);
						}else{
							refresh(3);
						}
					}//ending cumulative totals
					
					
					//3. starting multi utility registers
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
							
							$col_1 = @(trim(str_replace("'","\'",strip_tags($emapData[0]))));
							$col_2 = @(trim(str_replace("'","\'",strip_tags($emapData[1]))));
							$col_3 = @(trim(str_replace("'","\'",strip_tags($emapData[2])))); 
							$col_4 = @(trim(str_replace("'","\'",strip_tags($emapData[3]))));
							
							if($col_1 == $columns[2]){									
								$isReached = True;
								$section = $col_1;
								continue;
							}else{
								if(!$isReached) continue;
								if(!empty($col_2)&&!empty($col_3)){
									
									if($col_2 != "Value" && $col_2 != "Unit"){
										//echo $col_1.' => '.((number_format((double)str_replace(',', '', @$col_2),3))).' => '.$col_3.' => '.$col_4.'<br/>';
										if(empty($col_2)||empty($col_3)){
											$error[] = "Check Line $count. Its should not be empty.";
											break;
										}
										$unit[] = strtolower(str_replace(' ', '_', @$col_3));
										$value[] = ((((double)str_replace(',', '', @$col_2))));
									}
								}
							}
							//break after another section has started.
							if($col_1 == $columns[3]){
								$sql = 'INSERT INTO r_multi_utility_registers ';
								$sql .= '( `mur_'.implode('`, `mur_', $unit).'`, `mur_reading_id`)';
								$sql .= ' VALUES ';
								$sql .=  '("'.implode('","', $value).'", "'.$rrr.'")';
								
								$insert = mysqli_query($con, $sql);
								if(!$insert){
									$error[] = "Not inserted";
									echo mysqli_error($con);
								}
								
								break;
							}
						}
						if(!empty($error)){
							error($error);
						}else{
							//echo '<div id="note1">Successfully Saved</div>';
							//refresh(3);
						}
					}//ending multi utility registers
					
					
					
					//4. starting Rates
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
								$sql .= '( `rate_'.implode('`, `rate_', $unit).'`, `rate_reading_id`, `rate_cus_id`)';
								$sql .= ' VALUES ';
								$sql .=  '("'.implode('","', $value).'", "'.$rrr.'", "'.$customer_id.'")';
								
								$insert = mysqli_query($con, $sql);
								if(!$insert){
									$error[] = "Not inserted";
									echo mysqli_error($con);
								}
								
								break;
							}
						}
						if(!empty($error)){
							error($error);
						}else{
							//echo '<div id="note1">Successfully Saved</div>';
							//refresh(3);
						}
					}//ending multi utility registers
					
					//5. starting Rates
					if(empty($error)){
						$file = fopen($filename, "r");
						$count = 0;
						$error = array();
						$section = "";
						$isReached = False;
						$value = array();
						$value1 = array();
						$unit = array();
						while (($emapData = fgetcsv($file, 10000, ",")) !== FALSE){
							$count++;
							
							$col_1 = @(trim(str_replace("'","\'",strip_tags($emapData[0]))));
							$col_2 = @(trim(str_replace("'","\'",strip_tags($emapData[1]))));
							$col_3 = @(trim(str_replace("'","\'",strip_tags($emapData[2])))); 
							$col_4 = @(trim(str_replace("'","\'",strip_tags($emapData[3]))));
							
							if($col_1 == $columns[4]){									
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
											$value1[] = ((strtotime(str_replace(',', '', @$col_4))));
										}
									}
								}
							}
							//break after another section has started.
							if($col_1 == $columns[5]){
								$sql = 'INSERT INTO r_maximum_demands ';
								$sql .= '( `md_'.implode('`, `md_', $unit).'`, `md_reading_id`)';
								$sql .= ' VALUES ';
								$sql .=  '("'.implode('","', $value).'", "'.$rrr.'")';
								
								$sql1 = 'INSERT INTO r_maximum_demands_time ';
								$sql1 .= '( `mdt_'.implode('`, `mdt_', $unit).'`, `mdt_reading_id`)';
								$sql1 .= ' VALUES ';
								$sql1 .=  '("'.implode('","', $value1).'", "'.$rrr.'")';
								
								$insert = mysqli_query($con, $sql);
								$insert1 = mysqli_query($con, $sql1);
								
								if(!$insert || !$insert1){
									$error[] = "Not inserted";
									//echo mysqli_error($con);
								}
								
								break;
							}
						}
						if(!empty($error)){
							error($error);
						}else{
							echo '<div id="note1">Successfully Saved</div>';
							Feedback::refresh();
						}
					}//ending multi utility registers
					
					
				}else{
					echo 'Invalid';
				}
				
			}
			
			if(isset($_POST['filter'])){
				$year_uploaded = $_POST['filter_year'];
				$month_uploaded = $_POST['filter_month'];
			}else{
				$month_uploaded = (date('m'));
				$year_uploaded = (date('Y'));
			}
			
			?>
			<div style="padding:10px;">
				<div class="pull-left">
					<p>Year</p>
					<select name="year" style="width:80px; margin: auto 10px;">
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
					<p>Month</p>
					<select name="month" style="width:120px; margin: auto 10px;">
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
					<p>Customer Name</p>
					<select id="customer" onchange="return metering_points(); " name="customer_id" style="width:150px; margin: auto 10px;">
						<option>Select</option>
						<?php 
						$select = mysqli_query($con, "SELECT * FROM customers ORDER BY customer_name ASC ");
						while($row = mysqli_fetch_assoc($select)){
							extract($row);
							echo '<option value="'.$cus_id.'">'.$customer_name.'</option>';
						}
						?>
					</select>
				</div>
				<div class="pull-left">
					<p>Metering Point <span id="meterPointNumber"></span></p>
					<select id="meterPoint" name="metering_point_id" style="width:150px; margin: auto 10px;">
						<option>Select</option>
					</select>
				</div>				
				<div  class="pull-left">
					<p>Choose File (CSV)</p>
					<input type="file" name="file" id="file" style="width:200px"/>
				</div>
				<div  class="pull-left">
					<p>&nbsp;</p>
					<input type="submit" value="Save" name="import" style="width:100%; margin: auto 10px;"/>
				</div>
				<div class="clear"></div>
			</fieldset>
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
		<script type="text/javascript">
			function metering_points(){
				var customer = _("customer");
				var customer = customer.options[customer.selectedIndex].value;
				var request;
				if (window.XMLHttpRequest){//for Chrome, Firefox, IE7+, Opera, Safari
					request = new XMLHttpRequest();
				}
				else{//for IE5, IE6
					request = new ActiveXObject("Microsoft.XMLHTTP");
				}	
				request.open("POST", "/UETCL_DEMO/ajax/metering_points.php", true);
				request.setRequestHeader("Content-type", "application/x-www-form-urlencoded");	

				request.onreadystatechange = function() {
					if(request.readyState == 4 && request.status == 200) {
						//4 = The connection is complete, the data was sent or retrieved.
						//200 = The file has been retrieved and you are free to do something with it
						var returned = request.responseText.split("====");			
						
						_("meterPointNumber").innerHTML = "("+returned[0]+")";
						_("meterPoint").innerHTML = returned[1];
					}
				}

				request.send("customer_id="+customer);
			}
		</script>
		<h3>Uploaded summary for Year: <?php echo @$year_uploaded; ?> and Month: <?php echo month_name(@$month_uploaded); ?></h3>
		<table border="1" id="table">
			<tr valign="bottom">
				<th style="width:30px;">No.</th>
				<th width="200px">Customer Name</th>
				<th width="30px">Metering Points</th>
				<th width="30px">Total Uploaded</th>
				<th width="30px">Total Remaining</th>
				<th>List of remaining</th>
				<th width="150px">Action</th>
			</tr>
			<?php 
			$no = 0;
			$select = mysqli_query($con, "SELECT * FROM customers");
			while($row = mysqli_fetch_assoc($select)){
				extract($row);
			echo '<tr>';
			echo '<td>'.(++$no).'.</td>';
			echo '<td>'.$customer_name.'</td>';
			
			echo '<td>'; 
			$sel = mysqli_query($con, "SELECT * FROM metering_points WHERE mp_cus_id = $cus_id;");	
			$total_metering_points = mysqli_num_rows($sel);
			echo $total_metering_points;
			echo '</td>';
			
			echo '<td>'; 
			$sel = mysqli_query($con, "SELECT `rea_mp_id`, `rea_date` as rdd FROM `r_reading` WHERE rea_cus_id = $cus_id;");
			$uploads = array();
			$tot = 0;
			while($ro = mysqli_fetch_assoc($sel)){
				extract($ro);
				if(date('Y', $rdd)==$year_uploaded && date('m', $rdd)==$month_uploaded)
				{
					$uploads[] = $rea_mp_id;
					$tot++;					
				}
			}
			$total_uploaded = $tot;
			echo $total_uploaded;
			echo '</td>';
			
			echo '<td>'; 
			echo $all_remaining = $total_metering_points - $total_uploaded;
			echo '</td>';
			
			echo '<td>';
			if($total_uploaded == 0){
				echo 'All';
			}else{
				$s = mysqli_query($con, "SELECT * FROM metering_points WHERE mp_cus_id = $cus_id");
				$num = number_format(mysqli_num_rows($s));
				$count = 0;
				while($row = mysqli_fetch_assoc($s)){
					extract($row);
					if(!in_array($mp_id, $uploads)){
						echo '<b>'.ucwords($mp_name).'</b>';
						echo ', ';					
					}
				}
			}
			echo '</td>';
			
			echo '<td>';
			if($total_uploaded != 0){
				echo '<a href="'.return_url().'services/schedule/'.$cus_id.'/'.$year_uploaded.'/'.$month_uploaded.'" > <i class="fa fa-fx fa-table"></i> View Schedule</a>';
			}
			echo '</td>';		
			
			echo '</tr>';
			}
			?>
			
		</table>
	</div>
	<?php
	}

	public function scheduleAction(){

		global $con;
		$id = portion(3);
		$year = portion(4);
		$month = portion(5);

	$select = mysqli_query($con, "SELECT * FROM customers WHERE cus_id = '$id'");
	$row = mysqli_fetch_assoc($select);
	extract($row);
	
	?>
	<div id="side">
		<br/>		
		<?php 
		echo '<div class="nav files pull-right">
		<a href="'.return_url().'services/view-invoice/'.$id.'/'.$month.'/'.$year.'"><i class="fa fa-file fa-fw"></i>View Invoice</a>		
		<a class="btn btn-default" href="" onclick="return print(\'printMe\');"><i class="fa fa-print fa-fw"></i> Print</a>
		</div>';
		echo '<div class="clearfix"></div>';
		?>
		<br/>
		<div id="printMe">
			<table border="1" cellspacing="0" cellpadding="2" class="table">
				<tbody>
				<tr valign="bottom">
					<th colspan="11" style="border:2px solid black;">
					<?php
					echo $customer_name;
					echo '<br/>';
					echo $year = portion(4);
					echo ' - ';
					echo month_name($month = portion(5));
					?>
					</th>				
				</tr>
				<tr valign="bottom">
					<th rowspan="3" style="border:2px solid black;">Metering Points</th>
					<th colspan="6" style="border:2px solid black;">Main Meter - Current Reading (KWh)</th>
					<th style="border:2px solid black;" colspan="4" style="border:2px solid black;">Advance</th>				
				</tr>
				<tr>
					<th style="border:2px solid black;" colspan="3">Imports</th>
					<th style="border:2px solid black;" colspan="3">Exports</th>
					<th style="border:2px solid black;" colspan="4">Imports</th>				
				</tr>
				<tr>
					<th style="border-left:2px solid black;border-bottom:2px solid black;">Rate 1</th>
					<th style="border-bottom:2px solid black;">Rate 2</th>
					<th style="border-bottom:2px solid black;">Rate 3</th>
					<th style="border-left:2px solid black;border-bottom:2px solid black;">Rate 4</th>
					<th style="border-bottom:2px solid black;">Rate 5</th>
					<th style="border-bottom:2px solid black;">Rate 6</th>
					<th style="border-left:2px solid black;border-bottom:2px solid black;">Rate 1</th>
					<th style="border-bottom:2px solid black;">Rate 2</th>
					<th style="border-bottom:2px solid black;">Rate 3</th>
					<th style="border-right:2px solid black;border-bottom:2px solid black;">Total</th>				
				</tr>
				<?php 
				$count = 1;

				$select = mysqli_query($con, "SELECT * FROM metering_points,r_reading WHERE mp_cus_id = '$cus_id' AND rea_cus_id = '$cus_id' AND mp_id = rea_mp_id");

				$total_mp = 0;
				while($row = mysqli_fetch_assoc($select)){
					extract($row);
					if(date('Y', $rea_date)==$year && date('m', $rea_date)==$month){
						$total_mp++;
					}
				}

				$total_mp;
				$select = mysqli_query($con, "SELECT * FROM metering_points,r_reading WHERE mp_cus_id = '$cus_id' AND rea_cus_id = '$cus_id' AND mp_id = rea_mp_id");
				while($row = mysqli_fetch_assoc($select)){
					extract($row);

					if(date('Y', $rea_date)==$year && date('m', $rea_date)==$month){
						//selecting rates
						$sel = mysqli_query($con, "SELECT * FROM r_rate WHERE rate_reading_id = '$rea_id'");
						$ro = mysqli_fetch_assoc($sel);
						@extract($ro);
						
						//calling the function
						$cal = rate_calculate($cus_id, ($rea_date), $mp_id);
						
						if($count != $total_mp){
							echo '<tr>';
							echo '<td style="border-left:2px solid black; border-right:2px solid black;">'.$mp_name.'</td>';
							echo '<td style=" text-align:right;">'.number_format(@$rate_import_wh_1).'</td>';
							echo '<td style=" text-align:right;">'.number_format(@$rate_import_wh_2).'</td>';
							echo '<td style="border-right:2px solid black; text-align:right;">'.number_format(@$rate_import_wh_3).'</td>';
							echo '<td style=" text-align:right;">'.number_format(@$rate_export_wh_4).'</td>';
							echo '<td style=" text-align:right;">'.number_format(@$rate_export_wh_5).'</td>';
							echo '<td style=" text-align:right;">'.number_format(@$rate_export_wh_6).'</td>';
							echo '<td style="border-left:2px solid black; text-align:right;">'.number_format($cal[0]).'</td>';
							echo '<td style=" text-align:right;">'.number_format($cal[1]).'</td>';
							echo '<td style=" text-align:right;">'.number_format($cal[2]).'</td>';
							echo '<td style="border-right:2px solid black; text-align:right;">'.number_format($cal[3]).'</td>';				
							echo '</tr>';
						}else{			
							echo '<tr>';
							echo '<td style="border-left:2px solid black; border-bottom:2px solid black; ">'.$mp_name.'</td>';
							echo '<td style="border-left:2px solid black; border-bottom:2px solid black;  text-align:right;">'.number_format(@$rate_import_wh_1).'</td>';
							echo '<td style="border-bottom:2px solid black;  text-align:right;">'.number_format(@$rate_import_wh_2).'</td>';
							echo '<td style="border-bottom:2px solid black;  text-align:right;">'.number_format(@$rate_import_wh_3).'</td>';
							echo '<td style="border-left:2px solid black; border-bottom:2px solid black;  text-align:right;">'.number_format(@$rate_export_wh_4).'</td>';
							echo '<td style="border-bottom:2px solid black;  text-align:right;">'.number_format(@$rate_export_wh_5).'</td>';
							echo '<td style="border-bottom:2px solid black; text-align:right;">'.number_format(@$rate_export_wh_6).'</td>';
							echo '<td style="border-left:2px solid black; border-bottom:2px solid black;  text-align:right;">'.number_format($cal[0]).'</td>';
							echo '<td style="border-bottom:2px solid black;  text-align:right;">'.number_format($cal[1]).'</td>';
							echo '<td style="border-bottom:2px solid black;  text-align:right;">'.number_format($cal[2]).'</td>';
							echo '<td style="border-right:2px solid black; border-bottom:2px solid black; text-align:right; ">'.number_format($cal[3]).'</td>';
							echo '</tr>';
						}
						$count++;	

					}					
				}
				?>
			</tbody></table>
		</div>
	
	</div>
	<?php
	}
	
	public function viewInvoiceAction(){
		global $con;
		$id = portion(3);

	$select = mysqli_query($con, "SELECT * FROM customers WHERE cus_id = '$id'");
	$row = mysqli_fetch_assoc($select);
	extract($row);
	
	?>
	<div id="side">
		<h2 style="border-bottom:2px solid #ccc;margin-bottom:10px;"><i class="fa fa-fw fa-table"></i>Customer Invoice</h2>
		
		
		<?php 
		echo '<div class="nav files">	
		<a class="btn btn-default" href="" onclick="return print(\'printMe\');"><i class="fa fa-print fa-fw"></i> Print Invoice</a>
		</div>';
		?>
		<style>
		table tr th{ background-color:#ccc; color:black; }
		.hide{display:none;}
		.table tr:hover{
			background-color:none;
			background:none !important;
		}
		</style>
		<div id="printMe" style="width:900px;margin:auto;">
			<table border="0" cellspacing="0" cellpadding="2">
				<tbody>
				<tr valign="bottom">
					<td colspan="11">
					
					<img src="images/SIS.png" alt=""/><br/><br/>
						<!-- <hr style="border:1px solid black;"/> -->
						<br/><br/>
						<br/>
						<table style="font-size:20px;" class="" cellspacing="10">
							
							<tr>
								<td>Date:</td>
								<td><?php echo date('dS F Y h:i:s a'); ?></td>
							</tr>
							<tr valign="top">
								<td width="200px">Name of Customer:</td>
								<td><?php echo $customer_name; 
								echo returnFieldFromCustomer($cus_id, "customer_number").'<br>';
								echo returnFieldFromCustomer($cus_id, "customer_phone").'<br>';
								echo returnFieldFromCustomer($cus_id, "customer_email").'<br>';
								?>
								
								</td>
							</tr>
							<tr>
								<td>Detail:</td>
								<td>Engery Bill <?php 
								echo month_name($month = portion(4));
								echo ' ';
								echo $year = portion(5);
								?></td>
							</tr>
						</table>
						<?php
						$all_services = array();
						$sel = mysqli_query($con, "SELECT * FROM service");
						while($row = mysqli_fetch_assoc($sel)){
							extract($row);
							//$all_services[] = $se_id;
							
							$all_services[$service_name] = $service_rate;
						}
						
						$t1 = $t2 = $t3 = $t4 = 0;
						//summing all metering points
						//echo "SELECT * FROM metering_points,r_reading WHERE mp_cus_id = '$cus_id' AND rea_cus_id = '$cus_id' AND mp_id = rea_mp_id AND year(from_unixtime(rea_date)) = '$year' AND month(from_unixtime(from_unixtime(rea_date)) = '$month'";
						$select = mysqli_query($con, "SELECT * FROM metering_points,r_reading WHERE mp_cus_id = '$cus_id' AND rea_cus_id = '$cus_id' AND mp_id = rea_mp_id AND year(from_unixtime(rea_date)) = '$year' AND month(from_unixtime(rea_date)) = '$month'");
						$total_mp = mysqli_num_rows($select);
						while($row = mysqli_fetch_assoc($select)){
							extract($row);
														
							//calling the function
							$cal = rate_calculate($cus_id, ($rea_date), $mp_id);
							
							$t1 += $cal[0];
							$t2 += $cal[1];
							$t3 += $cal[2];
							$t4 += $cal[3];						
							
						}
						$ttu = $tta = 0;
						?><br/><br/>
						<table cellpadding="5" cellspacing="0" border="1" width="100%" class="table">
							<tr>
								<th>TIME BAND</th>
								<th>RATE</th>
								<th>QTY - KWh</th>
								<th>AMOUNT</th>
							</tr>
							<tr>
								<?php 
								$o = $all_services['Peak Sales - Local'];
								$ttu += $t1;
								$tta += $o*$t1;
								?>
								<td>Peak Sales - Local</td>
								<td align="right"><?php echo number_format($o); ?></td>
								<td align="right"><?php echo number_format($t1); ?></td>
								<td align="right"><?php echo number_format($o*$t1); ?></td>
							</tr>
							<tr>
								<?php 
								$o = $all_services['Shoulder Sales - Local'];
								$ttu += $t2;
								$tta += $o*$t2;
								?>
								<td>Shoulder Sales - Local</td>
								<td align="right"><?php echo number_format($o); ?></td>
								<td align="right"><?php echo number_format($t2); ?></td>
								<td align="right"><?php echo number_format($o*$t2); ?></td>
							</tr>
							<tr>
								<td>Off peak Local</td>
								<?php 
								$o = $all_services['Off peak Local'];
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
								<?php $tot = $tta*(18/100+1); if($tot<2000000000){?>
								Total in words:<?php echo '<b>'.number_to_words($tta*(18/100+1)).'</b>'; ?></td>
								<?php } ?>
							</tr>
						</table>
					
					</td>				
				</tr>
				
			</tbody></table>
		</div>
	
	</div>
	<?php
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
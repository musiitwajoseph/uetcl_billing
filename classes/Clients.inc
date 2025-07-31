<?php

Class Clients extends BeforeAndAfter{

	public $page = "CLIENTS";

	

	public function __construct(){

		$access = new AccessRights();

		//$access->pageAccess(user_id(), $this->page, 'V');

	}



	public function accountNumber(){

		$dm = date('y').date('m');

		$suffix = "K-";

		$db = new Db();

		$select = $db->select("SELECT client_account_number FROM clients WHERE client_account_number LIKE 'K-%'ORDER BY client_account_number DESC LIMIT 1");

		extract($select[0][0]);

		$client_account_number;

		$f = end(explode($suffix, $client_account_number))+1;



		$number = $suffix.str_pad($f, 4, "0", STR_PAD_LEFT);

		return $number;

	

	}

	

	public function getLinks(){

		$page = "CLIENTS";

		$links = array(

			array(

				"link_name"=>"Add Client", 

				"link_address"=>"clients/add-client",

				"link_icon"=>"fa-plus",

				"link_page"=>$page,

				"link_right"=>"A",

			),

			array(

				"link_name"=>"View Clients", 

				"link_address"=>"clients/view-clients",

				"link_icon"=>"fa-eye",

				"link_page"=>$page,

				"link_right"=>"A",

			),

		);

		

		return $links;

	}

	

	public function deleteDesignationAction(){

		$id = portion(3);

		$this->deletor("designation", "designation_id",  $id, 'designation/all-user-role');

	}

	

	public function addClientAction(){



		$access = new AccessRights();

		$access->pageAccess(user_id(), $this->page, 'A');



		$msg_pass = "Password should be at least 7 characters in length and should include at least an upper case letter(A-Z), lower case letter(a-z), number(0-9), and special character.(@ ? / # $ %) ";

		$db = new Db();

		if(isset($_POST['submit'])){

			

			$account_number = addslashes(ucwords(strtolower($_POST['account_number'])));	

			$location = addslashes(ucwords(strtolower($_POST['location'])));	

			$surname = addslashes(ucwords(strtolower($_POST['surname'])));			

			$othername = addslashes(ucwords(strtolower($_POST['othername'])));			

			$username = addslashes($_POST['username']);			

			$email = addslashes($_POST['email']);				

			$telephone = addslashes($_POST['telephone']);		

			$nok = addslashes($_POST['nok']);		

			$gender = addslashes($_POST['gender']);

			$sen = $_POST['sen'];

			

			$user_id = user_id();

			$time = time();

			

			$errors = array();



			if(empty($account_number)){

				$errors[] = "Please Enter Account Number";

			}

			if(empty($location)){

				$errors[] = "Please Enter Location";

			}

			if(empty($othername)){

				$errors[] = "Please Enter othername";

			}

			// if(empty($email)){

			// 	$errors[] = "Please Enter your Email Address";

			// }

			if(empty($telephone)){

				$errors[] = "Enter telephone number";

			}else{

				if(strlen($telephone) < 10 || strlen($telephone) > 13){

					$errors[] = "Invalid Telephone Number";

				}

				if(!preg_match("/^\d+$/", $telephone)){
					$errors[] = "Telephone Number must should only contains digits";
				}

				if (strncmp($telephone, "256", 3) !== 0) {
					$errors[] = "Telephone Number must start with: <b>256</b>";
				}

			}

			// if(empty($nok)){

			// 	$errors[] = "Enter Next of Kin";

			// }

			

			// if($this->isThere("clients", ["client_account_number"=>$account_number])){

			// 	$errors[] = "Account Number $account_number already exists";

			// }



			if(empty($errors)){

				$password = Feedback::password_generator();



				$insert = $db->insert("clients", [

				"client_account_number"=>$account_number, 

				"client_surname"=>$surname,

				"client_othername"=>$othername,				

				"client_email"=>$email,

				"client_telephone"=>$telephone,

				"client_password"=>$this->penc($password),

				"client_date_added"=>$time,

				"client_added_by"=>$user_id,

				"client_nok"=>$nok,

				"client_gender"=>$gender,

				"client_location"=>$location,

				

				]);

				

				echo $db->error();

				if($insert){



					if(!empty($email)){

						$msg = array();

		                $msg[] = "Hello $othername, ";

		                $msg[] = "";

		                $msg[] = "Your Account has been successfully created by :".$this->full_name(user_id());

		                $msg[] = "";

		                $msg[] = "Username: $telephone";

		                

		                if(!empty($password)){

		                	 $msg[] = "Password: $password";

		            	}

		               

		                $mssg[] = "";

		                $message[] = "Thank You.";



		                $to = $email;

		                $subject = "KUSIIMA FINANCIAL SERVICES ACCOUNT DETAILS";

		                $message = implode("\r \n <br/>", $msg);

		                //Feedback::sendmail($to,$subject,$message,$name);

		            }


		            if($sen){
	               		Feedback::sms("Hello $surname $othername, KUSIIMA welcomes to its Services", $telephone);
	                	Feedback::smsm("$surname $othername, has been added");
	                }


					FeedBack::success();

					FeedBack::refresh();





				}else{

					FeedBack::error('Not Saved, '.$db->error());

				}

			}else{

				FeedBack::errors($errors);

			}

					

		}	

		

		//echo $this->branch_name(3);

			

		?>

		<div class="col-lg-12">

			<div class="panel panel-default">

				<div class="panel-heading">

					Add User Form

				</div>

				<div class="panel-body">

					<div class="row">



						<form role="form" action="" method="post">

							<div class="col-md-12">

								<div id="must">All field with asterisk(*) are mandatory.</div>

							</div>

							<div class="clearfix"></div>

							<div class="col-lg-2">

								<div class="form-group">

									<label>Account Number<span class="must">*</span></label>

									<div class="form-line">



										<input class="form-control" type="text" autocomplete="off" name="account_number" placeholder="Enter Account No." value="<?php echo $this->accountNumber(); ?>">

									</div>

								</div>

							</div>

							<div class="col-lg-3">

								<div class="form-group">

									<label>Surname<span class="must">*</span></label>

									<div class="form-line">

										<input class="form-control" type="text" autocomplete="off" name="surname" value="<?php echo @$surname; ?>">

									</div>

								</div>

							</div>

							<div class="col-lg-2">

								<div class="form-group">

									<label>Othername(s)<span class="must"></span></label>

									<div class="form-line">

										<input class="form-control" type="text" autocomplete="off" name="othername" value="<?php echo @$othername; ?>">

									</div>

								</div>

							</div>

							<div class="col-lg-2">

								<div class="form-group">

									<?php if(empty($telephone)) $telephone = '256'; ?>

									<label>Telephone Phone<span class="must">*</span></label>

									<div class="form-line">

										<input class="form-control" type="phone" autocomplete="off" name="telephone" value="<?php echo @$telephone; ?>">

									</div>

								</div>

							</div>							

							<div class="col-lg-3">

								<div class="form-group">

									<label>Email Address<span class="must">*</span></label>

									<div class="form-line">

										<input class="form-control" type="email" autocomplete="off" name="email" value="<?php echo @$email; ?>">

									</div>

								</div>

							</div>							

							<div class="col-lg-2">

								<div class="form-group">

									<label>Gender<span class="must"></span></label>

									<div class="form-line">

										<select name="gender">

											<?php 

											if($gender ==1)

												echo '<option selected="selected" value="1">Male</option>';

											else

												echo '<option value="1">Male</option>';



											if($gender ==0)

												echo '<option selected="selected" value="0">Female</option>';

											else

												echo '<option value="0">Female</option>';

											?>

										</select>

									</div>

								</div>

							</div>													

							<div class="col-lg-4" style="display:none;" title="<?php echo $msg_pass; ?>" class="form-control" type="password">

								<div class="form-group">

									<label>Password<span class="must">*</span></label>

									<div class="form-line">

										<input class="form-control" type="password" name="password" value="<?php echo @$password; ?>">

									</div>

								</div>

							</div>							

							<div class="col-lg-4" style="display:none;">

								<div class="form-group">

									<label>Confirm Password<span class="must">*</span></label>

									<div class="form-line">

										<input class="form-control" type="password" name="cpassword" value="<?php echo @$cpassword; ?>">

									</div>

								</div>

							</div>	

							<div class="col-lg-2">

								<div class="form-group">

									<label>Next of Kin<span class="must">*</span></label>

									<div class="form-line">

										<input class="form-control" type="text" autocomplete="off" name="nok" value="<?php echo @$nok; ?>">

									</div>

								</div>

							</div>

							<div class="col-lg-2">

								<div class="form-group">

									<label>Location<span class="must">*</span></label>

									<div class="form-line">

										<input class="form-control" type="text" autocomplete="" name="location" value="<?php echo @$location; ?>">

									</div>

								</div>

							</div>	

							<div class="col-lg-12">

								<?php
								if($sen)
			            			echo '<input value="1" type="checkbox" id="sen" checked name="sen"/><label style="line-height:20px; margin-top:20px;" for="sen">Send SMS</label>';
				        		else
				            		echo '<input value="1" type="checkbox" id="sen" name="sen"/><label style="line-height:20px; margin-top:20px;" for="sen">Send SMS</label>';
				            	echo '<br/><br/>';

								?>

								<button type="submit" onclick="return confirm('Are you sure you want to save.');"name="submit" class="btn btn-primary"><i class="fa fa-fw fa-save"></i> Save</button>

							</div>

						</form>

					</div>

				</div>

			</div>

		</div>

		<?php

	}



	public function viewClientsAction(){

		$access = new AccessRights();

	?>

		<div class="col-md-12">

			<?php 



	echo '<form action="" method="post">';



        if(isset($_POST['search'])){

            $month = $_POST['month'];

            $year = $_POST['year'];



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

            $from = $_POST['from'];

            $to = $_POST['to'];



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



        echo '<div class="clearfix"></div><br/>';



        echo '<div style="float:left; margin-right:20px;">';

        echo '<label>Year&nbsp;&nbsp;</label>';

        echo '<select name="year">';

        echo '<option value="">Select</option>';

        // if($year == "all")

        //  echo '<option selected value="all">All</option>';

        // else

        //  echo '<option value="all">All</option>';

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

                echo '<option selected value="'.$i.'">'.$this->month_name($i).'</option>';

            else

                echo '<option value="'.$i.'">'.$this->month_name($i).'</option>';

        }

        echo '</select>';

        echo '</div>';

        

        echo '<div style="float:left; margin-right:20px;">';

        echo '<button type="submit" name="search" class="btn btn-xs btn-primary pt-0 pb-0"><i class="fa fa-fw fa-search"></i> Show &nbsp; </button>';

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

        echo '<button type="submit" name="searchPeroid" class="btn btn-xs btn-primary pt-0 pb-0"><i class="fa fa-fw fa-search"></i> Show &nbsp; </button>';

        echo '</div>';



        echo '<div class="clearfix"></div>';

        echo '</form>';

        echo '</div>';

        echo '<div class="clearfix"></div>';



        echo '<hr class="mb-5" />';

        echo '<input type="text" name="search" placeholder="Search Client\'s Name" id="searchTerm" onkeyup="doSearch()" onClick="this.select();">';

        echo '<div class="clearfix"></div>';


        if($start_date && $end_date){

            $sd = FeedBack::date_fm($start_date);

            $ed = FeedBack::date_fm($end_date);

            

            $ti = "All Customers from $sd to $ed";


            echo "<h5 class='pull-left'>$ti</h5>";



        }

        $this->start_print($ti);

        $db = new Db();

        

        $sql = "SELECT * FROM clients, loan_details WHERE client_id = ld_customer_id AND ld_date_added >= '$start_date' AND ld_date_added <= '$end_date' ORDER BY ld_date_added ASC, client_date_added DESC";



			$db = new Db();

			$select = $db->select("SELECT * FROM clients ORDER BY client_date_added DESC ");

			

			if(!$select){

				echo $db->error();

			}else{

				echo '<table style="font-size:10px;" cellspacing="0" cellpadding="2" border="1" width="100%" id="table">';

				echo '<thead>';

				echo '<tr valign="bottom">';

				echo '<th width="30px">No.</th>';

				echo '<th width="90px" class="hide2">Date Added</th>';

				echo '<th>Account No.</th>';

				echo '<th>Name</th>';

				echo '<th>Telephone</th>';

				echo '<th>Location</th>';

				echo '<th class="hide hide2">Email</th>';

				echo '<th class="hide">Next of Kin</th>';

				echo '<th>Savings</th>';

				echo '<th>Loan</th>';

				echo '<th>Fine</th>';

				echo '<th>Paid</th>';

				echo '<th>Balance</th>';

				if($access->sectionAccess(user_id(), $this->page, 'E') || $access->sectionAccess(user_id(), $this->page, 'V')){

					echo '<th class="hide2" width="180px">Action</th>';

				}

				echo '</tr>';

				echo '</thead>';

////////////////////////////////////report step1/////////////////////////////////////

				

				$db_values = array();

				$db_values[] = array(

					

					"No",

					"Name",

					"Check No.",

					"Department",

					"User Role",

					"Last Logged in",

					"Online/<br/>Offline",

					"Status",

				);

////////////////////////////////////////////////////////////////////////////////				

				$i=1;
				$total_fine = 0;

				echo '<tbody>';

				foreach($select[0] as $row){

					extract($row);

					echo '<tr>';

					echo '<td><center>'.($i++).'.</center></td>';

					echo '<td class="hide2">'.Feedback::date_s($client_date_added).'</td>';

					echo '<td>'.$client_account_number.'</td>';

					echo '<td>'.strtoupper($client_surname.' '.$client_othername).'</td>';

					echo '<td>'.($client_telephone).'</td>';

					echo '<td>'.($client_location).'</td>';

					echo '<td class="hide hide2">'.($client_email).'</td>';

					echo '<td class="hide">'.($client_nok).'</td>';



					$select = $db->select("SELECT sum(savings_amount) as tot FROM savings WHERE savings_customer_id = '$client_id'");

	    			extract($select[0][0]);

					echo '<td align="right">'.number_format($tot).'</td>';



					$select = $db->select("SELECT sum(ld_loan_amount+ld_loan_amount*ld_rate/100) as tot FROM loan_details WHERE ld_customer_id = '$client_id'");

	    			extract($select[0][0]);

					$select = $db->select("SELECT ld_id FROM loan_details WHERE ld_customer_id = '$client_id'");
					$fine = 0;
					foreach($select[0] as $row){
						extract($row);
						$fine += $this->fineFinder($ld_id);
					}
					$total_fine += $fine;

	    			extract($select[0][0]);



					$select = $db->select("SELECT sum(rec_amount) as rec FROM receipts WHERE rec_customer_id = '$client_id'");

	    			extract($select[0][0]);

	    				

	    			echo '<td align="right">'.number_format($tot).'</td>';
	    			echo '<td align="right">'.number_format($fine).'</td>';

	    			echo '<td align="right">'.number_format($rec).'</td>';



	    			if($tot == 0){

	    				echo '<td align="right">-&nbsp;&nbsp;</td>';

	    			}else{

	    				echo '<td align="right">'.number_format($tot+$fine-$rec).'</td>';

	    			}





					//if($access->sectionAccess(user_id(), $this->page, 'E') || $access->sectionAccess(user_id(), $this->page, 'V'))

					{

						echo '<td class="hide2">';

						if($access->sectionAccess(user_id(), $this->page, 'V'))

							echo $this->action('view','dashboard/loan-details/'.$client_id, 'Loan');

							echo '&nbsp;&nbsp;';



						if($access->sectionAccess(user_id(), $this->page, 'V'))

							echo $this->action('view','savings/index/'.$client_id, 'Savings');

							echo '&nbsp;&nbsp;';



						if($access->sectionAccess(user_id(), $this->page, 'E'))

							echo $this->action('edit','clients/edit-client/'.$client_id, 'Settings');

						

						

						echo '</td>';

					}

					echo '</tr>';



					

					$db_values[] = array(

						($i),

						($user_surname.' '.$user_othername),

						$this->rgf("department", $user_department_id, "dept_id", "dept_name"),

						

						$this->rgf("designation", $user_designation, "designation_id", "designation_name"),



						Feedback::date_fm($user_last_logged_in),

						$sstt,

						

						(($user_active)?"Active":"Locked"),



						

						); 

				}

				echo '<tr><th colspan="12"></th></tr>';

				echo '</tbody>';

				

				echo '</table><br/>';

				$this->end_print();

//////////////////////////////report step3/////////////////////////////////////

				

				$t = new TableCreator();

				$heading = "SYSTEM USER LIST"; //CHANGE

				$t->open($this->full_name(user_id()), $heading);

				$t->thd($db_values);

				$t->close();

				$t->results();



				$e = new Exporter();

				echo $e->getDisplay($heading, $t->results());

////////////////////////////////////////////////////////////////////////////////////

			}

			

			?>

		</div>

		<script type="text/javascript">
			
			
		</script>

		<?php

	}



	public function editClientAction(){



		$access = new AccessRights();

		$access->pageAccess(user_id(), $this->page, 'E');

		

		$db = new Db();

		$id = portion(3);



		$select = $db->select("SELECT * FROM clients WHERE client_id = '$id'");

		extract($select[0][0]);



		$account_number = $client_account_number;

		$surname = $client_surname;

		$othername = $client_othername;

		$telephone = $client_telephone;

		$email = $client_email;

		$gender = $client_gender;

		$nok = $client_nok;



		if(isset($_POST['submit'])){

			

			$account_number = addslashes(ucwords(strtolower($_POST['account_number'])));	

			$surname = addslashes(ucwords(strtolower($_POST['surname'])));			

			$othername = addslashes(ucwords(strtolower($_POST['othername'])));			

			$username = addslashes($_POST['username']);			

			$email = addslashes($_POST['email']);				

			$telephone = addslashes($_POST['telephone']);		

			$nok = addslashes($_POST['nok']);		

			$gender = addslashes($_POST['gender']);

			

			$user_id = user_id();

			$time = time();

			

			$errors = array();



			if(empty($account_number)){

				$errors[] = "Please Enter Account Number";

			}

			if(empty($othername)){

				$errors[] = "Please fill othername";

			}

			if(empty($email)){

				//$errors[] = "Please Enter Email Address";

			}

			if(empty($telephone)){

				$errors[] = "Enter telephone number";

			}else{

				if(strlen($telephone) < 12 || strlen($telephone) > 12){

					$errors[] = "Invalid Telephone Number";

				}

				
				if(!preg_match("/^\d+$/", $telephone)){
					$errors[] = "Telephone Number must should only contains digits";
				}

				if (strncmp($telephone, "256", 3) !== 0) {

					$errors[] = "Telephone Number must start with: <b>256</b>";

				}

			}

			if(empty($nok)){

				$errors[] = "Enter Next of Kin";

			}

			

			if($this->isThereEdit("clients", ["client_account_number"=>$account_number, "client_id"=>$id])){

				$errors[] = "Account Number $account_number already exists";

			}



			if(empty($errors)){

				$password = Feedback::password_generator();



				$insert = $db->update("clients", [

				"client_account_number"=>$account_number, 

				"client_surname"=>$surname,

				"client_othername"=>$othername,				

				"client_email"=>$email,

				"client_telephone"=>$telephone,

				"client_password"=>$this->penc($password),

				"client_date_added"=>$time,

				"client_added_by"=>$user_id,

				"client_nok"=>$nok,

				"client_gender"=>$gender,

				

				],["client_id"=>$client_id]);

				

				echo $db->error();

				if($insert){

	                //Feedback::sms("Hello $surname $othername, KUSIIMA welcomes to its Services", $telephone);



					FeedBack::success();

					FeedBack::refresh();

				}else{

					FeedBack::error('Not Saved, '.$db->error());

				}

			}else{

				FeedBack::errors($errors);

			}

					

		}	

		

		//echo $this->branch_name(3);

			

		?>

		<div class="col-lg-12">

			<div class="">

				<p id="" data-toggle="modal" data-target="#insertimageModal" id="" class="btn btn-dark" type="botton"><i class="fa fa-camera"></i> Change Profile Picture</p>

				<br/><br/>

			<img  class="avatar avatar-100 photo activeImage" id="activeImage" src="<?php 

			if($client_image)

				echo return_url().''.$client_image;

			else

				echo return_url().'assets/images/blog/author.jpg'; 

			?>" alt="">

			</div>

			<!-- class="modal vd_mdl" id="insertimageModal" tabindex="-1" role="dialog" aria-hidden="true" style="width:" -->

			<?php $_SESSION['CLIENT_ID'] = $client_id; ?>

			<input type="hidden" id="urlPath" value="<?php echo return_url(); ?>"/>

			



			<?php





			?>





			<div class="panel panel-default">

				<div class="panel-heading">

					Update Client Form

				</div>

				<div class="panel-body">

					<div class="row">



						<form role="form" action="" method="post">

							<div class="col-md-12">

								<div id="must">All field with asterisk(*) are mandatory.</div>

							</div>

							<div class="clearfix"></div>

							<div class="col-lg-2">

								<div class="form-group">

									<label>Account Number<span class="must">*</span></label>

									<div class="form-line">

										<input class="form-control" type="text" autocomplete="off" name="account_number" placeholder="Enter Account No." value="<?php echo @$account_number; ?>">

									</div>

								</div>

							</div>

							<div class="col-lg-3">

								<div class="form-group">

									<label>Surname<span class="must">*</span></label>

									<div class="form-line">

										<input class="form-control" type="text" autocomplete="off" name="surname" value="<?php echo @$surname; ?>">

									</div>

								</div>

							</div>

							<div class="col-lg-2">

								<div class="form-group">

									<label>Othername(s)<span class="must"></span></label>

									<div class="form-line">

										<input class="form-control" type="text" autocomplete="off" name="othername" value="<?php echo @$othername; ?>">

									</div>

								</div>

							</div>

							<div class="col-lg-2">

								<div class="form-group">

									<?php if(empty($telephone)) $telephone = '256'; ?>

									<label>Telephone Phone<span class="must">*</span></label>

									<div class="form-line">

										<input class="form-control" type="phone" autocomplete="off" name="telephone" value="<?php echo @$telephone; ?>">

									</div>

								</div>

							</div>							

							<div class="col-lg-3">

								<div class="form-group">

									<label>Email Address<span class="must">*</span></label>

									<div class="form-line">

										<input class="form-control" type="email" autocomplete="off" name="email" value="<?php echo @$email; ?>">

									</div>

								</div>

							</div>							

							<div class="col-lg-2">

								<div class="form-group">

									<label>Gender<span class="must"></span></label>

									<div class="form-line">

										<select name="gender">

											<?php 

											if($gender ==1)

												echo '<option selected="selected" value="1">Male</option>';

											else

												echo '<option value="1">Male</option>';



											if($gender ==0)

												echo '<option selected="selected" value="0">Female</option>';

											else

												echo '<option value="0">Female</option>';

											?>

										</select>

									</div>

								</div>

							</div>

							<div class="col-lg-2">

								<div class="form-group">

									<label>Next of Kin<span class="must">*</span></label>

									<div class="form-line">

										<input class="form-control" type="text" autocomplete="off" name="nok" value="<?php echo @$nok; ?>">

									</div>

								</div>

							</div>	

							<div class="col-lg-12">

								<button type="submit" onclick="return confirm('Are you sure you want to save.');"name="submit" class="btn btn-primary"><i class="fa fa-fw fa-save"></i> Save</button>

							</div>

						</form>

					</div>

				</div>

			</div>



			<div >

			  <div class="modal-dialog modal-lg" role="document">

			  <div class="modal-content">

				  <div class="modal-header">

					<button type="button" class="close" data-dismiss="modal">&times;</button>

					<h4 class="modal-title">Upload, Crop & Save</h4>

				  </div>

				  <div class="modal-body">

				  	<div id="imageUpload">

				  		<div class="custom-file">

							<input accept="image/*" type="file" class="custom-file-input" id="ProfilePicture">

							<label class="custom-file-label" for="inputGroupFile04" id="profilePictureLabel">Browse and Upload image</label>

						</div>

						<script>

							function readURL5(input) {

							  if (input.files && input.files[0]) {

							    var reader = new FileReader();

							    

							    reader.onload = function(e) {

							      $('#image_demo_file').attr('src', e.target.result);

							    }

							    

							    reader.readAsDataURL(input.files[0]); // convert to base64 string

							  }

							}



							$("#ProfilePicture1").change(function() {

							  readURL5(this);

							});

						</script>

				  	</div>

					<div class="row" id="imgcontent">

					  <div class="col-md-8 text-center">

						

						<div id="image_demo" style="width:150px; height: 250px; margin-top:30px;"></div>

						<BR/><BR/><BR/>

					  </div>

					  <div class="col-md-4" style="padding-top:30px;">

					

						<button  style="margin-bottom:10px;" class="btn btn-success crop_image">Crop & Insert Image</button>

						<button  style="margin-bottom:10px;" class="btn btn-success" id="RotateAntiClockwise" class="btn btn-primary" title="Rotate anti-clockwise"><i class="fa fa-redo"></i> </button>

						<button  style="margin-bottom:10px;" class="btn btn-success" id="RotateClockwise" class="btn btn-primary" title="Rotate clockwise"><i class="fa fa-undo"></i> </button>

						<div id="SaveStatusPP"></div>

					  </div>

					</div>

					</div>

				  <div class="modal-footer">

					<button type="button" class="btn btn-default" data-dismiss="modal">Close</button>

				  </div>

				  </div>

				</div>

			</div>

		</div>

		<?php

	}





	

	public function deleteValidNameAction(){

		$id = portion(3);

		$this->deletor("valid_names", "valid_name_id",  $id, 'valid-names-and-addresses/all-valid-names');

	}



}
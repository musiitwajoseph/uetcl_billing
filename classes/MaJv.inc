<?php

class MaJv extends BeforeAndAfter
{
    //
	public $type = "";
	public $choice = "Aprove";
	public $comment = "";
	public $reminder = "";
	public $count = "";
	public $id = "";
	public $requestor = "";
	public $reference = "";
	public $counter_field = "";

    public static function form($id, $where, $count, $notify_user, $ref, $label, $table, $status, $id_field, $counter_field, $status_value){

		$id_form = $id;

		$bf = new BeforeAndAfter();
		?>
		<style>
			#MasterApprovalReminderSection{
				display: none;
			}
		</style>
		<?php
		$verified = $total = 0;
		
		$db = new Db();
		$list = $db->select("SELECT TOP 1 * FROM master_approval_orders WHERE app_type = '$where' AND app_count = '$count'");
		extract($list[0][0]);

		if(@!$app_reprogrammed){
			$actions = array('approve'=>'Approve', 'reject'=>'Reject', 'amend'=>'RequestMoreInformation', 'onhold'=>'OnHold');
			foreach($actions as $action => $value){		
				if(${'app_btn_'.$action}){	
					$label = ${'app_btn_'.$action.'_label'};
					$label = (empty($label)) ? $value : $label ;
					$action_list[$label] = $value;		
				}		
			}	
		}else{
			$action_list = array(
				"Approve" => "Approve",
				"Reject" => "Reject",
				"Request More Information from Supplier" => "RequestMoreInformation",
				// "On hold" => "OnHold",
			); 
		}
		

		echo '<input type="hidden" value="'.$table.'" id="MasterTable">';
		echo '<input type="hidden" value="'.$status.'" id="MasterStatus">';
		echo '<input type="hidden" value="'.$id_field.'" id="MasterIDField">';
		echo '<input type="hidden" value="'.$where.'" id="MasterApprovalType">';
		echo '<input type="hidden" value="'.$count.'" id="MasterApprovalCount">';
		echo '<input type="hidden" value="'.$id_form.'" id="MasterApprovalID">';
		echo '<input type="hidden" value="'.$notify_user.'" id="MasterApprovalRequestor">';
		echo '<input type="hidden" value="'.$ref.'" id="MasterApprovalReference">';
		
		echo '<form>';
		echo '<div class="row">';

		echo '<div style="background-color:#ffd0cc36; border:1px solid red; border-radius:5px; padding-bottom:0; margin-bottom:0;margin-left:15px;width:99%;">';

		echo '<div class="row">';
		
		if($app_signature || $app_attachment)
			echo '<div class="col-xl-6 col-lg-12 col-md-12">';
		else
			echo '<div class="col-xl-12 col-lg-12 col-md-12">';

		echo '<div class="col-md-12">';
	

		echo '<label class="fs-12 font-weight-bold">Action by '.$bf->full_name(user_id()).'</label>';
		echo '<select id="MasterApprovalChoice" class="form-control">';

        //accepted buttons
        
		echo '<option value = "" disabled selected>Select Action</option>';


        foreach($action_list as $item=>$item_value){

            if((int)$status_value == -3){
                echo '<option value = "MoreInformation">More Information</option>';
                break;
            }else
			{
                if(1){//check accepted buttons
                    if(0)
                        echo '<option value="'.$item_value.'">'.$label.' : '.$bf->full_name(user_id()).'</option>';
                    else
                        echo '<option value="'.$item_value.'">'.$item.'</option>';
                }
            }
        }

		echo '</select>';

		
		// if(is_array($extra_field)){
		// 	foreach($extra_field as $ef){
		// 		echo '<div class="col-md-3" style="padding-top:10px;"/>';
		// 		echo '<label>'.$ef[0].'</label>';
		// 		echo '<input type="text" value="" />';
		// 		echo '</div>';
		// 	}
		// }

		echo '<div id="MasterApprovalReminderSection"class="col-md-12" style="padding-top:10px;">';
		echo '<label class="fs-12 font-weight-bold">';
		echo 'Reminder Date: ';
		echo '<label>';
		echo '<input id="MasterApprovalReminderDate" type="date" class="date form-control" value=""/>';
		echo '</div>';
		echo '<input id="MasterCounterField" type="hidden" class="date form-control" value="'.$counter_field.'"/>';

		echo '<div class="clearfix"></div>';
		
		echo '<div class="" style="padding-top:10px;">';
		echo '<label class="fs-12 font-weight-bold">';
		echo 'Comment/Reason: ';
		echo '</label>';

		echo '<button type="button" class="btn btn-sm btn-gray btn-pill p-0 pull-right" id="generateMessagesMaster" style="display:hidden;"> &nbsp; Auto Generate Message &nbsp; </button>';
        echo '<div id="loadGeneratedMessagesMaster"></div>';

		echo '<textarea placeholder="Enter Comment/Reason" style="height:100px;" maxlength="200" id="MasterApprovalComment" class="form-control"></textarea>';
		echo '<span id="MasterApprovalCharacterCount"></span>';
		echo '</div>';

		echo '<div class="clearfix"></div>';

		
		echo '<div>';

		echo '</div>';

		

		echo '<div class="clearfix"></div>';

		echo '<div class="row" style="padding:15px;">';

		echo '<div class="col-md-6" style="padding-top:10px;">';
		echo '<button class="btn btn-success" type="button" id="MasterApprovalPost"><i class="fa fa-fw fa-plane"></i> Post</button>';
		echo '<span style="padding-left:10px;" id="MasterApprovalPostStatus"></span>';
		echo '</div>';

		
		

		echo '</div>';

		echo '</div>';
		echo '</div>';

		
		if($app_signature || $app_attachment){

			echo '<input type="hidden" id="isSignature" value="1"/>';

			echo '<div class="col-xl-6 col-lg-12 col-md-12">';

			echo '<div class="card">'; 
			echo '<div class="card-body">'; 
			echo '<div class="card-pay">'; 

			echo '<ul class="tabs-menu nav">'; 

			if($app_signature)
				echo '<li class=""><a href="#tab20" class="active" data-toggle="tab"><i class="fa fa-pencil-square-o"></i> Draw Signature</a></li>'; 

			if($app_attachment)
				echo '<li><a href="#tab21" data-toggle="tab" class=""><i class="fa fa-upload"></i> Upload Signature</a></li>'; 

			echo '</ul>'; 
			
			echo '<div class="tab-content">'; 
			if($app_signature){
				echo '<div class="tab-pane active show" id="tab20">'; 			
				echo '<canvas id="signaturePad"></canvas><br>';
				echo '<button type="button" id="clear" class="btn btn-sm btn-pill btn-danger"> &nbsp; <i class="fa fa-times"></i> Clear &nbsp; </button>';
				echo '</div>'; 
				?>
							
				<script>
					$(document).ready(function() {
						const canvas = document.getElementById('signaturePad');
						const ctx = canvas.getContext('2d');
						let isDrawing = false;

						canvas.width = 400;
						canvas.height = 200;

						canvas.addEventListener('mousedown', (e) => {
							isDrawing = true;
							ctx.beginPath();
							ctx.moveTo(e.offsetX, e.offsetY);
						});

						canvas.addEventListener('mousemove', (e) => {
							if (isDrawing) {
								ctx.lineTo(e.offsetX, e.offsetY);
								ctx.stroke();
							}
						});

						canvas.addEventListener('mouseup', () => {
							isDrawing = false;
						});

						$('#clear').click(function() {
							ctx.clearRect(0, 0, canvas.width, canvas.height);
						});

					});
				</script>

				<?php
			}

			if($app_attachment){
				echo '<div class="tab-pane h-200" id="tab21">'; 			
				echo '<input type="file" value="" id="uploadFile"/>';
				echo '<br>';
				echo '<img id="preview" src="" alt="Image Preview">';
				echo '</div>'; 

				?>
							
				<script>
					$(document).ready(function() {
						$('#uploadFile').on('change', function(event) {
							var file = event.target.files[0];
							if (file) {

								var validTypes = ['image/jpeg', 'image/png', 'image/gif', 'image/webp'];
								if ($.inArray(file.type, validTypes) === -1) {
									alert('Please select a valid image file (JPG, PNG, GIF, WEBP).');
									$('#preview').hide();
									return;
								}

								var reader = new FileReader();
								reader.onload = function(e) {
									$('#preview').attr('src', e.target.result).show(); 
								};
								reader.readAsDataURL(file);

							} else {
								$('#preview').hide(); 
							}
						});
					});
				</script>

				<?php			
			}
			echo '</div>'; 

			echo '</div>'; 
			echo '</div>'; 
			echo '</div>'; 		

		}else{			
			echo '<input type="hidden" id="isSignature" value="0"/>';
		}

		echo '</div>';
		
	 //=========================	


		echo '</div>';



		echo '</div>';
        
		echo '</form>';	

		?>
		
		<script src="<?php echo return_url().'jquery-3.4.1.min.js'; ?>"></script>
		<script type="text/javascript" src="<?php echo return_url(); ?>js/sweetalert.min.js"></script>
		
		<script type="text/javascript">
		$(document).ready(function(){
			$(function(){
				function showLoading() {
					$('#loading-gif').show();
				}

				// Hide loading GIF
				function hideLoading() {
					$('#loading-gif').hide();
				}

				
		$('#generateMessagesMaster').click(function(){
            var status = $('#MasterApprovalChoice').val();
            if(!status){ alert('Select status'); return false; }            
			fetchMessages(0, status);
        });

        $('#generateMessagesMaster').hide();

        $(document).on('click', '.add-in-message', function(){
            $('#MasterApprovalComment').html($(this).html());
        });
        $(document).on('click', '.add-in-message-append', function(){			
			var textarea = $('#MasterApprovalComment');

			var start = textarea[0].selectionStart;
            var end = textarea[0].selectionEnd;
			var text = $(this).attr('data-append');

			var currentValue = textarea.val();
			if (currentValue.length + text.length <= 200) {

				$('#MasterApprovalCharacterCount').html('Remaining Characters: '+(200 - currentValue.length - text.length));

				textarea.val(currentValue.slice(0, start) + text + currentValue.slice(end));

				textarea[0].selectionStart = textarea[0].selectionEnd = start + text.length;
				textare.focus();
			}

        });

        $('#MasterApprovalChoice').change(function(){
            $('#loadGeneratedMessagesMaster').html('');
            $('#generateMessagesMaster').show();
			status = $(this).val();
			fetchMessages(0, status);
        });

		function fetchMessages(where, status) {
			
			$.ajax({
				url: '<?php return_url().'ajax/ai-jv.php'; ?>',
				type: 'GET',
				dataType: 'json',
				data: { status: status},
				success: function(data) {
					
					if (Array.isArray(data)) {
						let messageHtml = '<ul style="display:inline-block;">';
						data.forEach(function(message) {
							messageHtml += '<li style="display:inline-block; background-color: beige; border: 1px solid #ecb403; padding: 1px 2px 2px 15px; margin: 1px 5px 0 0; border-radius: 10px;cursor:pointer;"><span title="Add Text" class="add-in-message">' + message + '</span><div style="display:inline-block; color:#fff;background-color:black; border-radius:15px; line-height:15px; height:15px; width:15px;text-align:center; margin-left:5px; cursor:pointer;" class="add-in-message-append" data-append="' + message + '" title="Append">+<div></li>';
						});
						messageHtml += '</ul><i><small>Please Click and read through</i></small>';
						$('#loadGeneratedMessagesMaster').html(messageHtml);
					} else {
						$('#loadGeneratedMessagesMaster').html('<p>' + data.error + '</p>');
					}
				},
				error: function(xhr) {
					$('#loadGeneratedMessagesMaster').html('<p>An error occurred while fetching messages.</p>');
				}
			});
			
		}

				
			
				$(document).on('click', '#saveSubcategoryEvaluation', function(){

$('#largemodal2').modal('show');
var form_id = $('#form_id').val();
var status = $('#status').val();
var reason = $('#reason').val();
var sup_id = $('#sup_id').val();

if(!confirm("Do you really want to save?")){
	return false;
}

if(!status || !reason){
	swal.fire({
		title: "Error",
		text:"Please select Status and Reason/Comment",
		type:"error",
		icon:"error",
	});

	return false;
}

var formData = new FormData();
formData.append('form_id', form_id);
formData.append('status', status);
formData.append('reason', reason);
formData.append('sup_id', sup_id);

$.ajax({
	type: "post",
	processData: false,
	contentType: false,
	cache: false,
	data: formData,
	url: '/suppliers/save-subcategories',
	headers: {
		'X-CSRF-TOKEN': $('meta[name="csrf-token"]').attr('content')
	},
	success: function(data) {    
		if(data.status){
			swal.fire({
				title: "Success",
				text: data.message,
				type:"success",
				icon:"success",
			}).then((result)=>{
				location.reload();
			});
		}else{
			swal.fire({
				title: "Error",
				text: 'Error',
				type:"error",
				icon:"error",
			});
		}             
	},
	error: function(data) {
		$('body').html(data.responseText);
	}
});

});	
			
			$(document).on('click', '#saveCriteria', function(){

			var form_id = $('#form_id').val();
			var status = $('#status').val();
			var reason = $('#reason').val();
			var sup_id = $('#sup_id').val();

			if(!confirm("Do you really want to save?")){
				return false;
			}

			if(!status || !reason){
				swal.fire({
					title: "Error",
					text:"Please select Status and Reason/Comment",
					type:"error",
					icon:"error",
				});

				return false;
			}

			var formData = new FormData();
			formData.append('form_id', form_id);
			formData.append('status', status);
			formData.append('reason', reason);
			formData.append('sup_id', sup_id);

			$.ajax({
				type: "post",
				processData: false,
				contentType: false,
				cache: false,
				data: formData,
				url: '/suppliers/save-criteria',
				headers: {
					'X-CSRF-TOKEN': $('meta[name="csrf-token"]').attr('content')
				},
				success: function(data) {    
					if(data.status){
						swal.fire({
							title: "Success",
							text: data.message,
							type:"success",
							icon:"success",
						}).then((result)=>{
							location.reload();
						});
					}else{
						swal.fire({
							title: "Error",
							text: 'Error',
							type:"error",
							icon:"error",
						});
					}             
				},
				error: function(data) {
					$('body').html(data.responseText);
				}
			});

		});			
			
			$(document).on('click', '#criteria', function(){

			$('#largemodal').modal('show');
			var form_id = $(this).attr('data-form-id');
			var sup_id = $(this).attr('data-sup-id');

			var formData = new FormData();
			formData.append('form_id', form_id);
			formData.append('sup_id', sup_id);

			$.ajax({
				type: "post",
				processData: false,
				contentType: false,
				cache: false,
				data: formData,
				url: '/suppliers/load-criteria',
				headers: {
					'X-CSRF-TOKEN': $('meta[name="csrf-token"]').attr('content')
				},
				success: function(data) {    
					$('.loadcontent').html(data);                
				},
				error: function(data) {
					$('body').html(data.responseText);
				}
			});

			});			

			$(document).on('click', '#subcategories', function(){

			$('#largemodal2').modal('show');
			var form_id = $(this).attr('data-form-id');

			var formData = new FormData();
			formData.append('form_id', form_id);

			$.ajax({
				type: "post",
				processData: false,
				contentType: false,
				cache: false,
				data: formData,
				url: '/suppliers/load-subcategories',
				headers: {
					'X-CSRF-TOKEN': $('meta[name="csrf-token"]').attr('content')
				},
				success: function(data) {    
					$('.loadcontent2').html(data);                
				},
				error: function(data) {
					$('body').html(data.responseText);
				}
			});

			});			

				$(document).off('keyup change','#MasterApprovalComment').on('keyup change', '#MasterApprovalComment', function(e){
					var maxlength = 200;
					$('#MasterApprovalCharacterCount').html('Remaining Characters: '+(maxlength - $(this).val().length));
					$('#MasterApprovalComment').attr('maxlength', maxlength);
				});

				$(document).off('change','#MasterApprovalChoice').on('change', '#MasterApprovalChoice', function(e){
					if($(this).val() == 'OnHold'){
						$('#MasterApprovalReminderSection').css('display','block');
					}else{
						$('#MasterApprovalReminderSection').css('display','none');
					}
				});

				$(document).off('click','#MasterApprovalPost').on('click', '#MasterApprovalPost', function(e){
					
					var MasterApprovalReminderDate = $('#MasterApprovalReminderDate').val();
					var MasterApprovalComment = $('#MasterApprovalComment').val();
					var MasterApprovalChoice = $('#MasterApprovalChoice').val();
					var MasterApprovalType = $('#MasterApprovalType').val();
					var MasterApprovalCount = $('#MasterApprovalCount').val();
					var MasterApprovalID = $('#MasterApprovalID').val();
					var MasterTable = $('#MasterTable').val();
					var MasterStatus = $('#MasterStatus').val();
					var MasterIDField = $('#MasterIDField').val();
					var MasterApprovalRequestor = $('#MasterApprovalRequestor').val();
					var MasterApprovalReference = $('#MasterApprovalReference').val();
					var MasterCounterField = $('#MasterCounterField').val();
					var MasterApprovalChoice2 = $('#MasterApprovalChoice option:selected').text();
					var procurementOfficer = $('#procurementOfficer').val();
					
					var isSignature = $('#isSignature').val();

					if(isSignature==1){
						const canvas = document.getElementById('signaturePad');

						const ctx = canvas.getContext('2d');
						const imageData = ctx.getImageData(0, 0, canvas.width, canvas.height);
						const hasContent = imageData.data.some(value => value !== 0); 

						var MasterSignRealTime = dataURL = canvas.toDataURL();
						var MasterSignUpload = $('#preview').attr('src');

						
						if (!hasContent && !MasterSignUpload) {
							alert('Please Draw or Upload a signature before saving.');
							return false;
						}

						var signature = (!hasContent) ? MasterSignUpload : MasterSignRealTime;
					}else{
						var signature = "";
					}


					var errors = [];
					if(!MasterApprovalChoice){
						alert("Select Action");
						return false;
					}

					if(MasterApprovalCount == 1 && !procurementOfficer && MasterApprovalChoice == "Approve"){
						// errors.push("Select Procurement Officer");
					}

					 if((MasterApprovalChoice == "RequestMoreInformation" || MasterApprovalChoice == "Reject")&& !MasterApprovalComment){
					//if(!MasterApprovalComment){
						errors.push("Comment/Reason is Required");
					}

					if(MasterApprovalChoice == "OnHold" && !MasterApprovalReminderDate ){
						errors.push("Date is Required");
					}

					if(confirm("Do you really want to post with '"+MasterApprovalChoice2+"'?")){

						if(errors.length){
							
							swal.fire({
								title: "Error",
								text:"Please Review the following:\n-"+errors.join('\n-'),
								type:"error",
								icon:"error",
							});
						}else{
							$('#MasterApprovalPost').attr('disabled', true);
							$('#MasterApprovalPostStatus').html('Posting. Please Wait <img src="/assets/images/loading.gif" alt="loading" />');

							var form_data = new FormData();

							form_data.append('choice', MasterApprovalChoice);
							form_data.append('comment', MasterApprovalComment);
							form_data.append('reminder', MasterApprovalReminderDate);
							form_data.append('formType', MasterApprovalType);
							form_data.append('count', MasterApprovalCount);
							form_data.append('id', MasterApprovalID);
							form_data.append('requestor', MasterApprovalRequestor);
							form_data.append('reference', MasterApprovalReference);
							form_data.append('table', MasterTable);
							form_data.append('status', MasterStatus);
							form_data.append('id_field', MasterIDField);
							form_data.append('counter_field', MasterCounterField);
							form_data.append('procurementOfficer', procurementOfficer);
							form_data.append('signature', signature);
                            
                            $.ajax({
                                type: "post",
                                processData: false,
                                contentType: false,
                                cache: false,
                                data: form_data,
                                url: '<?php echo return_url().'ajax/submit-approvals.php'; ?>',
                                headers: {
                                    'X-CSRF-TOKEN': $('meta[name="csrf-token"]').attr('content')
                                },
                                success: function(data) {
									// alert(data);
                                    location.reload();
                                },
                                error: function(data) {
									
                                    $('body').html(data.responseText);
                                }
                            });

						}
					}

				});
			});
		});
		</script>
		<?php

		
	}

	static function supplierSubCategories($id){

		echo '<h5  class="text-primary" style="margin-top:15px;">Supplier Evaluation Criteria</h5>';

			$criteria = DB::table('master_datas')->where('md_code', 'PREQUALIFICATION_CRITERIA')->get();

			echo '<table border="1" class="eagle-table">';
			echo '<tr>';
			echo '<th style="width:30px;">No.</th>';
			echo '<th>Creteria</th>';
			echo '<th>Pass / Fail</th>';
			echo '<th>Comment</th>';
			echo '</tr>';

			$no = 1;
			foreach($criteria as $c){

				@$criteria_saved = DB::table('supplier_prequalification_evaluations')
				->where('spe_sup_ref', $id)
				->where('spe_evaluation_id', $c->md_id)
				->first();

				echo '<tr id="criteria" data-sup-id="'.$id.'" data-form-id="'.$c->md_id.'" data-name="'.$c->md_name.'">';
				echo '<td>'.($no++).'.</td>';
				echo '<td>'.$c->md_name.'</td>';
				
				if((int)@$criteria_saved->spe_passed == 0){
					echo '<td><center>-</center>';
					echo '</td>';
					echo '<td><center>-</center></td>';
				}else{
					echo '<td>'.(((int)@$criteria_saved->spe_passed== 1) ? "Passed" : 'Failed').'</td>';
					echo '<td>'.@$criteria_saved->spe_comment.'</td>';
				}
				echo '</tr>';

			}

			echo '</table>';



			echo '<br/>';
			echo '<h5  class="text-primary" style="margin-top:15px;">Category and Sub categories</h5>';

			$supplier_reference = self::rgf('supplier_registration_details_models', $id, 'id', 'Reference');

			$selected = DB::table('supplier_catagories')
            ->where('sup_ref_id', $supplier_reference)
            ->get();

			$subcategories_selected = array();

			foreach($selected as $item){
				$subcategories_selected[$item->sup_category][$item->sup_sub_category] = $item->id;
			}   
			
			foreach($subcategories_selected as $category => $sub_category){
				echo '<p class="fs-16" style="color:bold; margin-bottom:0; margin-top:10px;padding-bottom:0;">'.@Controller::rgf("master_datas",$category, "md_id", "md_name").'</p>';

				$no = 1;
				echo '<table class="eagle-table" border="1">';
				echo '<tr>';
				echo '<th>No.</th>';
				echo '<th style="width:30%;">Subcategory</th>';
				echo '<th style="width:150px;">Pass / Fail</th>';
				echo '<th>Comment </th>';
				echo '</tr>';
				foreach($sub_category as $subcategory => $id){
			
					echo '<tr valign="top">';
					echo '<td style="width:30px;">'.($no++).'.</td>';
					echo '<td>'.@Controller::rgf("master_datas",$subcategory, "md_id", "md_name").'</td>';					
				
					
					$status = @Controller::rgf("supplier_catagories",$id, "id", "sup_evaluation_passed");
					if((int)$status == 0){
						echo '<td><center>-</center>';
						echo '</td>';
						echo '<td><center>-</center></td>';
					}else{
						$string = ($status == 1) ? "Passed" : 'Failed <br><small class="text-danger">Sub category will be dropped</small>';
						echo '<td>'.$string.'</td>';
						echo '<td>'.@Controller::rgf("supplier_catagories",$id, "id", "sup_evaluation_comment").'</td>';
					}
					echo '</tr>';
				}
				
				echo '</table>';

			}

			echo '<br/>';


	}

	public static function approvers($count, $type, $show='emails'){

        $sql = "SELECT app_role_id FROM master_approval_orders WHERE app_type = '$type' AND app_count = '$count' ORDER BY app_count DESC";
        $select = DB::select($sql);

		$info = array();
		if($show == "ROLE_ID"){
			foreach($select as $row){
				$info[] = $row->app_role_id;
			}

			return $info;
		}else if($show == "ROLE_NAME"){			
			foreach($select as $row){
				$info[] = Controller::rgf('user_roles', $row->app_role_id, 'id', 'user_name');
			}

			return $info;
		}else if($show == "ID"){			
			foreach($select as $row){
				$role = $row->app_role_id;
				
				$user_ids = DB::table('users_and_roles')->where('uar_role_id','=', $role)->get();

				if(count($user_ids)){
					foreach($user_ids as $user_id){
						$info[] = $user_id->uar_user_id;
					}
				}
			}

			return $info;
		}else if($show == "NAME"){			
			foreach($select as $row){
				$role = $row->app_role_id;

				$user_ids = DB::table('users_and_roles')->where('uar_role_id','=', $role)->get();

				foreach($user_ids as $user_id){
					if(count($user_ids)){
						$info[] = Controller::rgf('admins', $user_id->uar_user_id, 'id', 'firstname').' '.Controller::rgf('admins', $user_id->uar_user_id, 'id', 'lastname');
					}
				}
			}

			return $info;
		}else if($show == "EMAIL"){			
			foreach($select as $row){
				$role = $row->app_role_id;

				$user_ids = DB::table('users_and_roles')->where('uar_role_id','=', $role)->get();
				if(count($user_ids)){
					foreach($user_ids as $user_id){
						$info[] = Controller::rgf('admins', $user_id->uar_user_id, 'id', 'email');
					}
				}
			}

			return $info;
		}
	}

	public function submitApprovals(Request $request){

		$type = $request->formType;
		$choice = $request->choice;
		$comment = $request->comment;
		$reminder = $request->reminder;
		$count = $request->count;
		$id = $request->id;
		$requestor = $request->requestor;
		$reference = $request->reference;
		$counter_field = $request->counter_field;
        $table = $request->table;
        $status = $request->status;
        $id_field = $request->id_field;
		$procurementOfficer = $request->procurementOfficer;
		$signature = $request->signature;

		$this->submitApprovalsProceed($type, $choice, $comment, $reminder, $count, $id, $requestor, $reference, $counter_field, $table, $status, $id_field, 1, $procurementOfficer, $signature);

	}

	public function submitApprovalDirect($type, $choice, $comment, $reminder, $count, $id, $requestor, $reference, $counter_field, $table, $status, $id_field, $isSendEmail){

		$this->submitApprovalsProceed($type, $choice, $comment, $reminder, $count, $id, $requestor, $reference, $counter_field, $table, $status, $id_field, $isSendEmail, 0, 0, "");

	}

    public function submitApprovalsProceed($type, $choice, $comment, $reminder, $count, $id, $requestor, $reference, $counter_field, $table, $status, $id_field, $isSendEmail=1, $procurementOfficer, $signature
	){

        $user = $user_id = $this->user_id();

		$signature = ($signature);

        $attachment = DB::table('attachments')->whereRaw("(at_ref IS NULL AND (at_added_by = '$user_id' OR at_type = '$type'))")->update(array('at_ref'=>$id, 'at_counter'=>$count));
        
        $approved = $forwarded = "";
        $sql = "SELECT app_role_id as approved FROM master_approval_orders WHERE app_type = '$type' AND app_count = '$count' ORDER BY app_count DESC";
        $select = DB::select($sql);

        $app_role_name = Controller::rgf('user_roles', $select[0]->approved, 'id', 'user_name');

        // dd($app_role_name);

        if($choice == "ToApprove1"){
			dd('toapprove');
		}else if($choice == "ToCancel1"){
			dd('to cancel');
		}else if($choice == "Approve" || $choice == "ToApprove" || $choice == "ToCancel"){
        
            $a = $b = array();
        
            $next_count = $count+1;
            $sql = "SELECT app_role_id as forwarded FROM master_approval_orders WHERE app_type = '$type' AND app_count = '$next_count' ORDER BY app_count DESC";
            $select = DB::select($sql);
                
            $a['ma_app_role_'.$count] = $approved;

            $b['ma_match_id'] = $id;
            $b['ma_type'] = $type;
			
            $a['ma_app_'.$count] = 1;
            $a['ma_user_id_'.$count] = $user;
            $a['ma_date_added_'.$count] = time();
            $a['ma_comment_'.$count] = $comment;
			$a['ma_signature_'.$count] = $signature;


            $sql = "SELECT TOP 1 app_count FROM master_approval_orders WHERE app_type = '$type'  AND  app_role_id IS NOT NULL AND app_role_id != 0 ORDER BY app_count DESC";
            $select = DB::select($sql);
                
            if($select[0]->app_count == $count){                
                $update = array();
                $update[$counter_field] = $next_count;
                $update[$status] = 10;
                $update_counter = DB::table($table)->where($id_field,$id)->update($update);

					
				$emails = Controller::rgf("supplier_registration_details_models", $id, "id", "company_email"); 
				$supplier_name = Controller::rgf("supplier_registration_details_models", $id, "id", "BusinessName"); 

				$loggedin_name = Controller::rgf('admins', $this->user_id(), 'id', 'firstname').' '.Controller::rgf('admins', $this->user_id(), 'id', 'lastname');
				$loggedin_email[] = Controller::rgf('admins', $this->user_id(), 'id', 'email');
				
				$data = [
					'to' => $supplier_name,
					'table' => self::supplierSubCategoriesApproved($id, 1, 0),
					'supplier' => $supplier_name,
					'reference' => $reference,
					'link' => url('supplier/supplier-profile/'.$id),//PROTAL
				];

				$email1 = self::approvers(1, $type, 'EMAIL');
				$email2 = self::approvers(2, $type, 'EMAIL');
				$email3 = self::approvers(3, $type, 'EMAIL');

				$ccEmails = array_merge($email1, $email2, $email3, $loggedin_email);

				if(($emails) && $isSendEmail){
					$subject = "CONFIRMATION OF SUCCESSFUL PREQUALIFICATION";
					try{
						Mail::send('emails.suppliers.supplier-prequalification-completed', $data, function ($message) use ($emails, $subject, $ccEmails) {
							$message->to($emails)
							->cc($ccEmails)
							->subject($subject);
						});
					} catch (\Exception $e) {
						// Log the exception or handle it accordingly
						Log::error('Failed to send email: ' . $e->getMessage());
					}

					//======== build email to SUPPLIER

				}            
				
            }else if($count == 1 && $isSendEmail == 1){ //assgin procurement officer

				$procurement_officer = $procurementOfficer;

				$suppliers = array($id);
		
				$no = 1;
				$string = '<table style="width:100%" border="1" cellspacing="0" cellpadding="2">';
				$string .= '<tr><th style="width:10px">No.</th><th>Supplier Name</th><th>Access Link</th></tr>';
		
				foreach ($suppliers as $supplier) {
		
					$form_details = DB::table('supplier_registration_details_models')->select('supplier_reference_form_no', 'BusinessName')->where('id', $supplier)->first();
		
					$update = DB::table('supplier_registration_details_models')->where('id', $supplier)->update([
						"s_assigned_officer" => $procurement_officer,
						"s_assigned_date" => time(),
						"s_assigned_by" => $this->user_id(),
						"s_type" => 'Supplier Prequalification',
						"s_counter" => 2,
					]);
				   		
					$string .= '<tr><td>'.($no++).'.</td><td>'.$form_details->supplier_reference_form_no.'<br/><b>'.$form_details->BusinessName.'</b></td><td>'.url('suppliers/supplier-profile/'.$supplier).'</td></tr>';
		
				}
		
				$string .= '</table>';
		
				$userData = DB::table('admins')->where('id', $procurement_officer)->first();
		
				$data = [
					'email' => $userData->email,
					'title' => 'NEW PREQUALIFICATION ASSIGNMENT'.((count($suppliers)>1) ? "S":"").' AWAITING YOUR REVIEW',
					'procurementOfficerName' => $userData->firstname.' '.$userData->lastname,
					'table' => $string,
					'total' => count($suppliers),
				];
		
				try {
					Mail::send('emails.suppliers.assigned-procurement', $data, function ($message) use ($data) {
						$message->to($data['email'])
							->subject($data['title']);
					});
				} catch (\Exception $e) {
					Log::error('Failed to send email: ' . $e->getMessage());
				}
			}else{
				
                $update = array();
                $update[$counter_field] = $next_count;
                $update[$status] = 1;

				if($count == 2){
                	$update['s_assigned_officer_recommendation'] = ($choice == "ToApprove") ? "1" : "-1";
                	$update['s_assigned_officer_recommendation_date'] = time();
				}

                $update[$status] = 1;
                $update_counter = DB::table($table)->where($id_field,$id)->update($update);
				
				$emails = self::approvers($next_count, $type, 'EMAIL');
				
				$email = "";//implode(',',$emails);
        		$names = self::approvers($next_count, $type, 'NAME');
        		$role = self::approvers($next_count, $type, 'ROLE_NAME')[0];

				$recommended = (($choice == 'ToApprove') ? '<span style="color: white; background-color: green; border-radius: 5px; padding: 3px 8px; font-weight: bold;">Approve</span>' : '<span style="color: white; background-color: red; border-radius: 5px; padding: 3px 8px; font-weight: bold;">Cancel</span>');

				if($count == 2){	
					$data = [
						'to' => $role.'('.implode(',',$names).')',
						'body' => 'You have a pending '.$type.' with reference <b>'.$reference.'</b> <br/><br/><b>Recommendation: </b>'.$recommended.'<br/><b>Recommended by: </b>'.$bf->full_name($this->user_id()).'<br/><br/><b>Reason:</b><br/>'.$comment.'<br/>',
						'link' => url('suppliers/supplier-profile/'.$id),
					];	
				}else{
                	$data = [
						'to' => $role.'('.implode(',',$names).')',
						'body' => 'You have a pending '.$type.' with reference <b>'.$reference.'</b>',
						'link' => url('suppliers/supplier-profile/'.$id),
					];
				}
                        
				
				if(($emails) && $isSendEmail){
					try{
						$subject = "PENDING ".strtoupper($type)." APPROVAL";
						Mail::send('emails.master-approval.approve-or-rejection-email', $data, function ($message) use ($emails, $subject) {
							$message->to($emails)
							->subject($subject);
						});
					} catch (\Exception $e) {
						// Log the exception or handle it accordingly
						Log::error('Failed to send email: ' . $e->getMessage());
					}
				}
            }   


            $done = master_approval::updateOrCreate($b,$a);
            
            AuditTrailController::register($type, 'Approved reference no. <b>'.$reference.'</b> at Level <b>'.$count.'</b> as '.$app_role_name);
        
        }else if($choice == "Reject"){
            $update = array();

            //$update[$counter_field] = $next_count;
            $update[$status] = -1;
            $update_counter = DB::table($table)->where($id_field,$id)->update($update);
            
            $rejected = new MasterRejection();
            $rejected->mr_comment = $comment;
            $rejected->mr_added_by = $user_id;
            $rejected->mr_date_added = time();
            $rejected->mr_type = $type;
            $rejected->mr_type_id = $id;
            $rejected->save();
			
			$emails = Controller::rgf("supplier_registration_details_models", $id, "id", "company_email"); 
			$supplier_name = Controller::rgf("supplier_registration_details_models", $id, "id", "BusinessName"); 

			$loggedin_name = Controller::rgf('admins', $this->user_id(), 'id', 'firstname').' '.Controller::rgf('admins', $this->user_id(), 'id', 'lastname');
			$loggedin_email[] = Controller::rgf('admins', $this->user_id(), 'id', 'email');

			$data = [
				'to' => Controller::rgf('admins', $requestor, 'id', 'firstname'),
				'body' => 'Your '.$type.' with reference <b>'.$reference.'</b> has been rejected',
				'link' => url('suppliers/supplier-profile/'.$id),
			];

			$data = [
				'to' => $supplier_name,
				'body' => 'Your Prequalication application with reference <b>'.$reference.'</b> has been rejected <br/><p><b>Reason:</b><br/>'.$comment.'</p><br/><span style="text-decoration:underline;"><b>Contact Information:</b></span> <br/><b>Name: </b>'.$loggedin_name.'<br/><b>Title:</b> '.$app_role_name.'<br/><b>Contact email:</b> <a href="mailto:'.$loggedin_email[0].'">'.$loggedin_email[0].'</a><BR/>You can use the link below to provide more information:',
				'link' => url('supplier/supplier-profile/'.$id),//PROTAL
			];


			$email1 = self::approvers(1, $type, 'EMAIL');
			$email2 = self::approvers(2, $type, 'EMAIL');
			$email3 = self::approvers(3, $type, 'EMAIL');

			$ccEmails = array_merge($email1, $email2, $email3, $loggedin_email);

			if(($emails) && $isSendEmail){
				$subject = "".strtoupper($type)." REJECTION";
				try{
					Mail::send('emails.master-approval.approve-or-rejection-email', $data, function ($message) use ($emails, $subject) {
						$message->to($emails)
						->subject($subject);
					});
				} catch (\Exception $e) {
					// Log the exception or handle it accordingly
					Log::error('Failed to send email: ' . $e->getMessage());
				}
			}

            AuditTrailController::register($type, 'Rejected reference no. <b>'.$reference.'</b> at Level <b>'.$count.'</b> as '.$app_role_name);

            
        }else if($choice == "RequestMoreInformation"){
            $update = array();
            //$update[$counter_field] = $next_count;
            $update[$status] = -3;
            $update_counter = DB::table($table)->where($id_field,$id)->update($update);
            
            $rejected = new MasterRejection();
            $rejected->mr_comment = $comment;
            $rejected->mr_added_by = $user_id;
            $rejected->mr_date_added = time();
            $rejected->mr_type = $type;
            $rejected->mr_type_id = $id;
            $rejected->mr_decision = 'MORE INFORMATION';
            $rejected->mr_to = $requestor;
            $rejected->mr_count = $count;
            $rejected->save();  			
			
			$emails = Controller::rgf("supplier_registration_details_models", $id, "id", "company_email"); 
			$supplier_name = Controller::rgf("supplier_registration_details_models", $id, "id", "BusinessName"); 

			$loggedin_name = Controller::rgf('admins', $this->user_id(), 'id', 'firstname').' '.Controller::rgf('admins', $this->user_id(), 'id', 'lastname');
			$loggedin_email[] = Controller::rgf('admins', $this->user_id(), 'id', 'email');
			
			$data = [
				'to' => $supplier_name,
				'body' => 'More information for '.$type.' with reference <b>'.$reference.'</b> is needed<br/><p><b>Comment:</b><br/>'.$comment.'</p><br/><span style="text-decoration:underline;"><b>Contact Information:</b></span> <br/><b>Name: </b>'.$loggedin_name.'<br/><b>Title:</b> '.$app_role_name.'<br/><b>Contact email:</b> <a href="mailto:'.$loggedin_email[0].'">'.$loggedin_email[0].'</a><BR/>You can use the link below to provide more information:',
				'link' => url('supplier/supplier-profile/'.$id),//PROTAL
			];

			$email1 = self::approvers(1, $type, 'EMAIL');
			$email2 = self::approvers(2, $type, 'EMAIL');
			$email3 = self::approvers(3, $type, 'EMAIL');

			$ccEmails = array_merge($email1, $email2, $email3, $loggedin_email);

			if(($emails) && $isSendEmail){
				$subject = "".strtoupper($type)." REQUEST FOR MORE INFORMATION";
				try{
					Mail::send('emails.master-approval.approve-or-rejection-email', $data, function ($message) use ($emails, $subject, $ccEmails) {
						$message->to($emails)
						->cc($ccEmails)
						->subject($subject);
					});
				} catch (\Exception $e) {
					dd($e->getMessage());
					// Log the exception or handle it accordingly
					Log::error('Failed to send email: ' . $e->getMessage());
				}
			}

            
            AuditTrailController::register($type, 'REQUESTED INFORMATION for reference no. <b>'.$reference.'</b> at Level <b>'.$count.'</b> as '.$app_role_name);

        }else if($choice == "MoreInformation"){
            $update = array();
            $update[$counter_field] = $count;
            $update[$status] = 1;
            $update_counter = DB::table($table)->where($id_field,$id)->update($update);
            
            $rejected = new MasterRejection();
            $rejected->mr_comment = $comment;
            $rejected->mr_added_by = $user_id;
            $rejected->mr_date_added = time();
            $rejected->mr_type = $type;
            $rejected->mr_type_id = $id;
            $rejected->mr_decision = 'MORE INFORMATION';
            $rejected->mr_to = $requestor;
            $rejected->mr_count = $count;
            $rejected->save();   

            AuditTrailController::register($type, 'PROVIDED MORE INFORMATION for reference no. <b>'.$reference.'</b> at Level <b>'.$count.'</b> as '.$app_role_name);
        }
        //@@@@@@@@@@@@@@@@@@@@@@@@

    }

    public static function display($vars = array()){


		echo '<div style="font-family:arial; font-size:10px;padding:0;margin:0;">';
		foreach($vars as $v=>$var){
			$$v=$var;
		}
		
		$id_form = $id;

		$sql = "SELECT $status AS sentReq FROM $table WHERE $status = '1' AND $id_field = '$id'";

		$db = new Db();
		$select = $db->select($sql);

        $sql = "SELECT * FROM master_approval_orders WHERE app_type = '$where' AND ( app_role_id IS NULL OR app_role_id != 0 ) ORDER BY app_count ASC";
		$select = $db->select($sql);
       

		$save_table = "master_approvals";

		// $user_id = user_id();
		// $user_id = user_id();
		// $del = $this->isDelegate($user_id, time());
		// if(count($del)){
		// 	$user_id = $del['by'];
		// }else{
		// 	$user_id = user_id();
		// }
			

		$count = 1; $xx=1;


        //////////////////////////////////
		$sql = "SELECT mr_added_by, mr_date_added, mr_comment FROM master_rejections WHERE mr_type_id = '$id' AND mr_decision = 'REJECTION' AND mr_type = '$where' ORDER BY mr_date_added ASC";
        $xx = $db->select($sql);

		$i=1;
		if(count($xx)){
        echo '<div style="margin-top:10px;"></div>';
		echo '<label style="font-weight:bold;font-size:12px;">REJECTION: </label>';
		echo '<div>';
			$i=1;
			echo '<div style="margin:0;padding-bottom:15px;" class="panel panel-success">';
			echo '<ol style="list-style:roman;color:#000;list-style:none;padding:0; margin:0;font-size:12px;">';
			foreach($xx[0] as $row){		
				extract($row);
					if(1){
						echo '<li style="border-bottom:1px solid #CCC;color:#000;padding:0 10px;margin:0;">';
						echo '<div style="padding:0 10px;margin:0;">';
						echo '<b>Rejected by: </b>'.$bf->full_name($mr_added_by).'';
						echo '&nbsp; &nbsp; &nbsp; &nbsp; &nbsp; ';
						echo '<b>Date: </b>'.$this->date_fm($mr_date_added).'';
						echo "<br/><b>Comment/Reason:</b> ";
						echo nl2br($mr_comment);
						echo '<br/>';
						echo '</div>';
						echo '</li>';
					}
			}
			echo '</ol>';
			echo '</div>';
		}
		//echo '<br/>';
		echo '</div>';

		$count = 1;
		echo '<label style="font-size:12px;padding:0;margin:0;margin-top:20px;font-family:arial;font-weight:bold;">APPROVALS ('.$where.'): </label>';

		echo '<ol style="padding:0 10px;padding-left:30px; margin:0;font-family:arial;font-size:14px;">';
		foreach($select[0] as $level2){
			extract($level2);
			$level = $app_role_id;
			$action = $app_action;

			$a = 'ma_app_'.$count;
			$aa = 'ma_app_role_'.$count;
			$c = 'ma_hold';

			//$b = 'ma_app_user_id_'.($count-1);
			$l = '';

			$label = ($action) ? $action : "Approved by";

			$label2 = "";

			$sql1 = "SELECT * FROM $save_table WHERE ma_match_id = '$id_form' AND ma_type = '$where' AND $a = 1";
			$select1 = $db->select($sql1);
			

            if($count > 1){
                $b = 'ma_app_'.($count-1);
			    $sql2 = "SELECT * FROM $save_table WHERE ma_match_id = '$id_form' AND ma_type = '$where' AND $b = 1";
			    $select2 = $db->select($sql2);
            }

			//====================================================
			{
				// $location = $bf->rgf('payment',$id,'pay_id','pay_location');
				// $location2 = $bf->rgf("master_data", $location, "md_code", "md_id");
				// $payment_type = $bf->rgf("payment", $id, 'pay_id', 'pay_type');
				// $payment_type2 = $bf->rgf("payment", $id, 'pay_id', 'pay_type2');
				// $payment_status = $bf->rgf("payment", $id, 'pay_id', 'pay_status');
				// $pay_type_price_range = $bf->rgf("payment", $id, 'pay_id', 'pay_type_price_range');

				ini_set('display_errors', 1);

				$bf = new BeforeAndAfter();
				

                $approval_status = $bf->rgf($table, $id_form, $id_field, $status);

				$type = $bf->rgf("user_role", $level, "ur_id", "ur_name");

				$level2 = $bf->rgf("user_role", $level, "ur_id", "ur_name");
				

				$time = time();
				
				$sql = "SELECT user_id FROM sysuser WHERE user_role = '$level'";

                $approvers = $db->select($sql);
                               
				
			}
			//=======================================================
            //echo $level2;
           
            $role = $bf->rgf("user_role", $level, "ur_id", "ur_name");

            // dd($role);


            $names_of_approvers = array();
            $users_to_approver = array();
            if(count($approvers)){
                foreach($approvers[0] as $a){
					extract($a);
                    $users_to_approver[] = $user_id;
					
					if($bf->full_name($user_id))
                    $names_of_approvers[$bf->full_name($user_id)] = '<b>'.$bf->full_name($user_id).'</b>';
                }
            }

            $xx = $db->select("SELECT * FROM master_rejections WHERE mr_type_id = '$id_form' AND mr_decision = 'MORE INFORMATION' AND mr_to = '".user_id()."' AND mr_count = '$count' AND mr_type = '$where' ORDER BY mr_date_added ASC");                        

			if(count($select1)){ 
				extract($select1[0][0]);
				echo '<li style="padding:0;margin:0;border-bottom:2px solid #ccc;margin-top:10px;"><b>'.$label2.' '.$role.'</b></li>';
                
                // self::moreInformation($id, $where, $count);
				if($count == 2){ 
					// self::supplierSubCategories($id);
				}

                echo '<b>'.$label.': </b>'.$bf->full_name(${"ma_user_id_".$count})." &nbsp; &nbsp; ";
				echo '<b>Date:</b> '.Feedback::date_fm(${"ma_date_added_".$count})."";
				
				
				if(${"ma_comment_".$count})
					echo '<br/><b>Comment:</b> '.nl2br(${"ma_comment_".$count})." ";
				
				$signature = ${"ma_signature_".$count};
				if($signature)
					echo '<br/><img style="height:100px;" src="'.$signature.'" alt="image">';


			}else if((in_array(user_id(), $users_to_approver) && $count == $counter)||count($xx)){
                //&& ((count($select1) && $count==1)|| (empty(count($select1)) && $count >= 2 && count($select2)))
				echo '<li style="border-bottom:2px solid #ccc;margin-top:10px;"><b>'.$label2.' '.$role.'</b></li>';
                               
				if($approval_status == -1 || $approval_status == -2)
					echo 'Rejected can\'t continue';
                else if($approval_status == 1 || $approval_status == -3)
                    self::form($id_form, $where, $count, $notify_user, $reference, $label, $table, $status,$id_field, $counter_field, $approval_status);
                else{
					echo 'Pending Approval from: <br/>'.implode(' <br/>', $names_of_approvers);
                    if(empty($names_of_approvers))
                        echo '<span title="'.$level2.'" class="label label-warning">'.$role.' Approval not found</span>';
                }
			}else {
				echo '<li style="border-bottom:2px solid #ccc;margin-top:10px;"><b>'.$label2.' '.$role.'</b></li>';		
				if($approval_status == -1 || $approval_status == -2)
					echo 'Rejected can\'t continue';
				else{
					
					echo 'Pending Approval from: <br/>'.implode(' <br/>', $names_of_approvers);
					if(empty($names_of_approvers)){
						echo '<span title="'.$level2.'" class="label label-warning">'.$role.' Approval not found</span>';
					}
                }
			}
			$count++;
			//$xx++;
		}
		echo '</ol>';
		echo '</div>';
	}

    public static function moreInformation($id, $where, $count){
        // =================== MORE INFO ==========================
        $xx = DB::select("SELECT * FROM master_rejections WHERE mr_type_id = '$id' AND mr_decision = 'MORE INFORMATION' AND mr_count = '$count' AND mr_type = '$where' ORDER BY mr_date_added ASC");

        $i=1;
        if(count($xx)){
        echo '<div style="margin-top:10px;"></div>';
        //echo '<label style="font-weight:bold;font-size:12px;">REJECTION: </label>';
        echo '<div>';
            $i=1;
            echo '<div style="margin:0;padding-bottom:15px;" class="panel panel-success">';
            echo '<ol style="list-style:roman;color:#000;list-style:none;padding:0; margin:0;font-size:12px;">';
            foreach($xx as $row){		

                    if(1){
                        echo '<li style="border-bottom:1px solid #CCC;color:#000;padding:0 10px;margin:0;">';
                        echo '<div style="padding:0 10px;margin:0;">';
                        echo '<b>More Information Requested by: </b>'.$bf->full_name($row->mr_added_by).'';
                        echo '&nbsp; &nbsp; &nbsp; &nbsp; &nbsp; ';
                        echo '<b>Date: </b>'.$this->date_fm($row->mr_date_added).'';
                        echo "<br/><b>Comment/Reason:</b> ";
                        echo nl2br($row->mr_comment);
                        echo '<br/>';
                        echo '</div>';
                        echo '</li>';
                    }
            }
            echo '</ol>';
            echo '</div>';
        }
        // =================== MORE INFO ==========================    
    }

	public function trimBase64Image($base64String) {
		// Decode the Base64 string
		$imageData = explode(',', $base64String);
		$imageType = str_replace('data:image/', '', $imageData[0]);
		$imageType = str_replace(';base64', '', $imageType);
		$decodedImage = base64_decode($imageData[1]);
	
		// Create an image resource
		$image = imagecreatefromstring($decodedImage);
		if (!$image) {
			die("Could not create image from string.");
		}
	
		// Get dimensions
		$width = imagesx($image);
		$height = imagesy($image);
	
		// Initialize bounding box values
		$xMin = $width;
		$yMin = $height;
		$xMax = 0;
		$yMax = 0;
	
		// Find the bounding box of non-white pixels
		for ($y = 0; $y < $height; $y++) {
			for ($x = 0; $x < $width; $x++) {
				$rgb = imagecolorat($image, $x, $y);
				$color = imagecolorsforindex($image, $rgb);
	
				// Check if pixel is not white
				if ($color['red'] < 255 || $color['green'] < 255 || $color['blue'] < 255) {
					if ($x < $xMin) $xMin = $x;
					if ($x > $xMax) $xMax = $x;
					if ($y < $yMin) $yMin = $y;
					if ($y > $yMax) $yMax = $y;
				}
			}
		}
	
		// Check if there are non-white pixels
		if ($xMax < $xMin || $yMax < $yMin) {
			die("No non-white pixels found.");
		}
	
		// Crop the image based on bounding box
		$trimmedWidth = $xMax - $xMin + 1;
		$trimmedHeight = $yMax - $yMin + 1;
		$trimmedImage = imagecreatetruecolor($trimmedWidth, $trimmedHeight);
		imagecopyresampled($trimmedImage, $image, 0, 0, $xMin, $yMin, $trimmedWidth, $trimmedHeight, $trimmedWidth, $trimmedHeight);
	
		// Create a new Base64 string
		ob_start();
		if ($imageType === 'jpeg') {
			imagejpeg($trimmedImage);
		} elseif ($imageType === 'png') {
			imagepng($trimmedImage);
		} else {
			die("Unsupported image type.");
		}
		$trimmedData = ob_get_contents();
		ob_end_clean();
		
		// Encode the trimmed image back to Base64
		$base64TrimmedImage = 'data:image/' . $imageType . ';base64,' . base64_encode($trimmedData);
	
		// Clean up
		imagedestroy($image);
		imagedestroy($trimmedImage);
	
		return $base64TrimmedImage;
	}
}

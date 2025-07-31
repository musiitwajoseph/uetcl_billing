<?php
class MasterApprovalOrder{
    public static $page = "APPROVAL WORKFLOWS";
    public static $actions = array("Approve", "Reject", "On Hold", "Send Back");
    public static $waitingTimeUnit = array("Seconds"=>1, "Minutes"=>60, "Hours" => 3600, "Days" => 86400);
    public static function links()
    {
        return $links = array(
            array(
                "link_name" => "Approvers",
                "link_address" => "master-approval-order/approvers",
                "link_icon" => "fa-user-check",
                "link_page" => self::$page,
                "link_right" => "V",
            ),
            array(
                "link_name" => "Requisition Approvers",
                "link_address"=>"master-approval-order/requisition",
                "link_icon" => "fa-list",
                "link_page" => self::$page,
                "link_right" => "V",
            ),
            array(
                "link_name" => "Procurement Plan Approvers",
                "link_address"=>"master-approval-order/procurement-plan",
                "link_icon" => "fa-list",
                "link_page" => self::$page,
                "link_right" => "V",
            ),
            array(
                "link_name" => "Travel Authorisation Form Approvers",
                "link_address"=>"master-approval-order/travel-authorisation-form",
                "link_icon" => "fa-list",
                "link_page" => self::$page,
                "link_right" => "V",
            ),
            array(
                "link_name" => "Tender Approvers",
                "link_address"=>"master-approval-order/tendering",
                "link_icon" => "fa-list",
                "link_page" => self::$page,
                "link_right" => "V",
            ),
            array(
                "link_name" => "RFQ Evaluation Approvers",
                "link_address"=>"master-approval-order/price-comparison",
                "link_icon" => "fa-list",
                "link_page" => self::$page,
                "link_right" => "V",
            ), array(
                "link_name" => "RFP EValuation Approvers",
                "link_address"=>"master-approval-order/tender-approval",
                "link_icon" => "fa-list",
                "link_page" => self::$page,
                "link_right" => "V",
            ),
            array(
                "link_name" => "Tender Committee Approvers",
                "link_address"=>"master-approval-order/tender-committee",
                "link_icon" => "fa-list",
                "link_page" => self::$page,
                "link_right" => "V",
            ),
            array(
                "link_name" => "Addendum Approvers",
                "link_address"=>"master-approval-order/addendum",
                "link_icon" => "fa-list",
                "link_page" => self::$page,
                "link_right" => "V",
            ),
            array(
                "link_name" => "Purchase Order",
                "link_address"=>"master-approval-order/purchase-order",
                "link_icon" => "fa-list",
                "link_page" => self::$page,
                "link_right" => "V",
            ),
            array(
                "link_name" => "Asset Disposal",
                "link_address"=>"master-approval-order/asset-disposal",
                "link_icon" => "fa-list",
                "link_page" => self::$page,
                "link_right" => "V",
            ),
            array(
                "link_name" => "Suppliers (Prequalification)",
                //"link_address"=>"#",
                "link_address" => "master-approval-order/suppliers",
                "link_icon" => "fa-list",
                "link_page" => self::$page,
                "link_right" => "V",
            ),
        );
    }

    public function suppliers()
    {
        return view('master-approval-order.suppliers');
    }

    public function approvers()
    {

        $data = ['LoggedUserAdmin' => Admin::where('id', '=', session('LoggedAdmin'))->first()];
        $user_data = user_role::where('id', '!=', config('constants.options.HEADS'))->get();
        $array_size = count($user_data);

        $user_info_count = admin::all();

        return view('master-approval-order.approvers', $data, compact(['user_data', 'array_size', 'user_info_count']));
    }

    public function tenderApproval()
    {

        $data = ['LoggedUserAdmin' => Admin::where('id', '=', session('LoggedAdmin'))->first()];

        return view('master-approval-order.tender-approval', $data);
    }

    public function tenderCommittee()
    {

        $data = ['LoggedUserAdmin' => Admin::where('id', '=', session('LoggedAdmin'))->first()];

        return view('master-approval-order.tender-committee', $data);
    }

    public function addendum()
    {

        $data = ['LoggedUserAdmin' => Admin::where('id', '=', session('LoggedAdmin'))->first()];

        return view('master-approval-order.addendum', $data);
    }

    public function purchaseOrder()
    {

        $data = ['LoggedUserAdmin' => Admin::where('id', '=', session('LoggedAdmin'))->first()];

        return view('master-approval-order..purchase-order', $data);
    }

    public function procurementPlan()
    {

        $data = ['LoggedUserAdmin' => Admin::where('id', '=', session('LoggedAdmin'))->first()];

        return view('master-approval-order.procurement-plan', $data);
    }

    public function priceComparison()
    {

        $data = ['LoggedUserAdmin' => Admin::where('id', '=', session('LoggedAdmin'))->first()];

        return view('master-approval-order.price-comparison', $data);
    }

    public function requisition()
    {

        $data = ['LoggedUserAdmin' => Admin::where('id', '=', session('LoggedAdmin'))->first()];

        return view('master-approval-order.approval-order', $data);
    }

    public function tendering()
    {

        $data = ['LoggedUserAdmin' => Admin::where('id', '=', session('LoggedAdmin'))->first()];

        return view('master-approval-order.tendering', $data);
    }

    public function travelAuthorisationForm()
    {

        $data = ['LoggedUserAdmin' => Admin::where('id', '=', session('LoggedAdmin'))->first()];

        return view('master-approval-order.travel-authorisation-form', $data);
    }

    public function assetDisposal()
    {

        $data = ['LoggedUserAdmin' => Admin::where('id', '=', session('LoggedAdmin'))->first()];

        return view('master-approval-order.asset-disposal', $data);
    }

    public function order($where, $label = "", $range = "")
    {
        if (empty($label)) {
            $label = $where;
        }

        $where2 = str_replace(' ', '', $where);

        ?>
<div style="display:none;">
    <style>
        table {
            width: 100%;
            border-collapse: collapse;
            position: relative;
        }
        th, td {
            border: 1px solid #ddd;
            padding: 8px;
            text-align: left;
        }
        th {
            background-color: #f2f2f2;
            position:sticky;
            top:0;
        }

        table {
  /* table-layout: fixed; */
  font-size:20px !important;
}

thead,
tr>th {
  position: sticky;
  background: #fff;
}

thead {
  top: 0;
  z-index: 2;
}
tr>th {
  left: 0;
  z-index: 1;
}
thead tr>th:first-child {
  z-index: 3;
}
    </style>


<h2>Tender Process Overview</h2>
<div style="overflow:scroll; height:500px;">
<table style="font-size:12px;">
    
        <tr>
            <th>Technical Requirements</th>
            <th>Publication of Tender Documents</th>
            <th>Tender Opening</th>
            <th>Tender Evaluation / Shortlisting Report</th>
            <th>Short List Notice Publication Date</th>
            <th>Invitation to Shortlisted Firms to Submit Proposals</th>
            <th>Evaluation of Bids under Shortlist Method</th>
            <th>Purchase / Contracts Committee Approval</th>
            <th>Secretary General (SGs) Approval of PC /CC Reports</th>
            <th>Contract Vetting</th>
            <th>SG/ASG(A&F)/ DHRA Approval</th>
            <th>Contract Signing Date</th>
            <th>Contract Duration</th>
            <th>Contract End Date</th>
            <th>Receipt of Final Technical Requirements Date</th>
            <th>Preparation of Tender Document/RFP/RFQ/BOQs/REOI</th>
            <th>Tender Publication Date</th>
            <th>Tender Closing Date</th>
            <th>Tender Evaluation Start Date</th>
            <th>Tender Evaluation End Date</th>
            <th>Invitation Date</th>
            <th>Closing Date</th>
            <th>Start Date</th>
            <th>End Date</th>
            <th>Submission Date</th>
            <th>Approval Date</th>
            <th>Submission Date</th>
            <th>Approval Date</th>
            <th>Contract Amount</th>
        </tr>
    
    <tbody>
        <!-- Add rows here -->

        <th>2024-01-01</th>
            <th>2024-01-15</th>
            <th>2024-01-20</th>
            <th>2024-01-22</th>
            <th>2024-01-25</th>
            <th>2024-02-01</th>
            <th>2024-02-10</th>
            <th>2024-02-15</th>
            <th>2024-02-20</th>
            <th>2024-03-01</th>
            
            <th>12 months</th>
            <th>2025-03-05</th>
            <th>2024-01-30</th>
            <th>2024-01-01</th>
            <th>2024-01-10</th>
            <th>2024-01-12</th>
            <th>2024-01-20</th>
            <th>2024-01-25</th>
            <th>2024-01-30</th>
            <th>2024-02-05</th>
            <th>2024-02-07</th>
            <th>2024-02-10</th>
            <th>2024-02-15</th>
            <th>2024-02-20</th>
            <th>100,000</th>
            <th>100,000</th>
         <?php for($i=0; $i<130; $i++){ ?>
        <tr>
            <?php echo ($i % 10 == 0) ? '<th style="background-color:#ddd;">'.$i.$i.$i.'</th>': '<td>2024-03-05</td>'; ?>
            <td>Example Tech Req</td>
            <td>2024-01-01</td>
            <td>2024-01-15</td>
            <td>2024-01-20</td>
            <td>2024-01-22</td>
            <td>2024-01-25</td>
            <td>2024-02-01</td>
            <td>2024-02-10</td>
            <td>2024-02-15</td>
            <td>2024-02-20</td>
            <td>2024-03-01</td>
            
            <td>12 months</td>
            <td>2025-03-05</td>
            <td>2024-01-30</td>
            <td>2024-01-01</td>
            <td>2024-01-10</td>
            <td>2024-01-12</td>
            <td>2024-01-20</td>
            <td>2024-01-25</td>
            <td>2024-01-30</td>
            <td>2024-02-05</td>
            <td>2024-02-07</td>
            <td>2024-02-10</td>
            <td>2024-02-15</td>
            <td>2024-02-20</td>
            <td>100,000</td>
            <td>100,000</td>
            <td>100,000</td>
        </tr>
        <?php } ?>
        <!-- Repeat rows as necessary -->
    </tbody>
</table>
    </div>
         </div>



         <style> .master-approval-order-setting-section-d{ background-color:#ffcece;}</style>
         <style> .master-approval-order-setting-section{ background-color:#cee5ff;}</style>
		<style>.order{background-color:#eeeeee; border:1px solid #000; border:1px solid inherit; border-radius:5px; padding:10px; /*#f1ffbd*/;}.order:hover{background-color:#eeeeee; border:1px solid #000; /*#bdf1ff*/;}</style>
		<form action="" method="post">
			<div class="col-lg-12 order" style="margin-bottom:20px">
				<h4><?php echo $label; ?> Levels</h4>
				<?php

        $db = new Db();
        $select = $db->select("SELECT * FROM master_approval_orders WHERE app_role_id != 0 AND app_type = '$where' ORDER BY app_count ASC");

        $app_role_ids = array();
        $app_ids = array();
        $tot = count($select);
        
        $select = $select[0];

        foreach ($select as $sel) {
            extract($sel);
            $app_role_ids[] = @$app_role_id;
            $app_ids[] = @$app_count;
            $app_actions[] = @$app_action;
            $app_attachments[] = @$app_attachment;
            $app_signatures[] = @$app_signature;
            $app_btn_approves[] = @$app_btn_approve;
            $app_btn_sendbacks[] = @$app_btn_amend;
            $app_btn_rejects[] = @$app_btn_reject;
            $app_btn_onholds[] = @$app_btn_onhold;

            $app_btn_approve_labels[] = @$app_btn_approve_label;
            $app_btn_sendback_labels[] = @$app_btn_amend_label;
            $app_btn_reject_labels[] = @$app_btn_reject_label;
            $app_btn_onhold_labels[] = @$app_btn_onhold_label;
            $app_waiting_times[] = @$app_waiting_time;
            $app_waiting_time_units[] = @$app_waiting_time_unit;
        }

        $i = 0;
        //while($i <= $db->num_rows() && $i <= $tot+1){
        while (($i <= count($select)) && ($i < $tot + 1)) {
            ?>
                <div class="row" style="border-bottom:1px solid #000; padding:10px 0; margin:20px 0;">
                <div class="col-md-8">
                    <div class="form-group">
                        <label style="font-weight:bold;font-size:20px;" class="text-uppercase text-primary">Select Person <?php echo $i + 1; ?>:</label>
                        <select id="role<?php echo $where2; ?><?php echo $i; ?>" name="role1<?php echo $where2; ?>[]" class="form-control">
                            <?php

            $db = new Db();
            $use_roles = $db->select("select * from user_role ORDER BY ur_name ASC"); 
            $use_roles = $use_roles[0]; 

            echo '<option value="0"> --- SELECT --- </option>';
            $j = 0;
            foreach ($use_roles as $row) {
                extract($row);
                if ($ur_id == @$app_role_ids[$i]) {
                    echo '<option class="form-control" selected="selected" value="' . $ur_id . '">' . $ur_name . '</option>';
                } else {
                    echo '<option class="form-control" value="' . $ur_id . '">' . $ur_name . '</option>';
                }

            }

            ?>

                        </select>
                    </div>
                </div>
                <div class="col-md-4">
                    <label style="font-weight:bold">Action Label <?php echo $i + 1; ?>:</label>
                    <input type="text" id="action<?php echo $where2; ?><?php echo $i; ?>" value="<?php echo @$app_actions[$i]; ?>" class="form-control"/>
                 </div>
                
                    <div class="col-md-12">
                        <label class="font-weight-bold">Actions allowed: &nbsp;&nbsp;&nbsp;&nbsp; &nbsp;&nbsp;&nbsp;&nbsp;Waiting Time <input type="number" id="waitingTime<?php echo $where2; ?><?php echo $i; ?>" value="<?php echo @$app_waiting_times[$i]; ?>" style="width:100px;" />&nbsp;In 
                        <?php 
                        echo '<select id="waitingTimeUnit'.$where2.$i.'">';
                        foreach(self::$waitingTimeUnit as $label => $value){
                            if(@$app_waiting_time_units[$i] == $value || empty(@$app_waiting_time_units[$i]) && $value == 3600)
                                echo '<option selected value="'.$value.'">'.$label.'</option>';
                            else
                                echo '<option value="'.$value.'">'.$label.'</option>';
                        
                        }
                        echo '</select>';

                        ?>
                        </label>
                    </div>
                    <?php 
                    
                    foreach(self::$actions as $action){
                        $action_cleaned = str_replace(' ','',$action);
                        $app = @${'app_btn_'.strtolower($action_cleaned).'s'}[$i];
                        $app_label = @${'app_btn_'.strtolower($action_cleaned).'_labels'}[$i];
                        $appcheck = ($app == 1) ? "checked" : "";
                        $appclass = ($app == 1) ? "master-approval-order-setting-section" : "master-approval-order-setting-section-d";
                        echo '<div class="col-md-4 col-lg-3 col-xs-6" >';
                        echo '<div id="section'.$action_cleaned.$where2.$i.'" style="position:relative; border:1px solid #ccc;padding:10px;margin:5px 0; border-radius:5px;" class="'.$appclass.'">';
                        echo '<input '.$appcheck.' style="top:-8px;position:absolute;" value="1" type="checkbox" class="action'.$where2.' action'.$where2.$i.'" data-action="'.$action_cleaned.'" data-value="'.$where2.'" data-id="'.$i.'" id="'.$action_cleaned.$where2.$i.'"/>'; 
                        echo '&nbsp;&nbsp;<label for="'.$action_cleaned.$where2.$i.'">'.$action.'</label>';
                        echo '<input id="'.$action_cleaned.$where2.$i.'Label" placeholder="'.$action.' '.($i+1).' Label" class="form-control" type="text" value="'.$app_label.'"/>';
                        echo '</div>';
                        echo '</div> ';
                    }

                    ?>
                    <div class="col-md-4 col-lg-3 col-xs-6">
                        <div style="border:1px solid #ccc;padding:10px;margin:5px 0; border-radius:5px;">
                            <input <?php echo (@$app_attachments[$i]==1) ? "checked":""; ?> value="1" type="checkbox" id="upload<?php echo $where2.$i; ?>"/>&nbsp;&nbsp;<label for="upload<?php echo $where2.$i; ?>">Signature Upload</label>
                        </div>
                    </div>
                    <div class="col-md-4 col-lg-3 col-xs-6">
                        <div style="border:1px solid #ccc;padding:10px;margin:5px 0; border-radius:5px;">
                            <input <?php echo (@$app_signatures[$i]==1) ? "checked" : ""; ?> value="1" type="checkbox" id="realtimeUpload<?php echo $where2.$i; ?>"/>&nbsp;&nbsp;<label for="realtimeUpload<?php echo $where2.$i; ?>">Signature Realtime</label>
                        </div>
                    </div>                    
            

                <div class="clearfix"></div>
                </div>
				<?php
                $i++;
                }
                ?>
				<input type="hidden" id="range<?php echo $where2; ?>" value="<?php echo $range; ?>"/>
				<input type="hidden" id="number<?php echo $where2; ?>" value="<?php echo $i; ?>"/>
				<input type="hidden" id="where<?php echo $where2; ?>" value="<?php echo $where; ?>"/>
				<div class="form-group" style="padding-top:20px;">
					<input class="btn btn-primary btn-block" type="button" data-value="<?php echo $where2; ?>" id="send<?php echo $where2; ?>" name="send<?php echo $where2; ?>" value="Save" class="master-approval-order-btn form-control btn btn-primary"/><span id="sendStatus<?php echo $where2; ?>"></span>
				</div>
                <div id="divdata"></div>
		        <script src="<?php echo return_url().'assets/js/vendors/jquery-3.5.1.min.js'; ?>"></script>
				<script type="text/javascript">
					$(document).ready(function(){ //
                        $('.action<?php echo $where2; ?>').change(function(){
                            var action = $(this).attr('data-action');
                            var id = $(this).attr('data-id');
                            var value = $(this).attr('data-value');
                            if($(this).is(':checked')){
                                $('#section'+action+value+id).addClass('master-approval-order-setting-section');
                                $('#section'+action+value+id).removeClass('master-approval-order-setting-section-d');
                            }else{
                                $('#section'+action+value+id).removeClass('master-approval-order-setting-section');
                                $('#section'+action+value+id).addClass('master-approval-order-setting-section-d');
                            }
                        });
						$('#send<?php echo $where2; ?>').click(function(){
							var button = $(this).attr('data-value');

							var totalNumber = $('#number'+button).val();
							var where = $('#where'+button).val();
							var range = $('#range'+button).val();                          


							for(var i=0; i<totalNumber; i++){
								//alert($('#role'+button+i).val()+" "+where+" "+(i+1));
								if(!$('#role'+button+i).val() && i <= totalNumber-1){
									alert("Select Approver "+(i));
									return false;
								}
							}

							$("#sendStatus"+button).html('<img src="/assets/img/loading.gif" alt="loading" /> Please wait');

							$('#send'+button).attr('disabled', true);
							$('.master-approval-order-btn').attr('disabled', true);

							var form_data = new FormData();

							form_data.append('where', where);
							form_data.append('number', totalNumber);
							form_data.append('range', range);

							for(var i=0; i<totalNumber; i++){
								form_data.append('role'+i, $('#role'+button+i).val());
								form_data.append('action'+i, $('#action'+button+i).val());
                                
                                var upload = ($('#upload'+button+i).is(':checked')) ? 1 : 0;
                                var realtimeUpload = ($('#realtimeUpload'+button+i).is(':checked')) ? 1 : 0;

								form_data.append('upload'+i, upload);
								form_data.append('realtimeUpload'+i, realtimeUpload);
								form_data.append('waitingTime'+i, $('#waitingTime'+button+i).val());
								form_data.append('waitingTimeUnit'+i, $('#waitingTimeUnit'+button+i).val());


                                $('.action'+button+i).each(function(){
                                    var action = ($(this).attr('data-action'));
                                    var actionValue = ($('#'+action+button+i).is(":checked")) ? "1" : "0";
                                    form_data.append(action+'_type'+i, actionValue);
                                    form_data.append(action+'_type_label'+i, $('#'+action+button+i+"Label").val());
                                });
							}
                            
                            $.ajax({
                                type: "post",
                                processData: false,
                                contentType: false,
                                cache: false,
                                data: form_data,
                                url: '<?php echo return_url().'ajax/save-order.php' ?>',
                                headers: {
                                    'X-CSRF-TOKEN': $('meta[name="csrf-token"]').attr('content')
                                },
                                success: function(data) {
                                    location.reload();
                                },
                                error: function(data) {
                                    $('body').html(data.responseText);
                                }
                            });
						});
					});
				</script>
			</div>
			</div>
		</form>
                </div>	
                <div style="clear:both"></div>	
        <?php

    }

    public function saveOrder(Request $request)
    {
        $where = $_POST['where'];
       echo 'sdfsdf'.$number = $_POST['number'];
        $range = $_POST['range'];

        $db = new Db();
        $delete = $db->query("DELETE FROM master_approval_orders WHERE app_type = '$where'");

        $setter = 1;

        $items = '';
        for ($i = 0; $i < $number; $i++) {

            $items .= '> ' . $item = $_POST['role'.$i];
            $items .= '> ' . $action = $_POST['action' . $i];
            $items .= '> ' . $realtimeUpload = $_POST['realtimeUpload' . $i];
            $items .= '> ' . $upload = $_POST['upload' . $i];
            $items .= '> ' . $action = $_POST['action' . $i];

            $items .= '> ' . $Approve_type = $_POST['Approve_type' . $i];
            $items .= '> ' . $Reject_type = $_POST['Reject_type' . $i];
            $items .= '> ' . $SendBack_type = $_POST['SendBack_type' . $i];
            $items .= '> ' . $OnHold_type = $_POST['OnHold_type' . $i];

            $items .= '> ' . $Approve_type_label = $_POST['Approve_type_label' . $i];
            $items .= '> ' . $Reject_type_label = $_POST['Reject_type_label' . $i];
            $items .= '> ' . $SendBack_type_label = $_POST['SendBack_type_label' . $i];
            $items .= '> ' . $OnHold_type_label = $_POST['OnHold_type_label' . $i];

            $items .= '> ' . $waitingTime = $_POST['waitingTime' . $i];
            $items .= '> ' . $waitingTimeUnit = $_POST['waitingTimeUnit'. $i];
            // $items .= '> ' . $action = $_POST['action' . $i];

            if ($item) {

                $db = new Db();

                $db->insert("master_approval_orders", [
                    "app_count" => $setter, "app_type" => $where,
                    "app_role_id" => $item,
                    "app_range" => $range,
                    "app_action" => $action,
                    "app_signature" => $realtimeUpload,
                    "app_attachment" => $upload,
                    
                    "app_btn_reject" => $Reject_type,
                    "app_btn_approve" => $Approve_type,
                    "app_btn_amend" => $SendBack_type,
                    "app_btn_onhold" => $OnHold_type,

                    "app_btn_reject_label" => $Reject_type_label,
                    "app_btn_approve_label" => $Approve_type_label,
                    "app_btn_amend_label" => $SendBack_type_label,
                    "app_btn_onhold_label" => $OnHold_type_label,

                    "app_waiting_time" => $waitingTime,
                    "app_waiting_time_unit" => $waitingTimeUnit,

                    "app_date_added" => time(),
                    "app_added_by" => 11, //Helper::user_id(),
                    ]
                );
            }

            $setter++;
        }

        if ($done) {
            $json['message'] = "Error";
            $json['messageError'] = "Not saved.";
        } else {
            $json['message'] = "Success";
        }

        echo json_encode($json);
    }


    public function view_users_and_roles(Request $request)
    {

        $data = ['LoggedUserAdmin' => Admin::where('id', '=', session('LoggedAdmin'))->first()];

        $user_data = user_role::where('id', '!=', config('constants.options.HEADS'))->get();

        $id = $request->id;
        $users_and_roles = DB::table('users_and_roles')->where('uar_role_id', '=', $id)->get();

        $existing = array();
        if (count($users_and_roles)) {
            foreach ($users_and_roles as $row) {
                $existing[] = $row->uar_user_id;
            }
        }

        $users = Admin::orderBy('firstname', 'ASC')->get();
        
        $isHead = ($id == config('constants.options.HEADS')) ? true : false;

        return view('master-approval-order.view-users-and-roles2', $data)
            ->with('user_data', $user_data)
            ->with('id', $id)
            ->with('ex', $existing)
            ->with('role', $id)
            ->with('users', $users)
            ->with('isHead', $isHead);
    }

    public function saveUserRoles(Request $request)
    {
        
        $role = $request->role;
        $ids = $request->ids;
        $ids = explode(',', $ids);
        $user_id = Helper::user_id();

        $currentTimestamp = time();
        $twoYearsFromNow = strtotime('+2 years', $currentTimestamp);

        users_and_roles::where("uar_role_id", '=', $role)->delete();

        foreach ($ids as $id) {
            if ($id) {
                $done = users_and_roles::updateOrCreate(
                    [
                        "uar_user_id" => $id,
                        "uar_role_id" => $role,
                    ],
                    [
                        "uar_date_added" => time(),
                        "uar_added_by" => $user_id,                     
                        "uar_delegatee_from" => time(),
                        "uar_delegatee_to" => $twoYearsFromNow,

                    ]
                );
            }
        }

    }

}


// $role = $request->role;
//         $division = $request->division;
//         $ids = $request->user_id;
//         $ids = explode(',', $ids);
//         $user_id = Helper::user_id();

//         users_and_roles::where("uar_role_id", '=', $role)->delete();
        
//         $currentTimestamp = time();
//         $twoYearsFromNow = strtotime('+2 years', $currentTimestamp);

//         foreach ($ids as $id) {
//             if ($id) {
//                 $done = users_and_roles::updateOrCreate(
//                     [
//                         "uar_role_id" => $role,
//                         "uar_head" => $division,
//                     ],
//                     [
//                         "uar_user_id" => $id,
//                         "uar_date_added" => time(),
//                         "uar_added_by" => $user_id,                        
//                         "uar_delegatee_from" => time(),
//                         "uar_delegatee_to" => $twoYearsFromNow,
//                     ]
//                 );

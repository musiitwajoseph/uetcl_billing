<?php
Class JournalMapping extends BeforeAndAfter{
  public $page = "USERS";
  
  public function __construct(){
    $access = new AccessRights();
    //$access->pageAccess(user_id(), $this->page, 'V');
  }
  
  public static function getLinks(){
    $page = "USERS";
    $links = array(
    // array(
    //     "link_name"=>"Journals", 
    //     "link_address"=>"journal-mapping/mapping",
    //     "link_icon"=>"fa-eye",
    //     "link_page"=>$page,
    //     "link_right"=>"V",
    //   ),
      array(
       "link_name"=>"Pending Journals", 
       "link_address"=>"journal-mapping/pending-journals",
       "link_icon"=>"fa-eye",
       "link_page"=>$page,
       "link_right"=>"V",
      ),
     array(
       "link_name"=>"Posted Journals", 
       "link_address"=>"journal-mapping/posted-journals",
       "link_icon"=>"fa-check",
       "link_page"=>$page,
       "link_right"=>"V",
      ),
     
      
      
    );
    
    return $links;
  }
  


  public function pendingJournalsAction(){
  $access = new AccessRights();
  ?>
    <div class="col-md-12">
      
      <?php 
      $db = new Db();
      $select = $db->select("SELECT * FROM other_invoice WHERE oi_status != '-1' AND oi_counter IS NOT NULL AND oi_jv_ref IS NULL ORDER BY oi_id DESC");
      
      if(!$select){
        $db->error();
      }else{
        echo '<table cellspacing="0" id="table" cellpadding="2"  style="font-size:8px!important;" border="1" width="100%">';
        echo '<thead>';
        echo '<tr>';
        echo '<th width="30px">No.</th>';
        echo '<th>Date</th>';
        //echo '<th>Category</th>';
        echo '<th>Customer</th>';
        echo '<th>Category</th>';
        echo '<th>Ref No.</th>';
        echo '<th>FDN</th>';
        echo '<th>JV Ref</th>';

        if($access->sectionAccess(user_id(), $this->page, 'E') || $access->sectionAccess(user_id(), $this->page, 'D')){
          echo '<th width="">Action</th>';
        }
        echo '</tr>';
        echo '</thead>';
        
        $i=1;
        echo '<tbody>';
        foreach($select[0] as $row){
          extract($row);
          echo '<tr>';
          echo '<td><center>'.($i++).'.</center></td>'; 
          echo '<td>'.FeedBack::date_s($oi_date).'</td>';
          //echo '<td>'.$this->rgf("categories",$oi_category,"cat_id","cat_name").'</td>';
          echo '<td>';
          if($oi_source=='PURCHASES'){
             echo $this->rgf("generators",$oi_customer,"ge_id","ge_name");
          }elseif($oi_source=='FIBRE'){
            echo $this->rgf("of_customer",$oi_customer,"ofc_id","ofc_customer_name");
          }elseif($oi_source=='ENERGY'){
             echo $this->rgf("customer",$oi_customer,"customer_id","customer_full_name");
          }elseif($oi_source=='OTHERINVOICES'){
            echo $this->rgf("customers",$oi_customer,"cust_id","cust_name");
          }

          echo'</td>';
          echo '<td>'.$oi_source.'</td>';
          echo '<td>'.$oi_ref_no.'</td>';
          echo '<td>'.$oi_fdn.'</td>';
          echo '<td>'.$oi_jv_ref.'</td>';
          
          if($access->sectionAccess(user_id(), $this->page, 'E') || $access->sectionAccess(user_id(), $this->page, 'D')){
          
            echo '<td>';
         
            echo"<a class='btn btn-primary btn-xs' href=".return_url().'journal-mapping/view-gl-account/'.$oi_id.">View JV</a> ";
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
  
  public function postedJournalsAction(){
  $access = new AccessRights();
  ?>
    <div class="col-md-12">
      
      <?php 
      $db = new Db();
      $select = $db->select("SELECT * FROM other_invoice WHERE oi_counter IS NOT NULL AND oi_jv_ref IS NOT NULL ORDER BY oi_id DESC");
      
      if(!$select){
        $db->error();
      }else{
        echo '<table cellspacing="0" id="table" cellpadding="2"  style="font-size:8px!important;" border="1" width="100%">';
        echo '<thead>';
        echo '<tr>';
        echo '<th width="30px">No.</th>';
        echo '<th>Date</th>';
        //echo '<th>Category</th>';
        echo '<th>Customer</th>';
        echo '<th>Category</th>';
        echo '<th>Ref No.</th>';
        echo '<th>FDN</th>';
        echo '<th>JV Ref</th>';

        if($access->sectionAccess(user_id(), $this->page, 'E') || $access->sectionAccess(user_id(), $this->page, 'D')){
          echo '<th width="">Action</th>';
        }
        echo '</tr>';
        echo '</thead>';
        
        $i=1;
        echo '<tbody>';
        foreach($select[0] as $row){
          extract($row);
          echo '<tr>';
          echo '<td><center>'.($i++).'.</center></td>'; 
          echo '<td>'.FeedBack::date_s($oi_date).'</td>';
          //echo '<td>'.$this->rgf("categories",$oi_category,"cat_id","cat_name").'</td>';
          echo '<td>';
          if($oi_source=='PURCHASES'){
             echo $this->rgf("generators",$oi_customer,"ge_id","ge_name");
          }elseif($oi_source=='FIBRE'){
            echo $this->rgf("of_customer",$oi_customer,"ofc_id","ofc_customer_name");
          }elseif($oi_source=='ENERGY'){
             echo $this->rgf("customer",$oi_customer,"customer_id","customer_full_name");
          }elseif($oi_source=='OTHERINVOICES'){
            echo $this->rgf("customers",$oi_customer,"cust_id","cust_name");
          }

          echo'</td>';
          echo '<td>'.$oi_source.'</td>';
          echo '<td>'.$oi_ref_no.'</td>';
          echo '<td>'.$oi_fdn.'</td>';
          // echo '<td>'.$oi_title.'</td>';
          echo '<td>'.$oi_jv_ref.'</td>';
          
          if($access->sectionAccess(user_id(), $this->page, 'E') || $access->sectionAccess(user_id(), $this->page, 'D')){
          
            echo '<td>';
         
            echo"<a class='btn btn-primary btn-xs' href=".return_url().'journal-mapping/view-gl-account/'.$oi_id.">View JV</a> ";
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
  
  public function viewGlAccountAction(){
    $id = portion(3);
     $db = new Db();
     $select = $db->select("SELECT * FROM other_invoice WHERE oi_id='$id'");

    //  print_r($select);
    $gl_details = $select[0];

     if($db->num_rows()){
        extract($select[0][0]);
     }
     

    $this->start_print("JOURNAL VOUCHER");


   $this->GL_ACCOUNT($oi_ref_no,$oi_post_date,$oi_invoice_date,$oi_customer_id,$oi_source, $gl_details,$oi_currency);

   
   if($oi_jv_ref)
    echo '<h4>Journal Number: '.$oi_jv_ref.'</h4>';

    if($oi_status == 10 && !$oi_jv_ref){

      echo '<button id="pushToSUN" class="btn btn-xs btn-success"><i class="fa fa-fw fa-plane"></i> Post to SUN</button>';
      echo '<input type="hidden" value="'.$id.'" id="id"/>';
      echo '<div id="pushStatus"></div>';
      ?>
      <script type="text/javascript">

        $(document).ready(function(){
          $('#pushToSUN').click(function(){

            if(!confirm("Do you really want to Push this Invoice")) return false;

            $('#pushToSUN').attr('disabled', true);

            $('#pushStatus').html('<?php echo '<img src="'.return_url().'images/loading.gif" alt="Loading ..."/>'; ?> Please Wait.');
            
            var form_data = new FormData(); 

            form_data.append('id', $('#id').val());
            $.ajax({
              url: '<?php echo return_url().'ajax/push-to-SUN.php'; ?>',
              type: "POST",
              data: form_data,
              contentType: false,
              cache: false,
              processData:false,
              success: function(data){
                // $('#pushToSUN').attr('disabled', false);
                // $('#pushStatus').html(data);
                alert("Posted with: "+data);

                location.reload();
              }
            });
          });
        });

      </script>      
      <?php

     }


    $masterApproval = new MaJv();
    $masterApproval->display([
        'table' => 'other_invoice',
        'status' => 'oi_status',
        'counter' => $oi_counter,
        'counter_field' => 's_counter',
        'id_field' => 'oi_id',    
        'where' => 'JV',
        'id' => $oi_id,
        'notify_user' => $oi_added_by,
        'reference' => $oi_ref_no,
    ]);


    $this->end_print();

  }
public function deleteCategoryAction(){
    $id = portion(3);
    $this->deletor("dictionary_category", "id",  $id, 'dictionary/category');
  }
  
  public function MappingAction(){
    $id2 = portion(3);
    $db = new Db();
    
   
    ?>
  
   <div class="col-md-12">
       
   
      <?php 
      $access = new AccessRights();
      $db = new Db();
      $select = $db->select("SELECT * FROM invoice_track");
      //print_r($select);
      if(!$select){
        $db->error();
      }else{
        echo '<table cellspacing="0" cellpadding="2" border="1" width="100%" id="table">';
        echo '<thead>';
        echo '<tr>';
        echo '<th width="">GL Account</th>';
        echo '<th>Description</th>';
        echo '<th>Accounting Period</th>';
        echo '<th>Base Amount</th>';
        echo '<th>Debit/Credit</th>';
        echo '<th>Transaction Amount</th>';
        echo '<th>Transaction</th>';
        echo '<th>Transaction Reference</th>';
        
        // if($access->sectionAccess(user_id(), $this->page, 'E') || $access->sectionAccess(user_id(), $this->page, 'D')){
        //   echo '<th width="100px">Action</th>';
        // }
        echo '</tr>';
        echo '</thead>';
        
        $i=1;
        echo '<tbody>';
        foreach($select[0] as $row){
          extract($row);

          if($it_type=="ENERGY"){
          $journal_no = $this->rgf("customer", $it_customer, "customer_id", "customer_journal_no");
          $customer = $this->rgf("customer", $it_customer, "customer_id", "customer_short_name");
          }else{
            $journal_no = $this->rgf("of_customer", $it_customer, "ofc_id", "ofc_journal_no");
            $customer = $this->rgf("of_customer", $it_customer, "ofc_id", "ofc_short_name");
          }

          echo '<tr>';
          echo '<td>'.$journal_no.'</td>';
          echo '<td>'.ucwords(strtolower($it_type)).' Sales of '.month_name($it_month).' '.$it_year.'</td>';//PESS
          echo '<td>'.$this->period(strtotime($it_year.'-'.$it_month.'-20')).'</td>';
          echo '<td style="text-align:right;">'.number_format($it_total_amount,2).'</td>';  
          echo '<td>D</td>';
          echo '<td></td>';
          echo '<td>'.$customer.' '.($it_invoice_id).'</td>';
          echo '<td>PESS/Inv.'.($it_id).'</td>';//PESS  
          echo '</tr>';

          if($it_type=="ENERGY"){
          echo '<tr>';
          echo '<td>220008</td>';
          echo '<td>'.ucwords(strtolower($it_type)).' Sales of '.month_name($it_month).' '.$it_year.'</td>';//PESS
          echo '<td>'.$this->period(strtotime($it_year.'-'.$it_month.'-20')).'</td>';
          echo '<td style="text-align:right;">'.number_format($it_total_vat*-1,2).'</td>';  
          echo '<td>C</td>';
          echo '<td></td>';
          echo '<td>'.$customer.' '.($it_invoice_id).'</td>';
          echo '<td>PESS/Inv.'.($it_id).'</td>';
          echo '</tr>';

          echo '<tr>';
          echo '<td>[TOTAL_VAT_EX]</td>';
          echo '<td>'.ucwords(strtolower($it_type)).' Sales of '.month_name($it_month).' '.$it_year.'</td>';//PESS
          echo '<td>'.$this->period(strtotime($it_year.'-'.$it_month.'-20')).'</td>';
          echo '<td style="text-align:right;">'.number_format($it_total_vat_ex_tax*-1,2).'</td>';  
          echo '<td>C</td>';
          echo '<td></td>';
          echo '<td>'.$customer.' '.($it_invoice_id).'</td>';
          echo '<td>PESS/Inv.'.($it_id).'</td>';//PESS    
          echo '</tr>';
          }else{
          echo '<tr>';
          echo '<td>220009</td>';
          echo '<td>'.ucwords(strtolower($it_type)).' Sales of '.month_name($it_month).' '.$it_year.'</td>';//PESS
          echo '<td>'.$this->period(strtotime($it_year.'-'.$it_month.'-20')).'</td>';
          echo '<td style="text-align:right;">'.number_format($it_total_vat*-1,2).'</td>';  
          echo '<td>C</td>';
          echo '<td></td>';
          echo '<td>'.($it_invoice_id).'</td>';
          echo '<td>PESS/Inv.'.($it_id).'</td>';
          echo '</tr>';

          echo '<tr>';
          echo '<td>320008</td>';
          echo '<td>'.ucwords(strtolower($it_type)).' Sales of '.month_name($it_month).' '.$it_year.'</td>';//PESS
          echo '<td>'.$this->period(strtotime($it_year.'-'.$it_month.'-20')).'</td>';
          echo '<td style="text-align:right;">'.number_format($it_total_vat_ex_tax*-1,2).'</td>';  
          echo '<td>C</td>';
          echo '<td></td>';
          echo '<td>'.($it_invoice_id).'</td>';
          echo '<td>PESS/Inv.'.($it_id).'</td>';//PESS    
          echo '</tr>';
          }    


          // echo '<td>'.($it_year).'</td>';
          // echo '<td>'.($it_month).'</td>';
          // echo '<td>'.($it_type).'</td>';
          // echo '<td>'.number_format($it_total_vat).'</td>';
          // echo '<td>'.number_format($it_total_vat_ex_tax).'</td>';
          // echo '<td>'.$this->rgf("dictionary",$it_currency,"d_code","d_name").'</td>';
          // if($access->sectionAccess(user_id(), $this->page, 'E') || $access->sectionAccess(user_id(), $this->page, 'D')){
          
          //   echo '<td>';
          //   if($access->sectionAccess(user_id(), $this->page, 'E'))
          //     //echo $this->action('edit','dictionary/category/'.$id, 'Edit');
          //    echo '<a class="btn-xs btn btn-success" href="'.return_url().'dictionary/category/'.$id.'">Edit</a>';
          //   //if($access->sectionAccess(user_id(), $this->page, 'D'))
          //     echo $this->action('delete','dictionary/delete-category/'.$id, 'Delete');
          //   echo '</td>';
          // }
         
        }
        echo '</tbody>';
        
        echo '</table>';
        
      
        
      }
      ?>
   </div> 
  <?php
  }
 public function importDictionaryAction(){
    $db = new Db();
    $time = time();
    $user = user_id();
 
    if (isset($_POST['send'])) {
      $category = $_POST['category'];
        $errors = array();
     
      $filename=$_FILES["file"]["tmp_name"];
      if($_FILES["file"]["size"] > 0){

        $valid_name = "Name";
        $file = fopen($filename, "r");
        while (($emapData = fgetcsv($file, 10000, ",")) !== FALSE){
          $count++;
          $col_1 = @trim(str_replace("'","\'",strip_tags($emapData[0])));
          $col_2 = @trim(str_replace("'","\'",strip_tags($emapData[1])));
          $col_3 = @trim(str_replace("'","\'",strip_tags($emapData[2])));
       

          if($count == 1){
           //echo "$col_1 is equal to $valid_name";
           if($col_1 != $valid_name){
             $errors[] = "Invalid Template";
           }
          }
          
          
          echo $db->error();
        }
         if(empty($category)){
            $errors[] = "Please Select Category name";
          } 
          
        fclose($file);

        if(empty($errors)){
          $count = 0;
          $db = new Db();
          $file = fopen($filename, "r");
          while (($emapData = fgetcsv($file, 10000, ",")) !== FALSE){
            $count++;
            if($count > 1){
            $col_1 = @trim(str_replace("'","\'",strip_tags($emapData[0])));
            $col_2 = @trim(str_replace("'","\'",strip_tags($emapData[1])));
            $col_3 = @trim(str_replace("'","\'",strip_tags($emapData[2])));
            
           
            $db = new Db();
            $db->insert("dictionary",["d_name"=>$col_1,"d_description"=>$col_2,"d_code"=>$col_3,"d_date_added"=>$time,"d_added_by"=>$user,"d_category"=>$category]);
          
            
          }

          }

          if(!$db->error()){
            FeedBack::success();
            FeedBack::refresh(3, return_url().'dictionary/import-dictionary');
          }else{
            FeedBack::errors($errors);
          }
        }else{
          FeedBack::errors($errors);
        }
      }else{ 
        $errors[] = "Please Attach file";
      }

    
      }

   echo '<form action="" method="post" enctype="multipart/form-data">';
    
    echo '<div class="col-md-3">';
    echo '<label>Category</label>';
    echo '<select class="form-control" name="category" style="height:;">';
    echo "<option value=''>-----select-----</option>";
    $db = new Db();
      $select = $db->select("SELECT * from dictionary_category");
      foreach ($select[0] as $rows) {
        extract($rows);
          echo'<option value='.$id.'>'.$dc_name.'</option>';
      }
    echo '</select>';
    echo '</div>';
    
    echo '<div class="col-md-3">';
    echo '<label>Attach Dictionary File</label>';
    echo '<input type="file" name="file">';
    echo '</div>';

    echo '<div class="col-md-3">';
    echo '<br/>';
    echo '<button name="send" type="submit"><i class="fa fa-fw fa-plane"></i> Upload</button>';
    echo '</div>';
    echo '</form>';
  
  }
 public function deleteDictionaryAction(){
     $id = portion(3);
    $this->deletor("dictionary", "d_id",  $id, 'dictionary/dictionary');
  }
  
  public function addDictionaryAction(){
    $id2 = portion(3);
    $db = new Db();
    if($id2){
     $select=$db->select("select * from dictionary WHERE d_id='$id2'");
     if($db->num_rows());
     extract($select[0][0]);
    }
    if(isset($_POST['submit'])){
    
      $category = $_POST['category'];
      $name = $_POST['name'];
      $code = $_POST['code'];
      $description = $_POST['description'];
     
      $time = time();
      $user = user_id();
      $db = new Db();
      
      $errors = array();
      
      if(empty($category)){
        $errors[]="Enter category";
      }
      
      
      if(empty($errors)){
          if($id2){
          $db->update("dictionary",["d_name"=>$name,"d_description"=>$description,"d_code"=>$code,"d_date_added"=>$time,"d_added_by"=>$user,"d_category"=>$category],["d_id"=>$id2]);
          }else{
            $db->insert("dictionary",["d_name"=>$name,"d_description"=>$description,"d_code"=>$code,"d_date_added"=>$time,"d_added_by"=>$user,"d_category"=>$category]);
          }

        if(!$db->error()){
          FeedBack::success();
          if($id2){
             FeedBack::refresh(3, return_url().'dictionary/add-dictionary/'.$id2);
          }else{
             FeedBack::refresh(3, return_url().'dictionary/add-dictionary');
          }
         
        }else{
          FeedBack::error();
        }
      }else{
        FeedBack::errors($errors);
      }
    }
    ?>
    <div class="col-md-3">
  
          <div class="row">
            <div class="col-lg-12">
              <form role="form" action="" method="post">
                <div class="form-group">
                 
                    <label>Category Name<span class="must">*</span></label>
                     <div class="form-line">
                    <?php
                     echo '<select class="form-control" name="category" style="height:;">';
                        echo "<option value=''>-----select-----</option>";
                        $db = new Db();
                          $select = $db->select("SELECT * from dictionary_category");
                          foreach ($select[0] as $rows) {
                            extract($rows);
                            if($d_category ==$id)
                              echo'<option selected value='.$id.'>'.$dc_name.'</option>';
                              else
                              echo'<option value='.$id.'>'.$dc_name.'</option>';
                          }
                        echo '</select>';
                    ?>
                  </div>
                  </div>
                  <div class="form-group">
                    <label>Name<span class="must">*</span></label>
                   <div class="form-line">
                     <input type="text" placeholder="Name" value="<?php echo$d_name;?>"  name="name" class="form-control">
                   </div>
                  </div>
                  <div class="form-group">
                    <label>Code<span class="must">*</span></label>
                   <div class="form-line">
                     <input type="text" placeholder="Code" value="<?php echo$d_code;?>" name="code" class="form-control">
                   </div>
                  </div>
                   <div class="form-group">
                    <label>Description<span class="must">*</span></label>
                   <div class="form-line">
                     <textarea class="form-control" name="description" placeholder="Description"><?php echo $d_description;?></textarea>
                   </div>
                  </div>
                  <?php
                    if ($id2) {
                      echo '<button type="submit" name="submit" class="btn btn-primary"><i class="fa fa-fw fa-save"></i> Update</button>';
                    }else{
                       echo'<button type="submit" name="submit" class="btn btn-primary"><i class="fa fa-fw fa-save"></i> Save</button>';
                    }
                  ?>
                 
              </form>
            </div>
          </div>
        
   </div>
  <?php 
 } 
 public function dictionaryAction(){
   $access = new AccessRights();
      $db = new Db();
      $select = $db->select("SELECT * FROM dictionary ORDER BY d_name ASC");
      //print_r($select);
      if(!$select){
        $db->error();
      }else{
        echo '<table cellspacing="0" cellpadding="2" border="1" width="100%" id="table">';
        echo '<thead>';
        echo '<tr>';
        echo '<th width="30px">No.</th>';
        echo '<th>Name</th>';
        echo '<th>Code</th>';
        echo '<th>Description</th>';
        echo '<th>Category</th>';
        
        if($access->sectionAccess(user_id(), $this->page, 'E') || $access->sectionAccess(user_id(), $this->page, 'D')){
          echo '<th width="100px">Action</th>';
        }
        echo '</tr>';
        echo '</thead>';
        
        $i=1;
        echo '<tbody>';
        foreach($select[0] as $row){
          extract($row);
          echo '<tr>';
          echo '<td><center>'.($i++).'.</center></td>';
          echo '<td>'.($d_name).'</td>';
          echo '<td>'.($d_code).'</td>';
          echo '<td>'.($d_description).'</td>';
          echo '<td>'.$this->rgf("dictionary_category",$d_category,"id","dc_name").'</td>';
          if($access->sectionAccess(user_id(), $this->page, 'E') || $access->sectionAccess(user_id(), $this->page, 'D')){
          
            echo '<td>';
            if($access->sectionAccess(user_id(), $this->page, 'E'))
              //echo $this->action('edit','dictionary/add-dictionary/'.$d_id, 'Edit');
            echo '<a class="btn-xs btn btn-success" href="'.return_url().'dictionary/add-dictionary/'.$d_id.'">Edit</a>';
            //if($access->sectionAccess(user_id(), $this->page, 'D'))
              echo $this->action('delete','dictionary/delete-dictionary/'.$d_id, 'Delete');
            echo '</td>';
          }
          echo '</tr>';
         
        }
        echo '</tbody>';
        
        echo '</table>';
        
      
        
      }
 }
}
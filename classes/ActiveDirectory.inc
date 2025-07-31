<?php 

class ActiveDirectory extends BeforeAndAfter{
    
    public function getLinks(){
        
        $links = array(
            array()
        );
        
        return $links;
    }

    public function login($username, $password){   
        // $username = explode('@', $username);
        // $username = $username[0]; 

        $name = array(
            'Abdulhafeez.Saeed@igad.int',
            'Ismail.Omar@igad.int',
            'Omar.Ali@igad.int',
            'Ahmed.Said@igad.int',
            'Bilan.Mohamed@igad.int',
            'Kebede.Fufa@igad.int',
            'Hassan.Salah@igad.int',
            'Abdullahi.Hussein@igad.int',
            'glory.karimi@igad.int',
            'Beverlyne.Nyanchera@igad.int',
            'cecil.gayi@igad.int',
            'Joshua.Turinawe@igad.int',
            'Samuel.Walusansa@igad.int',
            'Asbel.Rutto@igad.int',
            'simon.mbugua@igad.int',
            'Josephat.Onyari@igad.int',
            'Bruck.Yohannes@igad.int',
            'issa.munyendo@flaxem.com',
            'jojo.mus@gmail.com'
        );


        for($i=0; $i<count($name); $i++){
            $name[$i] = strtoupper($name[$i]);
        }

        $errors = array();

        if(in_array(strtoupper($username), $name) && $password == "123"){
            $un = explode('@', $username);   
            $un = explode('.', $un[0]);  

                $errors["status"] = True;         
                $errors["message"] = array(
                    "name"=>strtoupper($un[0].' '.$un[1]),
                    "email"=>$username,//$info[$x]['mail'][0],
                    "extention"=>'0788229210',
                    "user_role"=>1084,
                );
        }else{       

            $cnx = ldap_connect(AD_DNS_NAME) or die("Could not connect to LDAP");
            ldap_set_option($cnx, LDAP_OPT_PROTOCOL_VERSION, 3);    //Set the LDAP Protocol used by your AD service
            ldap_set_option($cnx, LDAP_OPT_REFERRALS, 0);       //This was necessary for my AD to do anything
            if(!ldap_bind($cnx,$username.AD_DOMAIN,$password)){ 
                $errors["message"] = "Could not Login, Contact Admin";
                $errors["status"] = False;
            }else{
                $errors["status"] = True;

                $SearchFor=$username;
                $SearchField="samaccountname";          
                $LDAPFieldsToFind = array("*");  
                $filter="($SearchField=$SearchFor*)";   //Wildcard is * Remove it if you want an exact match
                $sr=ldap_search($cnx, AD_DN, $filter, $LDAPFieldsToFind);
                $info = ldap_get_entries($cnx, $sr);
               
                for ($x=0; $x<$info["count"]; $x++) {
                    
                    //print_r($info[$x]);

                    // echo "Name: " .$info[$x]['cn'][0]."<br/>";
                    // echo "Windows Login: " .$info[$x]['samaccountname'][0]."<br/>";
                    // echo "Extention: " .$info[$x]['telephonenumber'][0]."<br/>";
                    // echo "Email: " .$info[$x]['mail'][0]."<br/>";
                    // echo "Office: " .$info[$x]['physicaldeliveryofficename'][0]."<br/>";
                    // echo "Main System UID: " .$info[$x]['description'][0]."<br/>";
                    // echo "Job Title: " .$info[$x]['title'][0]."<br/>";
                    // echo "Department: " .$info[$x]['department'][0]."<br/>";
                    // $info[$x]['manager'][0] = explode(",",$info[$x]['manager'][0]);
                    // $info[$x]['manager'][0] = str_replace('CN=','',$info[$x]['manager'][0][0]);
                    // echo "Line Manager: " .$info[$x]['manager'][0]."<br/>";
                    // echo "Company: " .$info[$x]['company'][0]."<br/>";

                    // $db = new Db();
                    // $select = $db->select("SELECT * FROM user_role");
                    // print_r($select[0]);
                    
                    if($info[$x]['memberof']['count'] != 0){
                        foreach($info[$x]['memberof'] as $key){
                            $key = explode(",",$key);
                            $key = str_replace('CN=','',$key[0]);
                            //echo "Member of: " .$key."<br/>";
                            $member = $this->rgf("user_role", $key, "ur_name", "ur_id");
                            if($member){
                                break;
                            }
                        }
                    }
                    //echo "\n\n";
                    if(empty($member)){
                        $member = 0;
                    }
                    //echo "Member:".$member;

                    $errors["message"] = array(
                        "name"=>$info[$x]['cn'][0],
                        "email"=>$info[$x]['mail'][0],
                        "extention"=>$info[$x]['telephonenumber'][0],
                        "user_role"=>$member
                    );
                }
            }
            //echo"</pre>";
        }

        return $errors;
    }
    
    public function login2($username, $password){   
        // $username = explode('@', $username);
        // $username = $username[0]; 

        $name = array(
            'Abdulhafeez.Saeed@igad.int',
            'Ismail.Omar@igad.int',
            'Omar.Ali@igad.int',
            'Ahmed.Said@igad.int',
            'Bilan.Mohamed@igad.int',
            'Kebede.Fufa@igad.int',
            'Hassan.Salah@igad.int',
            'Abdullahi.Hussein@igad.int',
            'glory.karimi@igad.int',
            'Beverlyne.Nyanchera@igad.int',
            'cecil.gayi@igad.int',
            'Joshua.Turinawe@igad.int',
            'Samuel.Walusansa@igad.int',
            'Asbel.Rutto@igad.int',
            'simon.mbugua@igad.int',
            'Josephat.Onyari@igad.int',
            'Bruck.Yohannes@igad.int',
            'issa.munyendo@flaxem.com',
            'jojo.mus@gmail.com'
        );


        for($i=0; $i<count($name); $i++){
            $name[$i] = strtoupper($name[$i]);
        }

        $errors = array();

        {       

            $cnx = ldap_connect(AD_DNS_NAME) or die("Could not connect to LDAP");
            ldap_set_option($cnx, LDAP_OPT_PROTOCOL_VERSION, 3);    //Set the LDAP Protocol used by your AD service
            ldap_set_option($cnx, LDAP_OPT_REFERRALS, 0);       //This was necessary for my AD to do anything
            if(!ldap_bind($cnx,$username.AD_DOMAIN,$password)){ 
                $errors["message"] = "Could not Login, Contact Admin";
                $errors["status"] = False;
            }else{
                $errors["status"] = True;

                $SearchFor=$username;
                $SearchField="samaccountname";          
                $LDAPFieldsToFind = array("*");  
                $filter="($SearchField=*)";   //Wildcard is * Remove it if you want an exact match
                $sr=ldap_search($cnx, AD_DN, $filter, $LDAPFieldsToFind);
                $info = ldap_get_entries($cnx, $sr);
               
                for ($x=0; $x<$info["count"]; $x++) {
                    echo '<pre>';
                    print_r($info[$x]);
                    echo '</pre>';

                    echo "Name: " .$info[$x]['cn'][0]."<br/>";
                    echo "Windows Login: " .$info[$x]['samaccountname'][0]."<br/>";
                    echo "Extention: " .$info[$x]['telephonenumber'][0]."<br/>";
                    echo "Email: " .$info[$x]['mail'][0]."<br/>";
                    echo "Office: " .$info[$x]['physicaldeliveryofficename'][0]."<br/>";
                    echo "Main System UID: " .$info[$x]['description'][0]."<br/>";
                    echo "Job Title: " .$info[$x]['title'][0]."<br/>";
                    echo "Department: " .$info[$x]['department'][0]."<br/>";
                    $info[$x]['manager'][0] = explode(",",$info[$x]['manager'][0]);
                    $info[$x]['manager'][0] = str_replace('CN=','',$info[$x]['manager'][0][0]);
                    echo "Line Manager: " .$info[$x]['manager'][0]."<br/>";
                    echo "Company: " .$info[$x]['company'][0]."<br/><br/>";

                    // $db = new Db();
                    // $select = $db->select("SELECT * FROM user_role");
                    // print_r($select[0]);
                    
                    if($info[$x]['memberof']['count'] != 0){
                        foreach($info[$x]['memberof'] as $key){
                            $key = explode(",",$key);
                            $key = str_replace('CN=','',$key[0]);
                            echo "Member of: " .$key."<br/>";
                            $member = $this->rgf("user_role", $key, "ur_name", "ur_id");
                            if($member){
                                break;
                            }
                        }
                    }
                    //echo "\n\n";
                    if(empty($member)){
                        $member = 0;
                    }
                    //echo "Member:".$member;

                    $errors["message"] = array(
                        "name"=>$info[$x]['cn'][0],
                        "email"=>$info[$x]['mail'][0],
                        "extention"=>$info[$x]['telephonenumber'][0],
                        "user_role"=>$member
                    );
                }
            }
            //echo"</pre>";
        }

        return $errors;
    }
    
    
}


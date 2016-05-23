<?php

/**
 * @file
 * Defines all hook menu callbacks.
 */

/**
 * Callback function for settings form.
 */
function instagram_settings() {

  $access_key = variable_get('instagram_api_key', '');
  // Access token request in process.
  if (isset($_GET['code']) && $_GET['code'] != '') {
    if ($access_key == '') {
      $url = "https://api.instagram.com/oauth/access_token";
      $fields = array(
        "client_id" => variable_get("instagram_client_id"),
        "client_secret" => variable_get("instagram_client_secret"),
        "grant_type" => "authorization_code",
        "redirect_uri" => variable_get("instagram_redirect_uri"),
        "code" => $_GET['code'],
      );

      $fields_string = '';
      foreach ($fields as $key => $value) {
        $fields_string .= $key . '=' . $value . '&';
      }
      rtrim($fields_string, '&');

      // Request access token.
      $ch = curl_init();
      curl_setopt($ch, CURLOPT_URL, $url);
      curl_setopt($ch, CURLOPT_RETURNTRANSFER, 1);
      curl_setopt($ch, CURLOPT_POST, count($fields));
      curl_setopt($ch, CURLOPT_POSTFIELDS, $fields_string);

      $output = curl_exec($ch);
      curl_close($ch);

      $auth = json_decode($output);

      if (empty($auth->error_message)) {
        variable_set('instagram_api_key', $auth->access_token);
        variable_set('instagram_user_id', $auth->user->id);
        variable_set('instagram_username', $auth->user->username);

        $access_key = $auth->access_token;
        drupal_set_message(t('Instagram authentication successful'));
      }
      else {
        drupal_set_message($auth->error_message, 'error');
      }
    }
  }
  elseif (array_key_exists('code', $_GET) && $_GET['code'] == '') {

    // Remove api key for re-authentication.
    variable_del('instagram_api_key');
    // Unset variable for form.
    $access_key = '';
  }

  $form = array();
  if ($access_key == '') {

    // Non-authenticated settings form.
    $form['instagram_client_id'] = array(
      '#type' => 'textfield',
      '#title' => t('Instagram Client ID'),
      '#default_value' => variable_get('instagram_client_id', ''),
      '#size' => 60,
      '#maxlength' => 255,
      '#description' => t('You must register an Instagram client key to use this module. You can register a client by <a href="http://instagram.com/developer/clients/manage/" target="_blank">clicking here</a>.'),
    );
    $form['instagram_client_secret'] = array(
      '#type' => 'textfield',
      '#title' => t('Instagram Client Secret'),
      '#default_value' => variable_get('instagram_client_secret', ''),
      '#size' => 60,
      '#maxlength' => 255,
      '#description' => t('The client secret can be found after creating an Instagram client in the API console.'),
    );
    $form['instagram_redirect_uri'] = array(
      '#type' => 'textfield',
      '#title' => t('Instagram Redirect URI'),
      '#default_value' => variable_get('instagram_redirect_uri', ''),
      '#size' => 60,
      '#maxlength' => 255,
      '#description' => t('Set the redirect URI to :url', array(
        ':url' => url('admin/config/services/instagram/settings'),
      )),
    );
    $form['instagram_follow_link'] = array(
      '#type' => 'textfield',
      '#title' => t("Instagram follow link"),
      '#default_value' => variable_get('instagram_follow_link', ''),
    );

    if (variable_get('instagram_client_id', '') != '' && variable_get('instagram_redirect_uri', '') != '') {
      $form['authenticate'] = array(
        '#markup' => l(t('Click here to authenticate via Instagram and create an access token'),
          'https://api.instagram.com/oauth/authorize/?client_id=' . variable_get('instagram_client_id') . '&redirect_uri=' . variable_get('instagram_redirect_uri') . '&response_type=code'
        )
      );
    }
  }
  else {

    // Authenticated user settings form.
//    $form['instagram_api_key'] = array(
//      '#type' => 'textfield',
//      '#title' => t('Instagram API Key'),
//      '#default_value' => variable_get('instagram_api_key', ''),
//      '#size' => 60,
//      '#maxlength' => 255,
//      '#disabled' => TRUE,
//      '#description' => t('Stored access key for accessing the API key'),
//    );
    $form['instagram_follow_link'] = array(
      '#type' => 'textfield',
      '#title' => t("Instagram follow link"),
      '#default_value' => variable_get('instagram_follow_link', ''),
    );
    $form['authenticate'] = array(
      '#markup' => l(t('Click here to remove the access key and re-authenticate via Instagram'),
        'admin/config/services/instagram/settings/', array(
          'query' => array('code' => ''),
        )
      ),
    );
  }

  return system_settings_form($form);

}

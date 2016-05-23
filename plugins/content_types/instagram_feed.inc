<?php

/**
 * @file
 */

/**
 * Plugins are described by creating a $plugin array which will be used
 * by the system that includes this file.
 */
$plugin = array(
  'single' => TRUE,
  'title' => t('Recent pictures from Instagram'),
  'description' => t('Display latest images from instagram by user ID.'),
  'category' => t('Widgets'),
  'edit form' => 'instagram_feed_config',
  'defaults' => array(
    'instagram_user_id' => 'self',
	'instagram_limit' => 10,
  ),
);

/**
 * Configuration form.
 */
function instagram_feed_config($form, $form_state) {
  $form['instagram_user_id'] = array(
    '#type' => 'textfield',
    '#title' => t('Instagram user id'),
	'#description' => t('The instagram user id to fetch most recent images from. Use "self" for the authenticated instagram user.'),
    '#default_value' => isset($form_state['conf']['instagram_user_id']) ? $form_state['conf']['instagram_user_id'] : '',
  );
  
  $form['instagram_limit'] = array(
    '#type' => 'textfield',
    '#title' => t('Limit'),
	'#description' => t('Number of images to fetch from instagram. Default to 10.'),
    '#default_value' => isset($form_state['conf']['instagram_limit']) ? $form_state['conf']['instagram_limit'] : '',
  );
  return $form;
}

/**
 * Submit handler for the configuration form.
 */
function instagram_feed_config_submit(&$form, &$form_state) {
  $form_state['conf']['instagram_user_id'] = $form_state['values']['instagram_user_id'];
  $form_state['conf']['instagram_limit'] = $form_state['values']['instagram_limit'];
}

/**
 * Render callback for the pane.
 * Initiliaze the search with facets
 */
function instagram_instagram_feed_content_type_render($subtype, $conf, $panel_args, &$contexts) {
  $block = new stdClass();

  $response = instagram_get_photos($conf['instagram_user_id'], $conf['instagram_limit'], TRUE);
  if ($response) {
    $block->content = theme('instagram_list', array(
      'photos' => $response,
	  'user_id' => $conf['instagram_user_id'],
	  'limit' => $conf['instagram_limit']
    ));
  }
  return $block;
}

<?php

/**
 * @file Rules integration for the Legal module.
 */

/**
 * Implements hook_rules_condition_info().
 */
function legal_rules_condition_info() {
  return array(
    'legal_user_has_accepted_tc' => array(
      'label' => t('User has accepted Terms and Conditions'),
      'parameter' => array(
        'account' => array('type' => 'user', 'label' => t('User')),
      ),
      'group' => t('User'),
    ),
  );
}

/**
 * Condition: User has accepted Terms and Conditions.
 */
function legal_user_has_accepted_tc($account) {
  global $language;

  $uid = $account->uid;

  // Don't check admin user (the same as in legal.module).
  if ($uid == 1) {
    return TRUE;
  }

  // Get last accepted version for this account
  $legal_account = legal_get_accept($uid);

  // If no version has been accepted yet, get version with current language revision.
  if (empty($legal_account['version'])) {
    $conditions = legal_get_conditions($language->language);
    // No conditions set yet.
    if (empty($conditions['conditions'])) {
      return TRUE;
    }
  }
  // Get version / revision of last accepted language.
  else {
    $conditions = legal_get_conditions($legal_account['language']);
    // No conditions set yet.
    if (empty($conditions['conditions'])) {
      return TRUE;
    }

    // Check latest version of T&C has been accepted.
    $accepted = legal_version_check($uid, $conditions['version'], $conditions['revision'], $legal_account);

    if ($accepted) {
      return TRUE;
    }
  }

  return FALSE;
}

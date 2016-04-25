/*
 * Licensed to the Apache Software Foundation (ASF) under one
 * or more contributor license agreements.  See the NOTICE file
 * distributed with this work for additional information
 * regarding copyright ownership.  The ASF licenses this file
 * to you under the Apache License, Version 2.0 (the
 * "License"); you may not use this file except in compliance
 * with the License.  You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 *
 */

#include "utils/user-util.h"

#include <pwd.h>
#include <unistd.h>
#include <sys/types.h>
#include <folly/Logging.h>

using namespace hbase;
using namespace std;

string UserUtil::user_name() {
  if (!init_) {
    compute_user_name();
  }
  return user_name_;
}

void UserUtil::compute_user_name() {
  lock_guard<mutex> lock(m_);

  if (init_) {
    return;
  }

  // According to the man page of getpwuid
  // this should never be free'd
  //
  // So yeah a raw pointer with no ownership....
  struct passwd *passwd = getpwuid(getuid());

  // make sure that we got something.
  if (passwd && passwd->pw_name) {
    user_name_ = string{passwd->pw_name};
    init_ = true;
  }
}

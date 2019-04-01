/**
 *
 * \section COPYRIGHT
 *
 * Copyright 2013-2017 Software Radio Systems Limited
 *
 * \section LICENSE
 *
 * This file is part of srsLTE.
 *
 * srsLTE is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Affero General Public License as
 * published by the Free Software Foundation, either version 3 of
 * the License, or (at your option) any later version.
 *
 * srsLTE is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Affero General Public License for more details.
 *
 * A copy of the GNU Affero General Public License can be found in
 * the LICENSE file in the top-level directory of this distribution
 * and at http://www.gnu.org/licenses/.
 *
 */
#ifndef SRSEPC_GTPC_H
#define SRSEPC_GTPC_H

#include "srsepc/hdr/spgw/spgw.h"
#include "srslte/asn1/gtpc.h"
#include "srslte/interfaces/epc_interfaces.h"

namespace srsepc {

class spgw::gtpc
{
public:
  gtpc();
  virtual ~gtpc();
  int  init(spgw_args_t* args, spgw* spgw, gtpu_interface_gtpc* gtpu, srslte::log_filter* gtpc_log);
  void stop();

  srslte::error_t init_ue_ip(spgw_args_t* args);

  uint64_t  get_new_ctrl_teid();
  uint64_t  get_new_user_teid();
  in_addr_t get_new_ue_ipv4();

  void handle_s11_pdu(srslte::gtpc_pdu* msg, srslte::gtpc_pdu* reply_msg);
  void handle_create_session_request(const srslte::gtpc_create_session_request& cs_req, srslte::gtpc_pdu* gtpc_pdu);
  void handle_modify_bearer_request(const srslte::gtpc_header&                mb_req_hdr,
                                    const srslte::gtpc_modify_bearer_request& mb_req,
                                    srslte::gtpc_pdu*                         mb_resp_pdu);
  void handle_delete_session_request(const srslte::gtpc_header&                 header,
                                     const srslte::gtpc_delete_session_request& del_req,
                                     srslte::gtpc_pdu*                          del_resp_pdu);
  void handle_release_access_bearers_request(const srslte::gtpc_header&                         header,
                                             const srslte::gtpc_release_access_bearers_request& rel_req,
                                             srslte::gtpc_pdu*                                  rel_resp_pdu);

  spgw_tunnel_ctx_t* create_gtpc_ctx(const srslte::gtpc_create_session_request& cs_req);
  bool               delete_gtpc_ctx(uint32_t ctrl_teid);

  spgw*                m_spgw;
  gtpu_interface_gtpc* m_gtpu;

  uint32_t m_h_next_ue_ip;
  uint64_t m_next_ctrl_teid;
  uint64_t m_next_user_teid;

  std::map<uint64_t, uint32_t> m_imsi_to_ctr_teid;           // IMSI to control TEID map. Important to check if UE
                                                             // is previously connected
  std::map<uint32_t, spgw_tunnel_ctx*> m_teid_to_tunnel_ctx; // Map control TEID to tunnel ctx. Usefull to get
                                                             // reply ctrl TEID, UE IP, etc.

  srslte::log_filter*       m_gtpc_log;
  srslte::byte_buffer_pool* m_pool;
};

inline uint64_t spgw::gtpc::get_new_ctrl_teid()
{
  return m_next_ctrl_teid++;
}

inline uint64_t spgw::gtpc::get_new_user_teid()
{
  return m_next_user_teid++;
}

inline in_addr_t spgw::gtpc::get_new_ue_ipv4()
{
  m_h_next_ue_ip++;
  return ntohl(m_h_next_ue_ip);
}

inline srslte::error_t spgw::gtpc::init_ue_ip(spgw_args_t* args)
{
  m_h_next_ue_ip = ntohl(inet_addr(args->sgi_if_addr.c_str()));
  return srslte::ERROR_NONE;
}

} // namespace srsepc
#endif // SRSEPC_SPGW_H
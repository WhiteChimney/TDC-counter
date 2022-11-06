#ifndef __VISA_HEADER__
// VISA replacements are only required if no real VISA is available.

//==============================================================================
// Constants

/*- Completion and Error Codes ----------------------------------------------*/

#define VI_SUCCESS_EVENT_EN                   (0x3FFF0002L) /* 3FFF0002,  1073676290 */
#define VI_SUCCESS_EVENT_DIS                  (0x3FFF0003L) /* 3FFF0003,  1073676291 */
#define VI_SUCCESS_QUEUE_EMPTY                (0x3FFF0004L) /* 3FFF0004,  1073676292 */
#define VI_SUCCESS_TERM_CHAR                  (0x3FFF0005L) /* 3FFF0005,  1073676293 */
#define VI_SUCCESS_MAX_CNT                    (0x3FFF0006L) /* 3FFF0006,  1073676294 */
#define VI_SUCCESS_DEV_NPRESENT               (0x3FFF007DL) /* 3FFF007D,  1073676413 */
#define VI_SUCCESS_TRIG_MAPPED                (0x3FFF007EL) /* 3FFF007E,  1073676414 */
#define VI_SUCCESS_QUEUE_NEMPTY               (0x3FFF0080L) /* 3FFF0080,  1073676416 */
#define VI_SUCCESS_NCHAIN                     (0x3FFF0098L) /* 3FFF0098,  1073676440 */
#define VI_SUCCESS_NESTED_SHARED              (0x3FFF0099L) /* 3FFF0099,  1073676441 */
#define VI_SUCCESS_NESTED_EXCLUSIVE           (0x3FFF009AL) /* 3FFF009A,  1073676442 */
#define VI_SUCCESS_SYNC                       (0x3FFF009BL) /* 3FFF009B,  1073676443 */

#define VI_WARN_QUEUE_OVERFLOW                (0x3FFF000CL) /* 3FFF000C,  1073676300 */
#define VI_WARN_CONFIG_NLOADED                (0x3FFF0077L) /* 3FFF0077,  1073676407 */
#define VI_WARN_NULL_OBJECT                   (0x3FFF0082L) /* 3FFF0082,  1073676418 */
#define VI_WARN_NSUP_ATTR_STATE               (0x3FFF0084L) /* 3FFF0084,  1073676420 */
#define VI_WARN_UNKNOWN_STATUS                (0x3FFF0085L) /* 3FFF0085,  1073676421 */
#define VI_WARN_NSUP_BUF                      (0x3FFF0088L) /* 3FFF0088,  1073676424 */
#define VI_WARN_EXT_FUNC_NIMPL                (0x3FFF00A9L) /* 3FFF00A9,  1073676457 */

#define VI_ERROR_SYSTEM_ERROR       (_VI_ERROR+0x3FFF0000L) /* BFFF0000, -1073807360 */
#define VI_ERROR_INV_OBJECT         (_VI_ERROR+0x3FFF000EL) /* BFFF000E, -1073807346 */
#define VI_ERROR_RSRC_LOCKED        (_VI_ERROR+0x3FFF000FL) /* BFFF000F, -1073807345 */
#define VI_ERROR_INV_EXPR           (_VI_ERROR+0x3FFF0010L) /* BFFF0010, -1073807344 */
#define VI_ERROR_RSRC_NFOUND        (_VI_ERROR+0x3FFF0011L) /* BFFF0011, -1073807343 */
#define VI_ERROR_INV_RSRC_NAME      (_VI_ERROR+0x3FFF0012L) /* BFFF0012, -1073807342 */
#define VI_ERROR_INV_ACC_MODE       (_VI_ERROR+0x3FFF0013L) /* BFFF0013, -1073807341 */
#define VI_ERROR_TMO                (_VI_ERROR+0x3FFF0015L) /* BFFF0015, -1073807339 */
#define VI_ERROR_CLOSING_FAILED     (_VI_ERROR+0x3FFF0016L) /* BFFF0016, -1073807338 */
#define VI_ERROR_INV_DEGREE         (_VI_ERROR+0x3FFF001BL) /* BFFF001B, -1073807333 */
#define VI_ERROR_INV_JOB_ID         (_VI_ERROR+0x3FFF001CL) /* BFFF001C, -1073807332 */
#define VI_ERROR_NSUP_ATTR          (_VI_ERROR+0x3FFF001DL) /* BFFF001D, -1073807331 */
#define VI_ERROR_NSUP_ATTR_STATE    (_VI_ERROR+0x3FFF001EL) /* BFFF001E, -1073807330 */
#define VI_ERROR_ATTR_READONLY      (_VI_ERROR+0x3FFF001FL) /* BFFF001F, -1073807329 */
#define VI_ERROR_INV_LOCK_TYPE      (_VI_ERROR+0x3FFF0020L) /* BFFF0020, -1073807328 */
#define VI_ERROR_INV_ACCESS_KEY     (_VI_ERROR+0x3FFF0021L) /* BFFF0021, -1073807327 */
#define VI_ERROR_INV_EVENT          (_VI_ERROR+0x3FFF0026L) /* BFFF0026, -1073807322 */
#define VI_ERROR_INV_MECH           (_VI_ERROR+0x3FFF0027L) /* BFFF0027, -1073807321 */
#define VI_ERROR_HNDLR_NINSTALLED   (_VI_ERROR+0x3FFF0028L) /* BFFF0028, -1073807320 */
#define VI_ERROR_INV_HNDLR_REF      (_VI_ERROR+0x3FFF0029L) /* BFFF0029, -1073807319 */
#define VI_ERROR_INV_CONTEXT        (_VI_ERROR+0x3FFF002AL) /* BFFF002A, -1073807318 */
#define VI_ERROR_QUEUE_OVERFLOW     (_VI_ERROR+0x3FFF002DL) /* BFFF002D, -1073807315 */
#define VI_ERROR_NENABLED           (_VI_ERROR+0x3FFF002FL) /* BFFF002F, -1073807313 */
#define VI_ERROR_ABORT              (_VI_ERROR+0x3FFF0030L) /* BFFF0030, -1073807312 */
#define VI_ERROR_RAW_WR_PROT_VIOL   (_VI_ERROR+0x3FFF0034L) /* BFFF0034, -1073807308 */
#define VI_ERROR_RAW_RD_PROT_VIOL   (_VI_ERROR+0x3FFF0035L) /* BFFF0035, -1073807307 */
#define VI_ERROR_OUTP_PROT_VIOL     (_VI_ERROR+0x3FFF0036L) /* BFFF0036, -1073807306 */
#define VI_ERROR_INP_PROT_VIOL      (_VI_ERROR+0x3FFF0037L) /* BFFF0037, -1073807305 */
#define VI_ERROR_BERR               (_VI_ERROR+0x3FFF0038L) /* BFFF0038, -1073807304 */
#define VI_ERROR_IN_PROGRESS        (_VI_ERROR+0x3FFF0039L) /* BFFF0039, -1073807303 */
#define VI_ERROR_INV_SETUP          (_VI_ERROR+0x3FFF003AL) /* BFFF003A, -1073807302 */
#define VI_ERROR_QUEUE_ERROR        (_VI_ERROR+0x3FFF003BL) /* BFFF003B, -1073807301 */
#define VI_ERROR_ALLOC              (_VI_ERROR+0x3FFF003CL) /* BFFF003C, -1073807300 */
#define VI_ERROR_INV_MASK           (_VI_ERROR+0x3FFF003DL) /* BFFF003D, -1073807299 */
#define VI_ERROR_IO                 (_VI_ERROR+0x3FFF003EL) /* BFFF003E, -1073807298 */
#define VI_ERROR_INV_FMT            (_VI_ERROR+0x3FFF003FL) /* BFFF003F, -1073807297 */
#define VI_ERROR_NSUP_FMT           (_VI_ERROR+0x3FFF0041L) /* BFFF0041, -1073807295 */
#define VI_ERROR_LINE_IN_USE        (_VI_ERROR+0x3FFF0042L) /* BFFF0042, -1073807294 */
#define VI_ERROR_LINE_NRESERVED     (_VI_ERROR+0x3FFF0043L) /* BFFF0043, -1073807293 */
#define VI_ERROR_NSUP_MODE          (_VI_ERROR+0x3FFF0046L) /* BFFF0046, -1073807290 */
#define VI_ERROR_SRQ_NOCCURRED      (_VI_ERROR+0x3FFF004AL) /* BFFF004A, -1073807286 */
#define VI_ERROR_INV_SPACE          (_VI_ERROR+0x3FFF004EL) /* BFFF004E, -1073807282 */
#define VI_ERROR_INV_OFFSET         (_VI_ERROR+0x3FFF0051L) /* BFFF0051, -1073807279 */
#define VI_ERROR_INV_WIDTH          (_VI_ERROR+0x3FFF0052L) /* BFFF0052, -1073807278 */
#define VI_ERROR_NSUP_OFFSET        (_VI_ERROR+0x3FFF0054L) /* BFFF0054, -1073807276 */
#define VI_ERROR_NSUP_VAR_WIDTH     (_VI_ERROR+0x3FFF0055L) /* BFFF0055, -1073807275 */
#define VI_ERROR_WINDOW_NMAPPED     (_VI_ERROR+0x3FFF0057L) /* BFFF0057, -1073807273 */
#define VI_ERROR_RESP_PENDING       (_VI_ERROR+0x3FFF0059L) /* BFFF0059, -1073807271 */
#define VI_ERROR_NLISTENERS         (_VI_ERROR+0x3FFF005FL) /* BFFF005F, -1073807265 */
#define VI_ERROR_NCIC               (_VI_ERROR+0x3FFF0060L) /* BFFF0060, -1073807264 */
#define VI_ERROR_NSYS_CNTLR         (_VI_ERROR+0x3FFF0061L) /* BFFF0061, -1073807263 */
#define VI_ERROR_NSUP_OPER          (_VI_ERROR+0x3FFF0067L) /* BFFF0067, -1073807257  */
#define VI_ERROR_INTR_PENDING       (_VI_ERROR+0x3FFF0068L) /* BFFF0068, -1073807256 */
#define VI_ERROR_ASRL_PARITY        (_VI_ERROR+0x3FFF006AL) /* BFFF006A, -1073807254 */
#define VI_ERROR_ASRL_FRAMING       (_VI_ERROR+0x3FFF006BL) /* BFFF006B, -1073807253 */
#define VI_ERROR_ASRL_OVERRUN       (_VI_ERROR+0x3FFF006CL) /* BFFF006C, -1073807252 */
#define VI_ERROR_TRIG_NMAPPED       (_VI_ERROR+0x3FFF006EL) /* BFFF006E, -1073807250 */
#define VI_ERROR_NSUP_ALIGN_OFFSET  (_VI_ERROR+0x3FFF0070L) /* BFFF0070, -1073807248 */
#define VI_ERROR_USER_BUF           (_VI_ERROR+0x3FFF0071L) /* BFFF0071, -1073807247 */
#define VI_ERROR_RSRC_BUSY          (_VI_ERROR+0x3FFF0072L) /* BFFF0072, -1073807246 */
#define VI_ERROR_NSUP_WIDTH         (_VI_ERROR+0x3FFF0076L) /* BFFF0076, -1073807242 */
#define VI_ERROR_INV_PARAMETER      (_VI_ERROR+0x3FFF0078L) /* BFFF0078, -1073807240 */
#define VI_ERROR_INV_PROT           (_VI_ERROR+0x3FFF0079L) /* BFFF0079, -1073807239 */
#define VI_ERROR_INV_SIZE           (_VI_ERROR+0x3FFF007BL) /* BFFF007B, -1073807237 */
#define VI_ERROR_WINDOW_MAPPED      (_VI_ERROR+0x3FFF0080L) /* BFFF0080, -1073807232 */
#define VI_ERROR_NIMPL_OPER         (_VI_ERROR+0x3FFF0081L) /* BFFF0081, -1073807231  */
#define VI_ERROR_INV_LENGTH         (_VI_ERROR+0x3FFF0083L) /* BFFF0083, -1073807229 */
#define VI_ERROR_INV_MODE           (_VI_ERROR+0x3FFF0091L) /* BFFF0091, -1073807215 */
#define VI_ERROR_SESN_NLOCKED       (_VI_ERROR+0x3FFF009CL) /* BFFF009C, -1073807204 */
#define VI_ERROR_MEM_NSHARED        (_VI_ERROR+0x3FFF009DL) /* BFFF009D, -1073807203 */
#define VI_ERROR_LIBRARY_NFOUND     (_VI_ERROR+0x3FFF009EL) /* BFFF009E, -1073807202 */
#define VI_ERROR_NSUP_INTR          (_VI_ERROR+0x3FFF009FL) /* BFFF009F, -1073807201 */
#define VI_ERROR_INV_LINE           (_VI_ERROR+0x3FFF00A0L) /* BFFF00A0, -1073807200 */
#define VI_ERROR_FILE_ACCESS        (_VI_ERROR+0x3FFF00A1L) /* BFFF00A1, -1073807199 */
#define VI_ERROR_FILE_IO            (_VI_ERROR+0x3FFF00A2L) /* BFFF00A2, -1073807198 */
#define VI_ERROR_NSUP_LINE          (_VI_ERROR+0x3FFF00A3L) /* BFFF00A3, -1073807197 */
#define VI_ERROR_NSUP_MECH          (_VI_ERROR+0x3FFF00A4L) /* BFFF00A4, -1073807196 */
#define VI_ERROR_INTF_NUM_NCONFIG   (_VI_ERROR+0x3FFF00A5L) /* BFFF00A5, -1073807195 */
#define VI_ERROR_CONN_LOST          (_VI_ERROR+0x3FFF00A6L) /* BFFF00A6, -1073807194 */
#define VI_ERROR_MACHINE_NAVAIL     (_VI_ERROR+0x3FFF00A7L) /* BFFF00A7, -1073807193 */
#define VI_ERROR_NPERMISSION        (_VI_ERROR+0x3FFF00A8L) /* BFFF00A8, -1073807192 */

//==============================================================================
// Types

#endif  /* ndef __VISA_HEADER__ */

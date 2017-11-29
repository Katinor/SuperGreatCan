#ifndef ITF_UBISHELL_H_
#define ITF_UBISHELL_H_

#ifdef	__cplusplus
extern "C" {
#endif

/**
 * @file ubishell.h
 *
 * @brief ubishell (Ubinos Shell) 컴포넌트 인터페이스
 */

/** 쉘 후크 함수 포인터  형 정의 */
typedef int (* ubishell_hookfunc_ft)(char * str, int max, void * arg);

/**
 * ubishell 컴포넌트를 초기화하는 함수
 *
 * 이 함수는 시스템 시작시  bsp 컴포넌트의 main 함수에서 자동적으로 호출됩니다.
 *
 * @return	  0: 성공<br>
 * 			<br>
 * 			 -1: 오류<br>
 */
int ubishell_comp_init(void);

/**
 * ubishell을 시작하는 함수
 *
 * @return	  0: 성공<br>
 * 			<br>
 * 			 -1: 오류<br>
 */
int ubishell_start(void);

/**
 * 후크 함수를 등록하는 함수
 *
 * 사용자는 후크 함수를 이용해 쉘에 원하는 기능을 추가할 수 있습니다.
 *
 * @param	후크 함수 포인터<br>
 *
 * @param	후크 함수가 호출될 때 전달할 인수<br>
 *
 * @return	  0: 성공<br>
 * 			<br>
 * 			 -1: 오류<br>
 */
int ubishell_sethookfunc(ubishell_hookfunc_ft hookfunc, void * arg);

/**
 * 쉘 메인 태스크 함수
 *
 * 쉘을 태스크로 실행할 경우 이 함수를 태스크 함수로 사용합니다.
 *
 * @param	사용자 정의 인수<br>
 *
 * @return	  0: 성공<br>
 * 			<br>
 * 			 -1: 오류<br>
 */
void ubishell_main_task(void * arg);

/**
 * 쉘 아이들 태스크 후크 함수
 *
 * 쉘을 아이들 태스크 후크 기능을 사용해 실행할 경우 이 함수를 아이들 태스크 후크 함수로 사용합니다.
 *
 * @param	사용자 정의 인수<br>
 *
 * @return	  0: 성공<br>
 * 			<br>
 * 			 -1: 오류<br>
 */
int ubishell_main_idletaskhook(void * arg);

#ifdef	__cplusplus
}
#endif

#endif /* ITF_UBISHELL_H_ */

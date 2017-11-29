/*
  Copyright (C) 2009 Sung Ho Park
  Contact: ubinos.org@gmail.com

  This file is part of the itf_ubinos component of the Ubinos.

  GNU General Public License Usage
  This file may be used under the terms of the GNU
  General Public License version 3.0 as published by the Free Software
  Foundation and appearing in the file license_gpl3.txt included in the
  packaging of this file. Please review the following information to
  ensure the GNU General Public License version 3.0 requirements will be
  met: http://www.gnu.org/copyleft/gpl.html.

  GNU Lesser General Public License Usage
  Alternatively, this file may be used under the terms of the GNU Lesser
  General Public License version 2.1 as published by the Free Software
  Foundation and appearing in the file license_lgpl.txt included in the
  packaging of this file. Please review the following information to
  ensure the GNU Lesser General Public License version 2.1 requirements
  will be met: http://www.gnu.org/licenses/old-licenses/lgpl-2.1.html.

  Commercial Usage
  Alternatively, licensees holding valid commercial licenses may
  use this file in accordance with the commercial license agreement
  provided with the software or, alternatively, in accordance with the
  terms contained in a written agreement between you and rightful owner.
*/

#ifndef ITF_UBIK_TASK_H_
#define ITF_UBIK_TASK_H_

#ifdef	__cplusplus
extern "C" {
#endif

/**
 * @file ubik_task.h
 *
 * @brief ubik (Ubinos Kernel) ������Ʈ �½�ũ �������̽�
 */

/**
 * @example	tasktest00.c
 * @example	tasktest01.c
 * @example	tasktest02.c
 * @example	tasktest03.c
 * @example	tasktest04.c
 * @example	tasktest05.c
 * @example	tasktest06.c
 * @example	tasktest07.c
 * @example	tasktest08.c
 * @example	tasktest09.c
 */

/**
 * \page task_example �½�ũ ����
 *
 *         \section tasktest00_sec ������ �½�ũ ��� ����
 *             Simple task example
 *
 *             - <a href="tasktest00_8c-example.html">tasktest00.c</a>
 *
 *             �� ������ ������ �޽����� �ݺ������� ����ϴ� �½�ũ�� �ϳ� �����Ѵ�.<br>
 *
 *         <br>
 *
 *         \section tasktest01_sec �½�ũ�� �⺻ ��� ����
 *             Test on basic functions of task
 *
 *             - <a href="tasktest01_8c-example.html">tasktest01.c</a>
 *
 *             �� ������ �Ʒ��� ������ �½�ũ�� �⺻ ����� �����Ѵ�.
 * <pre>
 *     �½�ũ�� �����ϴ� ���
 *     �½�ũ�� ����� ������ ��ٸ��� ���
 * </pre>
 *
 *         <br>
 *
 *         \section tasktest02_sec �½�ũ�� �޸�sleep ��� ����
 *             Test on sleep function of task
 *
 *             - <a href="tasktest02_8c-example.html">tasktest02.c</a>
 *
 *             �� ������ �½�ũ�� �޸�sleep ����� �����Ѵ�.<br>
 *             ���� ������ ������ ����.<br>
 *
 *             ���� 1���� �½�ũ 1�� �����Ѵ�.<br>
 *             ������� ���������� ����Ǿ��ٸ� Ȯ�� ���� 1���� �½�ũ 1�� ���� ���̾�� �Ѵ�.<br>
 *
 *             ���� 2���� �½�ũ 1�� ���� 3���� �޸�sleep�� �õ��Ѵ�.<br>
 *             ������� ���������� ����Ǿ��ٸ� Ȯ�� ���� 2���� �½�ũ 1�� ���� ���� �ƴϾ��(���block ���¿���) �Ѵ�.<br>
 *
 *             ���� 3�̵Ǹ� �½�ũ 1�� �޸�sleep �ð��� ���ؼ� �����.<br>
 *             ������� ���������� ����Ǿ��ٸ� Ȯ�� ���� 3���� �½�ũ 1�� ���� ���̾�� �Ѵ�.<br>
 *
 *             ���� 4���� �½�ũ 1�� ����ȴ�.<br>
 *
 *             \image html ubik_test_tasktest02.gif
 *
 *         <br>
 *
 *         \section tasktest03_sec �½�ũ�� ����suspend �� �����resume ��� ����
 *             Test on suspend and resume function of task
 *
 *             - <a href="tasktest03_8c-example.html">tasktest03.c</a>
 *
 *             �� ������ �½�ũ�� ����suspend �� �����resume ����� �����Ѵ�.<br>
 *             ���� ������ ������ ����.<br>
 *
 *             ���� 1���� �½�ũ 1�� �����Ѵ�.<br>
 *             ������� ���������� ����Ǿ��ٸ� Ȯ�� ���� 1���� �½�ũ 1�� ���� ���̾�� �Ѵ�.<br>
 *
 *             ���� 2���� �½�ũ 1�� ����suspend�Ѵ�. <br>
 *             ������� ���������� ����Ǿ��ٸ� Ȯ�� ���� 2���� �½�ũ 1�� ���� ���� �ƴϾ��(����suspended ���¿���) �Ѵ�.<br>
 *
 *             ���� 3���� �½�ũ 1�� �����resume�Ѵ�. <br>
 *             ������� ���������� ����Ǿ��ٸ� Ȯ�� ���� 3���� �½�ũ 1�� ���� ���̾�� �Ѵ�.<br>
 *
 *             ���� 4���� �½�ũ 1�� ������ ����suspend�Ѵ�. <br>
 *             ������� ���������� ����Ǿ��ٸ� Ȯ�� ���� 4���� �½�ũ 1�� ���� ���� �ƴϾ��(����suspended ���¿���) �Ѵ�.<br>
 *
 *             ���� 5���� �½�ũ 1�� �����resume�Ѵ�. <br>
 *             ������� ���������� ����Ǿ��ٸ� Ȯ�� ���� 5���� �½�ũ 1�� ���� ���̾�� �Ѵ�.<br>
 *
 *             ���� 6���� �½�ũ 1�� ����ȴ�.<br>
 *
 *             \image html ubik_test_tasktest03.gif
 *
 *         <br>
 *
 *         \section tasktest04_sec �켱���� ��� ��Ƽ�½�ŷ ��� ����
 *             Test on priority-based multi-tasking
 *
 *             - <a href="tasktest04_8c-example.html">tasktest04.c</a>
 *
 *             �� ������ �켱���� ��� ��Ƽ�½�ŷ ����� �����Ѵ�.<br>
 *             ���� ������ ������ ����.<br>
 *
 *             ���� 1���� ���� �켱������ �½�ũ 4�� �����Ѵ�.<br>
 *             ������� ���������� ����Ǿ��ٸ� Ȯ�� ���� 1���� �½�ũ 4�� ���� ���̾�� �Ѵ�.<br>
 *
 *             ���� 2���� ���� �켱������ �½�ũ 1�� �����Ѵ�.<br>
 *             �½�ũ 1�� �켱������ �½�ũ 4�� �켱���� ���� ����.<br>
 *             ���� ������� ���������� ����Ǿ��ٸ� Ȯ�� ���� 2���� �½�ũ 1�� ���� ���̾�� �Ѵ�.<br>
 *
 *             ���� 3���� �߰� �켱������ �½�ũ 2�� 3�� �����Ѵ�.<br>
 *             �½�ũ 1�� �켱������ �½�ũ 2�� 3�� �켱���� ���� ����.<br>
 *             ���� ������� ���������� ����Ǿ��ٸ� Ȯ�� ���� 3���� �½�ũ 1�� ���� ���̾�� �Ѵ�.<br>
 *
 *             ���� 4���� �½�ũ 1�� ����ȴ�.<br>
 *             �׷��� �½�ũ 2�� 3�� ���� ���� �켱������ ���� �½�ũ�� �ȴ�.<br>
 *             ���� ������� ���������� ����Ǿ��ٸ�  Ȯ�� ���� 4���� �½�ũ 2�� 3�� ���� ���̾�� �Ѵ�.<br>
 *             �½�ũ 2�� 3�� ���� �켱������ �����Ƿ� ���� �κ� ������� ���ÿ� ����ȴ�.<br>
 *
 *             ���� 5���� �½�ũ 2�� 3�� ����ȴ�.<br>
 *
 *             ���� 6���� �½�ũ 4�� ����ȴ�.<br>
 *
 *             \image html ubik_test_tasktest04.gif
 *
 *         <br>
 *
 *         \section tasktest05_sec �½�ũ�� �켱���� ���� ��� ����
 *             Test on set priority function of task
 *
 *             - <a href="tasktest05_8c-example.html">tasktest05.c</a>
 *
 *             �� ������ �½�ũ�� �켱���� ���� ����� �����Ѵ�.<br>
 *             ���� ������ ������ ����.<br>
 *
 *             ���� 1���� ���� �켱������ �½�ũ 2�� �����Ѵ�.<br>
 *             ������� ���������� ����Ǿ��ٸ� Ȯ�� ���� 1���� �½�ũ 2�� ���� ���̾�� �Ѵ�.<br>
 *
 *             ���� 2���� �߰� �켱������ �½�ũ 1�� �����Ѵ�.<br>
 *             �½�ũ 1�� �켱������ �½�ũ 2�� �켱�������� ����.<br>
 *             ���� ������� ���������� ����Ǿ��ٸ� Ȯ�� ���� 2���� �½�ũ 1�� ���� ���̾�� �Ѵ�.<br>
 *
 *             ���� 3���� �½�ũ 2�� �켱������ ���� �켱������ �����Ѵ�.<br>
 *             �׷��� �½�ũ 2�� �켱������ �½�ũ 1�� �켱�������� ��������.<br>
 *             ���� ������� ���������� ����Ǿ��ٸ� Ȯ�� ���� 3���� �½�ũ 2�� ���� ���̾�� �Ѵ�.<br>
 *
 *             ���� 4���� �½�ũ 2�� �켱������ ���� �켱������ �����Ѵ�.<br>
 *             �׷��� �½�ũ 1�� �켱������ �½�ũ 2�� �켱�������� ��������.<br>
 *             ���� ������� ���������� ����Ǿ��ٸ� Ȯ�� ���� 4���� �½�ũ 1�� ���� ���̾�� �Ѵ�.<br>
 *
 *             ���� 5���� �½�ũ 1�� ����ȴ�.<br>
 *
 *             ���� 6���� �½�ũ 2�� ����ȴ�.<br>
 *
 *             \image html ubik_test_tasktest05.gif
 *
 *         <br>
 *
 *         \section tasktest06_sec �½�ũ�� ���� �޸� ���� ����
 *             Test for memory leak by task
 *
 *             - <a href="tasktest06_8c-example.html">tasktest06.c</a>
 *
 *             �� ������ �½�ũ�� ���� �޸� ������ �����Ѵ�.<br>
 *             ���� ������ ������ ����.<br>
 *
 *             ���� ��heap�� �Ҵ�� �޸� �ѷ��� �޸� ��� ���� ������ �����Ѵ�.<br>
 *             �׸��� �½�ũ ������ ���Ÿ� ���� �� �ݺ��Ѵ�.<br>
 *             �� ���� ��heap�� �Ҵ�� �޸� �ѷ��� �޸� �� ���� �ٽ� ������ �����ߴ� ���� ���Ѵ�.<br>
 *             �޸� ������ �߻����� �ʾҴٸ� �����ߴ� ���� �ٽ� ������ ���� ���ƾ� �Ѵ�.<br>
 *
 *         <br>
 *
 *         \section tasktest07_sec �½�ũ�� �ټ� ��ȣ ��ü ��ٸ��� ��� ����
 *             Test on wait for multiple signal objects function of task
 *
 *             - <a href="tasktest07_8c-example.html">tasktest07.c</a>
 *
 *             �� ������ �½�ũ�� �ټ� ��ȣ ��ü ��ٸ��� ����� �����Ѵ�.<br>
 *             �½�ũ�� �ټ� ��ȣ ��ü ��ٸ��� ����� ��������, ���ؽ�, �޽���ť, �ñ׳�, ���Ǻ���condition variable,
 *             ����¹��� �� �پ��� ������ ��ȣ ��ü ���� ���κ��� ��ȣ�� ���ÿ� ��ٷ��� �� ��� ����Ѵ�.<br>
 *             �� ����� ����� �ټ� ��ȣ ��ü�� ��ٸ��� �½�ũ�� ��ٸ��� ���� �������� ���� �ΰ��� �� �ϳ��� ������ �� �ִ�.<br>
 *             ù ��°�� ��ٸ��� ��ȣ ��ü�� �� �ϳ� �̻����κ��� ��ȣ�� �޴� ���̰�,
 *             �� ��°�� ��ٸ��� ��ȣ ��ü�� ��ηκ��� ��ȣ�� �޴� ���̴�.<br>
 *             ��� ��ٸ��� ���� ����(TASK_WAITOPT__ALL)�� �����ϰ� �� ����� ����ϸ� �� ��°��, �׷��� ������ ù ��°�� ��ٸ��� ���� ������ �ȴ�.<br>
 *             ���� ������ ������ ����.<br>
 *
 *             ���� 3���� �½�ũ 1�� ��� ��ٸ��� ���� ����(TASK_WAITOPT__ALL)�� �������� �ʰ� �ټ� ��ȣ ��ü ��ٸ��⸦ �õ��Ѵ�.<br>
 *             ������� ���������� ����Ǿ��ٸ�  Ȯ�� ���� 3���� �½�ũ 2�� ���� ���̾�� �Ѵ�.<br>
 *
 *             ���� 4���� �½�ũ 2�� �½�ũ 1�� ��ٸ��� �ִ� ��ȣ ��ü�� �� �ϳ����� ��ȣ�� ������.<br>
 *             �׷���  �½�ũ 1�� �� ��ȣ�� �ް� �����.<br>
 *             �̴� �½�ũ 1�� ��� ��ٸ��� ���� ����(TASK_WAITOPT__ALL)�� �������� �ʰ� �ټ� ��ȣ ��ü�� ��ٸ��� �ֱ� �����̴�.<br>
 *             ������� ���������� ����Ǿ��ٸ�  Ȯ�� ���� 4���� �½�ũ 1�� ���� ���̾�� �Ѵ�.<br>
 *
 *             ���� 5���� �½�ũ 1�� ��� ��ٸ��� ���� ����(TASK_WAITOPT__ALL)�� �����ϰ� �ټ� ��ȣ ��ü ��ٸ��⸦ �õ��Ѵ�.<br>
 *             ������� ���������� ����Ǿ��ٸ�  Ȯ�� ���� 5���� �½�ũ 2�� ���� ���̾�� �Ѵ�.<br>
 *
 *             ���� 6���� �½�ũ 2�� �½�ũ 1�� ��ٸ��� �ִ� ��ȣ ��ü�� �� �ϳ����� ��ȣ�� ������.<br>
 *             �׷��� �½�ũ 1�� �� ��ȣ�� �޴´�. �׷��� �½�ũ 1�� ������� �ʴ´�.<br>
 *             �̴� �½�ũ 1�� ��� ��ٸ��� ���� ����(TASK_WAITOPT__ALL)�� �����ϰ� �ټ� ��ȣ ��ü�� ��ٸ��� �ֱ� �����̴�.<br>
 *             ������� ���������� ����Ǿ��ٸ� Ȯ�� ���� 6���� �½�ũ 2�� ���� ���̾�� �Ѵ�.<br>
 *
 *             ���� 7���� �½�ũ 2�� �½�ũ 1�� ��ٸ��� �ִ� ������ ��� ��ȣ ��ü�鿡�� ��ȣ�� ������.<br>
 *             �׷��� �½�ũ 1�� �� ��ȣ���� �ް� �����.<br>
 *             ������� ���������� ����Ǿ��ٸ� Ȯ�� ���� 7���� �½�ũ 1�� ���� ���̾�� �Ѵ�.<br>
 *
 *             ���� 10���� �½�ũ 1�� ��� ��ٸ��� ���� ����(TASK_WAITOPT__ALL)�� �����ϰ�
 *             ���� 12������ ���� �ð����� �ΰ� �ټ� ��ȣ ��ü ��ٸ��⸦ �õ��Ѵ�.<br>
 *             ������� ���������� ����Ǿ��ٸ�  Ȯ�� ���� 10���� �½�ũ 2�� ���� ���̾�� �Ѵ�.<br>
 *
 *             ���� 11���� �½�ũ 2�� �½�ũ 1�� ��ٸ��� �ִ� ��ȣ ��ü�� �� �ϳ����� ��ȣ�� ������.<br>
 *             �׷��� �½�ũ 1�� �� ��ȣ�� �޴´�. �׷��� �½�ũ 1�� ������� �ʴ´�.<br>
 *             �̴� �½�ũ 1�� ��� ��ٸ��� ���� ����(TASK_WAITOPT__ALL)�� �����ϰ� �ټ� ��ȣ ��ü�� ��ٸ��� �ֱ� �����̴�.<br>
 *             ������� ���������� ����Ǿ��ٸ� Ȯ�� ���� 11���� �½�ũ 2�� ���� ���̾�� �Ѵ�.<br>
 *
 *             ���� 12�� �Ǹ� �½�ũ 1�� ���� �ð��� ���ؼ� ��ٸ��� ��� ��ȣ ��ü��κ��� ��ȣ�� ���� ���ߴ��� �����.<br>
 *             ������� ���������� ����Ǿ��ٸ� Ȯ�� ���� 12���� �½�ũ 1�� ���� ���̾�� �Ѵ�.<br>
 *
 *             \image html ubik_test_tasktest07.gif
 *
 *         <br>
 *
 *         \section tasktest08_sec �½�ũ�� ���� ���� �ð� �������� ��� ����
 *             Test on get remaining timeout tick function of task
 *
 *             - <a href="tasktest08_8c-example.html">tasktest08.c</a>
 *
 *             �� ������ ���� ���� �ð��� �������� ����� �����Ѵ�.<br>
 *             ���� �ð��� �ΰ� ��ȣ ��ü�� ��ٸ��� �½�ũ�� ���� �ð��� ���ϱ� ���� ��ȣ�� �ް� ����� ��,
 *             �� ����� ����ϸ� ���� ���� �ð��� �˾Ƴ� �� �ִ�.<br>
 *             �� ����� �� ���������� ������ ��ȣ ���� ���� ���������� �޾� ó���ϴ� �۾��� ���� �ð��� �ξ�� �� ��� �����ϴ�.<br>
 *             ���� ������ ������ ����.<br>
 *
 *             ���� 1���� �߰� �켱������ �½�ũ 2�� �����Ѵ�.<br>
 *             ������� ���������� ����Ǿ��ٸ� Ȯ�� ���� 1���� �½�ũ 2�� ���� ���̾�� �Ѵ�.<br>
 *
 *             ���� 2���� ���� �켱������ �½�ũ 1�� �����Ѵ�.<br>
 *             ������� ���������� ����Ǿ��ٸ� Ȯ�� ���� 2���� �½�ũ 1�� ���� ���̾�� �Ѵ�.<br>
 *
 *             ���� 3���� �½�ũ 1�� ���� 6������ ���� �ð����� �ΰ� ù ��° ��ȣ�� ��ٸ���.<br>
 *             ������� ���������� ����Ǿ��ٸ� Ȯ�� ���� 3���� �½�ũ 2�� ���� ���̾�� �Ѵ�.<br>
 *
 *             ���� 4���� �½�ũ 2��  ù ��° ��ȣ�� ������.<br>
 *             �׷��� �½�ũ 1�� �� ��ȣ�� �ް� �����.<br>
 *             �׸��� ��� �½�ũ 1�� ���� ���� �ð��� �����´�.<br>
 *             ������� ���������� ����Ǿ��ٸ� Ȯ�� ���� 4���� �½�ũ 1�� ���� ���̾�� �Ѵ�.<br>
 *
 *             ���� 5���� �½�ũ 1�� ������ ���� ���� �ð��� �̹� �˰� �ִ� ���� 4�� 5 ������ �۾� �ð��� ������ ���� 6���� ���� �ð��� ����Ѵ�.<br>
 *             �׸��� ����� ���� �ð��� Ȱ���� �ٽ� ���� 6������ ���� �ð����� �ΰ� �� ��° ��ȣ�� ��ٸ���.<br>
 *             ������� ���������� ����Ǿ��ٸ� Ȯ�� ���� 5���� �½�ũ 2�� ���� ���̾�� �Ѵ�.<br>
 *
 *             ���� 6�̵Ǹ� �½�ũ 1�� ���� �ð��� ���ؼ� ��ȣ�� ���� ���ߴ���  �����.<br>
 *             ������� ���������� ����Ǿ��ٸ� Ȯ�� ���� 6���� �½�ũ 1�� ���� ���̾�� �Ѵ�.<br>
 *
 *             ���� 7���� �½�ũ 1�� ����ȴ�.<br>
 *
 *             ���� 8���� �½�ũ 2�� ����ȴ�.<br>
 *
 *             \image html ubik_test_tasktest08.gif
 *
 *         <br>
 *
 *         \section tasktest09_sec �½�ũ ����suspend �� �����resume ��� ���� 2
 *             Test 2 on suspend and resume function of task
 *
 *             - <a href="tasktest09_8c-example.html">tasktest09.c</a>
 *
 *             �� ������ �½�ũ�� ����suspend�ߴٰ� �����resume���� ��, �½�ũ ���°� ����suspend�ϱ� ���� ���·� ���ư������� �����Ѵ�.<br>
 *             ���� ������ ������ ����.<br>
 *
 *             ���� 1���� �߰� �켱������ �½�ũ 2�� �����Ѵ�.<br>
 *             ������� ���������� ����Ǿ��ٸ� Ȯ�� ���� 1���� �½�ũ 2�� ���� ���̾�� �Ѵ�.<br>
 *
 *             ���� 2���� ���� �켱������ �½�ũ 1�� �����Ѵ�.<br>
 *             ������� ���������� ����Ǿ��ٸ� Ȯ�� ���� 2���� �½�ũ 1�� ���� ���̾�� �Ѵ�.<br>
 *
 *             ���� 3���� �½�ũ 1�� ��ȣ�� ��ٸ���(���blocked ���°� �ȴ�).<br>
 *             ������� ���������� ����Ǿ��ٸ� Ȯ�� ���� 3���� �½�ũ 2�� ���� ���̾�� �Ѵ�.<br>
 *
 *             ���� 4���� �½�ũ 2�� �½�ũ 1�� ����suspend�� �� ��ȣ�� ������.<br>
 *             ����suspend�� �½�ũ�� ��ȣ�� ���� ���Ѵ�.<br>
 *             ���� ������� ���������� ����Ǿ��ٸ� Ȯ�� ���� 4���� �½�ũ 2�� ���� ���̾�� �Ѵ�.<br>
 *
 *             ���� 5���� �½�ũ 2�� �½�ũ 1�� �����resume�Ѵ�.<br>
 *             �����resume�� �½�ũ 1�� ���� 4���� ������ ��ȣ�� �ް� �����(�غ�ready ���°� �ȴ�).<br>
 *             ���� ������� ���������� ����Ǿ��ٸ� Ȯ�� ���� 5���� �½�ũ 1�� ���� ���̾�� �Ѵ�.<br>
 *
 *             ���� 6���� �½�ũ 1�� ���� 8������ ���� �ð����� �ΰ� ��ȣ�� ��ٸ���(���blocked ���°� �ȴ�).<br>
 *             ������� ���������� ����Ǿ��ٸ� Ȯ�� ���� 6���� �½�ũ 2�� ���� ���̾�� �Ѵ�.<br>
 *
 *             ���� 7���� �½�ũ 2�� �½�ũ 1�� ����suspend�Ѵ�.<br>
 *             ������� ���������� ����Ǿ��ٸ� Ȯ�� ���� 7���� �½�ũ 2�� ���� ���̾�� �Ѵ�.<br>
 *
 *             ���� 8���� �½�ũ 2�� �½�ũ 1�� �����resume�Ѵ�.<br>
 *             �����resume�� �½�ũ 1��  ����suspend�Ǳ� ���� ������ ���blocked ���°� �ȴ�.<br>
 *             �׸��� �½�ũ 1�� �� ���������� ���� �ð����� �ΰ� ��ȣ�� ��ٷ�����
 *             ����suspend ���¿� �ӹ��� �ð��� ������ �ð����� �������� �ʱ� ������ ����� �ʴ´�.<br>
 *             ���� �ð��� ���ϴ� ������ ����suspend ���¿� �ӹ��� �ð��� ���� ���� 9�� �ȴ�.<br>
 *             ���� ������� ���������� ����Ǿ��ٸ� Ȯ�� ���� 8���� �½�ũ 2�� ���� ���̾�� �Ѵ�.<br>
 *
 *             ���� 9�� �Ǹ� �½�ũ 1�� ���� �ð��� ���ؼ� ��ȣ�� ���� ���ߴ��� �����.<br>
 *             ������� ���������� ����Ǿ��ٸ� Ȯ�� ���� 9���� �½�ũ 1�� ���� ���̾�� �Ѵ�.<br>
 *
 *             ���� 10���� �½�ũ 1�� ����ȴ�.<br>
 *
 *             ���� 11���� �½�ũ 2�� ����ȴ�.<br>
 *
 *             \image html ubik_test_tasktest09.gif
 *
 * <br>
 *
 * Content is available under Attribution-Noncommercial-No Derivative Works 3.0 Unported.
 */

#include "type.h"

/** ��ٸ��� ��� ���� ����: ��ٸ��� ��ȣ ��ü�� �� NULL�̰ų� ��ȿ�� ��Ʈvalid bit�� 0�� ���� �ִ���
 *                        �װ͵��� ������ ������ ��ȣ ��ü���� ��ٸ� (��ȿ���� ���� ��ȣ ��ü �����ϰ� ��ٸ��� ���� ����)*/
#define	TASK_WAITOPT__IGNOREINVALID		0x80

/** ��ٸ��� ��� ���� ����: ��ٸ��� ��ȣ ��ü�� ��ηκ��� ��ȣ�� ���� ������ ��ٸ� (��� ��ٸ��� ���� ����) */
#define	TASK_WAITOPT__ALL				0x40

/** �½�ũ �Լ� ������  �� ���� */
typedef void (* taskfunc_ft)(void *);

typedef	struct __task_tip_t {
	unsigned int		type					:  8;
	unsigned int		valid					:  1;
	unsigned int		reserved				:  7;

	unsigned int		reserved2				: 16;
} _task_tip_t;

/** �½�ũ ������ �� ���� */
typedef	_task_tip_t * task_pt;

/**
 * �½�ũ�� �����ϴ� �Լ�
 *
 * @param	task_p		������ �½�ũ�� �ּҸ� ������ �������� �ּ�<br>
 * 						NULL: ������ �½�ũ�� �ּҸ� �������� ����<br>
 * 						<br>
 *
 * @param	func		�½�ũ�� ������ �Լ� ������<br>
 * 						<br>
 *
 * @param	arg			�½�ũ�� ������ �Լ��� ������ �Ű�����<br>
 * 						<br>
 *
 * @param	priority	�켱����<br>
 * 						<br>
 * 						ū ���ڰ� ���� �켱����<br>
 * 						�ְ� �켱������ task_gethighestpriority �Լ��� ���� �ִ� ��<br>
 * 						�߰� �켱������ task_getmiddlepriority �Լ��� �����ִ� ��<br>
 * 						���� �켱������ task_getlowestpriority �Լ��� �����ִ� ��<br>
 * 						<br>
 *
 * @param	stackdepth	���� ����(���� ���� ũ��� stackdepth * INT_SIZE ����Ʈ�� ��)<br>
 * 						0: �⺻  ���� ���� ��(task_getdefaultstackdepth �Լ��� �����ִ� ��)<br>
 * 						1 ~ ���� ���� ���� ��: ���� ���� ���� ��(task_getminstackdepth �Լ��� �����ִ� ��)<br>
 * 						<br>
 *
 * @param	name		�½�ũ �̸�<br>
 * 						NULL: �½�ũ �̸��� �������� ����<br>
 * 						<br>
 *
 * @return	  0: ����<br>
 * 			<br>
 * 			 -1: ����<br>
 * 			 -n: n-1 ��° �Ű������� �߸��Ǿ���<br>
 */
int task_create(task_pt * task_p, taskfunc_ft func, void * arg, int priority, unsigned int stackdepth, const char * name);

/**
 * �½�ũ�� �����ϴ� �Լ� Ȯ����
 *
 * @param	task_p		������ �½�ũ�� �ּҸ� ������ �������� �ּ�<br>
 * 						NULL: ������ �½�ũ�� �ּҸ� �������� ����<br>
 * 						<br>
 *
 * @param	func		�½�ũ�� ������ �Լ� ������<br>
 * 						<br>
 *
 * @param	arg			�½�ũ�� ������ �Լ��� ������ �Ű�����<br>
 * 						<br>
 *
 * @param	priority	�켱����<br>
 * 						<br>
 * 						ū ���ڰ� ���� �켱����<br>
 * 						�ְ� �켱������ task_gethighestpriority �Լ��� ���� �ִ� ��<br>
 * 						�߰� �켱������ task_getmiddlepriority �Լ��� �����ִ� ��<br>
 * 						���� �켱������ task_getlowestpriority �Լ��� �����ִ� ��<br>
 * 						<br>
 *
 * @param	stackdepth	���� ����(���� ���� ũ��� stackdepth * INT_SIZE ����Ʈ�� ��)<br>
 * 						0: �⺻  ���� ���� ��(task_getdefaultstackdepth �Լ��� �����ִ� ��)<br>
 * 						1 ~ ���� ���� ���� ��: ���� ���� ���� ��(task_getminstackdepth �Լ��� �����ִ� ��)<br>
 * 						<br>
 *
 * @param	name		�½�ũ �̸�<br>
 * 						NULL: �½�ũ �̸��� �������� ����<br>
 * 						<br>
 *
 * @param	tag			Ŀ�� ��ü�� �����ϱ� ���� ����ϴ� ����ǥ<br>
 * 						0: 0 ~ UBIK_TAG__USRSTART-1 ������ ������ �ڵ��Ҵ��<br>
 * 						1 ~ UBIK_TAG__USRSTART-1: ����<br>
 * 						<br>
 * 						����ڰ� ���� ������ ��� UBIK_TAG__USRSTART ~ UINT_MAX ������ ���� ����ؾ� ��<br>
 * 						<br>
 *
 * @param	option		���� ���� (TASK_OPT__...)<br>
 * 						0: ��� �͵� �������� ����<br>
 * 						<br>
 *
 * @return	  0: ����<br>
 * 			<br>
 * 			 -1: ����<br>
 * 			 -n: n-1 ��° �Ű������� �߸��Ǿ���<br>
 */
int task_create_ext(task_pt * task_p, taskfunc_ft func, void * arg, int priority, unsigned int stackdepth, const char * name, unsigned int tag, unsigned int option);

/**
 * ��� �½�ũ�� �����ϴ� �Լ�
 *
 * @param	task_p		������ ��� �½�ũ�� �ּҰ� ����� �������� �ּ�<br>
 * 						task_p �Ǵ� *task_p�� NULL: ���� �½�ũ�� ������<br>
 * 						<br>
 * 						���ſ� �����ϸ� *task_p��  NULL�� ��<br>
 * 						<br>
 *
 * @return	  0: ����<br>
 * 			<br>
 * 			 -1: ����<br>
 * 			 -n: n-1 ��° �Ű������� �߸��Ǿ���<br>
 */
int task_delete(task_pt * task_p);

/**
 * ��� �½�ũ�� ����suspend�ϴ� �Լ�
 *
 * @param	task		��� �½�ũ ������<br>
 * 						NULL: ���� �½�ũ<br>
 * 						<br>
 *
 * @return	  0: ����<br>
 * 			<br>
 * 			 -1: ����<br>
 * 			 -n: n-1 ��° �Ű������� �߸��Ǿ���<br>
 */
int task_suspend(task_pt task);

/**
 * ����suspend�� ��� �½�ũ�� �����resume�ϴ� �Լ�
 *
 * @param	task		��� �½�ũ ������<br>
 * 						<br>
 *
 * @return	  0: ����<br>
 * 			<br>
 * 			 -1: ����<br>
 * 			 -n: n-1 ��° �Ű������� �߸��Ǿ���<br>
 */
int task_resume(task_pt task);

/**
 * ���� �½�ũ�� �޸�sleep��Ű�� �Լ�
 *
 * @param	tick		�޸�sleep��ų �ð�(�ý��� ƽtick ��)<br>
 * 						<br>
 *
 * @return	  0: ����<br>
 * 			<br>
 * 			 -1: ����<br>
 */
int task_sleep(unsigned int tick);

/**
 * ���� �½�ũ�� �޸�sleep��Ű�� �Լ� (õ���� ���� ����)
 *
 * @param	timems		�޸�sleep��ų �ð�(õ���� ����)<br>
 * 						<br>
 *
 * @return	  0: ����<br>
 * 			<br>
 * 			 -1: ����<br>
 */
int task_sleepms(unsigned int timems);

/**
 * ��� �½�ũ�� �켱������ �����ϴ� �Լ�
 *
 * @param	task		��� �½�ũ ������<br>
 * 						NULL: ���� �½�ũ<br>
 * 						<br>
 *
 * @param	priority	�켱����<br>
 * 						<br>
 * 						ū ���ڰ� ���� �켱����<br>
 * 						�ְ� �켱������ task_gethighestpriority �Լ��� ���� �ִ� ��<br>
 * 						�߰� �켱������ task_getmiddlepriority �Լ��� �����ִ� ��<br>
 * 						���� �켱������ task_getlowestpriority �Լ��� �����ִ� ��<br>
 * 						<br>
 *
 * @return	  0: ����<br>
 * 			<br>
 * 			 -1: ����<br>
 * 			 -n: n-1 ��° �Ű������� �߸��Ǿ���<br>
 */
int task_setpriority(task_pt task, int priority);

/**
 * ��� �½�ũ�� �켱������ �����ִ� �Լ�
 *
 * ��� �½�ũ�� NULL�̰� Ŀ���� Ȱ��ȭ ���°� �ƴϸ�(ubik_comp_start �Լ��� ȣ���ϱ� ���̸�)
 * �߰� �켱 ����(task_getmiddlepriority �Լ��� ���� �ִ� ��)�� ������<br>
 *
 * @param	task		��� �½�ũ ������<br>
 * 						NULL: ���� �½�ũ<br>
 * 						<br>
 *
 * @return	��� �½�ũ�� �켱����<br>
 * 			<br>
 * 			 -1: ����<br>
 * 			 -n: n-1 ��° �Ű������� �߸��Ǿ���<br>
 */
int task_getpriority(task_pt task);

/**
 * �½�ũ�� ���� �� �ִ� �ְ� �켱������ �����ִ� �Լ�
 *
 * @return	�½�ũ�� ���� �� �ִ� �ְ� �켱����
 */
int task_gethighestpriority(void);

/**
 * �½�ũ�� ���� �� �ִ� ���� �켱������ �����ִ� �Լ�
 *
 * @return	�½�ũ�� ���� �� �ִ� ���� �켱����
 */
int task_getlowestpriority(void);

/**
 * �½�ũ�� ���� �� �ִ� �߰� �켱������ �����ִ� �Լ�
 *
 * @return	�½�ũ�� ���� �� �ִ� �߰� �켱����
 */
int task_getmiddlepriority(void);

/**
 * �½�ũ ��ȯ�� �����ϴ� �Լ�
 */
void task_lock(void);

/**
 * �½�ũ ��ȯ�� �㰡�ϴ� �Լ�
 */
void task_unlock(void);

/**
 * ��� �½�ũ�� ���ÿ� ��ٸ� �� �ִ� ��ȣ ��ü �ִ� ���� �����ϴ� �Լ�
 *
 * @param	task		��� �½�ũ ������<br>
 * 						NULL: ���� �½�ũ<br>
 * 						<br>
 *
 * @param	max			���ÿ� ��ٸ� �� �ִ� ��ȣ ��ü �ִ� �� (TASK_MAXWAITSIGOBJ_MAX ���� �۰ų� ���ƾ� ��)<br>
 * 						<br>
 *
 * @return	  0: ����<br>
 * 			<br>
 * 			 -1: ����<br>
 * 			 -n: n-1 ��° �Ű������� �߸��Ǿ���<br>
 */
int task_setmaxwaitsigobj(task_pt task, int max);

/**
 * ��� �½�ũ�� ���ÿ� ��ٸ� �� �ִ� ��ȣ ��ü �ִ� ���� �����ִ� �Լ�
 *
 * @param	task		��� �½�ũ ������<br>
 * 						NULL: ���� �½�ũ<br>
 * 						<br>
 *
 * @return	���ÿ� ��ٸ� �� �ִ� ��ȣ ��ü �ִ� ��<br>
 * 			<br>
 * 			 -1: ����<br>
 * 			 -n: n-1 ��° �Ű������� �߸��Ǿ���<br>
 */
int task_getmaxwaitsigobj(task_pt task);

/**
 * ��ȣ ��ü ���� ���� ���ÿ� ��ٸ��� �Լ�
 *
 * ���ÿ� ��ٸ� �� �ִ� ��ȣ ��ü �� �⺻���� 1�̸�, task_setmaxwaitsigobj �Լ��� ������ �� ����<br>
 * ���ÿ� ��ٸ��� ��ȣ ��ü ���� ������ �Լ� ������ ũ��Ƽ�� ����(���ͷ�Ʈ ��Ȱ��ȭ ����) ���� �ð��� ������� ������,
 * ������ ���� �ý��ۿ��� ������ �߻��� �� ����
 *
 * @param	sigobj_p	��� ��ü ������ �迭<br>
 * 						<br>
 *
 * @param	sigtype_p	���� ��ȣ�� ������ �迭<br>
 * 						<br>
 *
 * @param	param_p		��ȣ ��ü�� ���� ������ �Ű�����<br>
 * 						NULL: �Ű������� �������� ����<br>
 * 						<br>
 * 						�Ű�����<br>
 * 						��ȣ ��ü�� �޽���ť�� ��� ���� �޽����� ������ ����<br>
 * 						��ȣ ��ü�� ���Ǻ���condition variable�� ��� ���Ǻ���condition variable�� ��ȣ�ϴ� ���ؽ�<br>
 * 						<br>
 *
 * @param	count		�迭 ����<br>
 * 						<br>
 *
 * @param	waitopt		��ٸ��� ��� ���� ���� (TASK_WAITOPT__...)<br>
 * 						0: ��� �͵� �������� ����<br>
 * 						<br>
 * 						��ٸ��� ����� �������� ������ ��ٸ��� ��ȣ ��ü�� �� �ϳ� �̻����κ��� ��ȣ�� ���� ������ ��ٸ�<br>
 * 						<br>
 *
 * @return	  0: ����<br>
 * 			<br>
 * 			 -1: ����<br>
 * 			 -n: n-1 ��° �Ű������� �߸��Ǿ���<br>
 * 			UBIK_ERR__DEADLOCK		: ����� �߻� ���ɼ��� ����<br>
 */
int task_waitforsigobjs(void ** sigobj_p, int * sigtype_p, void ** param_p, int count, unsigned int waitopt);

/**
 * ���� �ð� ���� ��ȣ ��ü ���� ���� ���ÿ� ��ٸ��� �Լ�
 *
 * ���ÿ� ��ٸ� �� �ִ� ��ȣ ��ü �� �⺻���� 1�̸�, task_setmaxwaitsigobj �Լ��� ������ �� ����<br>
 * ���ÿ� ��ٸ��� ��ȣ ��ü ���� ������ �Լ� ������ ũ��Ƽ�� ����(���ͷ�Ʈ ��Ȱ��ȭ ����) ���� �ð��� ������� ������,
 * ������ ���� �ý��ۿ��� ������ �߻��� �� ����
 *
 * @param	sigobj_p	��� ��ü ������ �迭<br>
 * 						<br>
 *
 * @param	sigtype_p	���� ��ȣ�� ������ �迭<br>
 * 						<br>
 *
 * @param	param_p		��ȣ ��ü�� ���� ������ �Ű�����<br>
 * 						NULL: �Ű������� �������� ����<br>
 * 						<br>
 * 						�Ű�����<br>
 * 						��ȣ ��ü�� �޽���ť�� ��� ���� �޽����� ������ ����<br>
 * 						��ȣ ��ü�� ���Ǻ���condition variable�� ��� ���Ǻ���condition variable�� ��ȣ�ϴ� ���ؽ�<br>
 * 						<br>
 *
 * @param	count		�迭 ����<br>
 * 						<br>
 *
 * @param	waitopt		��ٸ��� ��� ���� ���� (TASK_WAITOPT__...)<br>
 * 						0: ��� �͵� �������� ����<br>
 * 						<br>
 * 						��ٸ��� ����� �������� ������ ��ٸ��� ��ȣ ��ü�� �� �ϳ� �̻����κ��� ��ȣ�� ���� ������ ��ٸ�<br>
 * 						<br>
 *
 * @param	tick		���� �ð� (�ý��� ƽtick ��)<br>
 * 						<br>
 *
 * @return	  0: ����<br>
 * 			<br>
 * 			 -1: ����<br>
 * 			 -n: n-1 ��° �Ű������� �߸��Ǿ���<br>
 * 			UBIK_ERR__TIMEOUT		: ���� �ð� �ʰ�<br>
 * 			UBIK_ERR__DEADLOCK		: ����� �߻� ���ɼ��� ����<br>
 */
int task_waitforsigobjs_timed(void ** sigobj_p, int * sigtype_p, void ** param_p, int count, unsigned int waitopt, unsigned int tick);

/**
 * ���� �ð� ���� ��ȣ ��ü ���� ���� ���ÿ� ��ٸ��� �Լ� (õ���� ���� ����)
 *
 * ���ÿ� ��ٸ� �� �ִ� ��ȣ ��ü �� �⺻���� 1�̸�, task_setmaxwaitsigobj �Լ��� ������ �� ����<br>
 * ���ÿ� ��ٸ��� ��ȣ ��ü ���� ������ �Լ� ������ ũ��Ƽ�� ����(���ͷ�Ʈ ��Ȱ��ȭ ����) ���� �ð��� ������� ������,
 * ������ ���� �ý��ۿ��� ������ �߻��� �� ����
 *
 * @param	sigobj_p	��� ��ü ������ �迭<br>
 * 						<br>
 *
 * @param	sigtype_p	���� ��ȣ�� ������ �迭<br>
 * 						<br>
 *
 * @param	param_p		��ȣ ��ü�� ���� ������ �Ű�����<br>
 * 						NULL: �Ű������� �������� ����<br>
 * 						<br>
 * 						�Ű�����<br>
 * 						��ȣ ��ü�� �޽���ť�� ��� ���� �޽����� ������ ����<br>
 * 						��ȣ ��ü�� ���Ǻ���condition variable�� ��� ���Ǻ���condition variable�� ��ȣ�ϴ� ���ؽ�<br>
 * 						<br>
 *
 * @param	count		�迭 ����<br>
 * 						<br>
 *
 * @param	waitopt		��ٸ��� ��� ���� ���� (TASK_WAITOPT__...)<br>
 * 						0: ��� �͵� �������� ����<br>
 * 						<br>
 * 						��ٸ��� ����� �������� ������ ��ٸ��� ��ȣ ��ü�� �� �ϳ� �̻����κ��� ��ȣ�� ���� ������ ��ٸ�<br>
 * 						<br>
 *
 * @param	timems		���� �ð� (õ���� ����)<br>
 * 						<br>
 *
 * @return	  0: ����<br>
 * 			<br>
 * 			 -1: ����<br>
 * 			 -n: n-1 ��° �Ű������� �߸��Ǿ���<br>
 * 			UBIK_ERR__TIMEOUT		: ���� �ð� �ʰ�<br>
 * 			UBIK_ERR__DEADLOCK		: ����� �߻� ���ɼ��� ����<br>
 */
int task_waitforsigobjs_timedms(void ** sigobj_p, int * sigtype_p, void ** param_p, int count, unsigned int waitopt, unsigned int timems);

/**
 * ��� �½�ũ���� ����Ǳ⸦ ��ٸ��� �Լ�
 *
 * @param	task_p		��� �½�ũ ������ �迭<br>
 * 						<br>
 *
 * @param	result_p	������� ������  �迭<br>
 * 						NULL: ������� �������� ����<br>
 * 						<br>
 * 						�����<br>
 * 						0: ���� ����<br>
 * 						UBIK_ERR__DEADLOCK		: ����� �߻� ���ɼ��� ����<br>
 * 						<br>
 *
 * @param	count		�迭 ����  (TASK_MAXWAITSIGOBJ_MAX ���� �۰ų� ���ƾ� ��)<br>
 * 						<br>
 *
 * @return	  0: ����<br>
 * 			<br>
 * 			 -1: ����<br>
 * 			 -n: n-1 ��° �Ű������� �߸��Ǿ���<br>
 * 			UBIK_ERR__DEADLOCK		: ����� �߻� ���ɼ��� ����<br>
 */
int task_join(task_pt * task_p, int * result_p, int count);

/**
 * ���� �ð� ���� ��� �½�ũ���� ����Ǳ⸦ ��ٸ��� �Լ�
 *
 * @param	task_p		��� �½�ũ ������ �迭<br>
 * 						<br>
 *
 * @param	result_p	������� ������  �迭<br>
 * 						NULL: ������� �������� ����<br>
 * 						<br>
 * 						�����<br>
 * 						0: ���� ����<br>
 *			 			UBIK_ERR__TIMEOUT		: ���� �ð� �ʰ�<br>
 * 						UBIK_ERR__DEADLOCK		: ����� �߻� ���ɼ��� ����<br>
 * 						<br>
 *
 * @param	count		�迭 ����  (TASK_MAXWAITSIGOBJ_MAX ���� �۰ų� ���ƾ� ��)<br>
 * 						<br>
 *
 * @param	tick		���� �ð� (�ý��� ƽtick ��)<br>
 * 						<br>
 *
 * @return	  0: ����<br>
 * 			<br>
 * 			 -1: ����<br>
 * 			 -n: n-1 ��° �Ű������� �߸��Ǿ���<br>
 * 			UBIK_ERR__TIMEOUT		: ���� �ð� �ʰ�<br>
 * 			UBIK_ERR__DEADLOCK		: ����� �߻� ���ɼ��� ����<br>
 */
int task_join_timed(task_pt * task_p, int * result_p, int count, unsigned int tick);

/**
 * ���� �ð� ���� ��� �½�ũ���� ����Ǳ⸦ ��ٸ��� �Լ� (õ���� ���� ����)
 *
 * @param	task_p		��� �½�ũ ������ �迭<br>
 * 						<br>
 *
 * @param	result_p	������� ������  �迭<br>
 * 						NULL: ������� �������� ����<br>
 * 						<br>
 * 						�����<br>
 * 						0: ���� ����<br>
 *			 			UBIK_ERR__TIMEOUT		: ���� �ð� �ʰ�<br>
 * 						UBIK_ERR__DEADLOCK		: ����� �߻� ���ɼ��� ����<br>
 * 						<br>
 *
 * @param	count		�迭 ����  (TASK_MAXWAITSIGOBJ_MAX ���� �۰ų� ���ƾ� ��)<br>
 * 						<br>
 *
 * @param	timems		���� �ð� (õ���� ����)<br>
 * 						<br>
 *
 * @return	  0: ����<br>
 * 			<br>
 * 			 -1: ����<br>
 * 			 -n: n-1 ��° �Ű������� �߸��Ǿ���<br>
 * 			UBIK_ERR__TIMEOUT		: ���� �ð� �ʰ�<br>
 * 			UBIK_ERR__DEADLOCK		: ����� �߻� ���ɼ��� ����<br>
 */
int task_join_timedms(task_pt * task_p, int * result_p, int count, unsigned int timems);

/**
 * ���� �½�ũ �����͸� �����ִ� �Լ�
 *
 * @return	���� �½�ũ ������<br>
 * 			<br>
 * 			NULL: ubik�� Ȱ��ȭ���� �ʾ���
 */
task_pt task_getcur(void);

/**
 * ���� �½�ũ�� ���� ���� �ð��� �����ִ� �Լ�
 *
 * ���� �ð��� �ΰ� ��ȣ ��ü�� ��ٸ��� �½�ũ�� ���� �ð��� ���ϱ� ���� ��ȣ�� �ް� ����� ���,
 * �� �Լ��� ����� ���� ���� �ð��� �˾Ƴ� �� ����
 *
 * @return	���� ���� �ð� (�ý��� ƽtick ��)
 */
unsigned int task_getremainingtimeout(void);

/**
 * ���� �½�ũ�� ���� ���� �ð��� �����ִ� �Լ�
 *
 * ���� �ð��� �ΰ� ��ȣ ��ü�� ��ٸ��� �½�ũ�� ���� �ð��� ���ϱ� ���� ��ȣ�� �ް� ����� ���,
 * �� �Լ��� ����� ���� ���� �ð��� �˾Ƴ� �� ����
 *
 * @return	���� ���� �ð� (õ���� ���� ����)
 */
unsigned int task_getremainingtimeoutms(void);

/**
 * ���� ���� �������� �����ִ� �Լ�
 *
 * @return	���� ���� ������
 */
unsigned int task_getminstackdepth(void);

/**
 * ���� ���� �⺻���� �����ִ� �Լ�
 *
 * @return	���� ���� �⺻��
 */
unsigned int task_getdefaultstackdepth(void);

/**
 * �ش� �½�ũ�� ���� ���� ũ�⸦ �����ִ� �Լ�
 *
 * @param	task			��� �½�ũ ������<br>
 * 							NULL: ���� �½�ũ<br>
 * 							<br>
 *
 * @param	stacksize_p		���� ���� ũ�⸦ ������ ������ �ּ�<br>
 *							<br>
 *
 * @return	  0: ����<br>
 * 			<br>
 * 			 -1: ����<br>
 * 			 -n: n-1 ��° �Ű������� �߸��Ǿ���<br>
 */
int task_getstacksize(task_pt task, unsigned int * stacksize_p);

/**
 * �ش� �½�ũ�� ���� ���� ũ�⸦ �����ִ� �Լ� (���� �����÷ο� �˻� ����)
 *
 * @param	task			��� �½�ũ ������<br>
 * 							NULL: ���� �½�ũ<br>
 * 							<br>
 *
 * @param	stacksize_p		���� ���� ũ�⸦ ������ ������ �ּ�<br>
 *							<br>
 *
 * @return	  0: ����<br>
 * 			<br>
 * 			 -1: ����<br>
 * 			 -n: n-1 ��° �Ű������� �߸��Ǿ���<br>
 */
int task_getstacksize_nosocheck(task_pt task, unsigned int * stacksize_p);

/**
 * �ش� �½�ũ�� �ִ� ���� ��뷮�� �����ִ� �Լ�
 *
 * @param	task			��� �½�ũ ������<br>
 * 							NULL: ���� �½�ũ<br>
 * 							<br>
 *
 * @param	maxstackusage_p	 �ִ� ���� ��뷮�� ������ ������ �ּ�<br>
 *							<br>
 *
 * @return	  0: ����<br>
 * 			<br>
 * 			 -1: ����<br>
 * 			 -n: n-1 ��° �Ű������� �߸��Ǿ���<br>
 */
int task_getmaxstackusage(task_pt task, unsigned int * maxstackusage_p);

/**
 * �ش� �½�ũ�� �ִ� ���� ��뷮�� �����ִ� �Լ� (���� �����÷ο� �˻� ����)
 *
 * @param	task			��� �½�ũ ������<br>
 * 							NULL: ���� �½�ũ<br>
 * 							<br>
 *
 * @param	maxstackusage_p	 �ִ� ���� ��뷮�� ������ ������ �ּ�<br>
 *							<br>
 *
 * @return	  0: ����<br>
 * 			<br>
 * 			 -1: ����<br>
 * 			 -n: n-1 ��° �Ű������� �߸��Ǿ���<br>
 */
int task_getmaxstackusage_nosocheck(task_pt task, unsigned int * maxstackusage_p);

#ifdef	__cplusplus
}
#endif

#endif /* ITF_UBIK_TASK_H_ */

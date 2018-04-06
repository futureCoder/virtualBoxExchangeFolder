# coding: utf-8
import stations
"""命令行火车票查看器

Usage:
    tickets [-gdtkz] <from> <to> <date>

Options:
    -h,--help   显示帮助菜单
    -g          高铁
    -d          动车
    -t          特快
    -k          快速
    -z          直达

Example:
    tickets 北京 上海 2018-3-10
    tickets -dg 成都 南京 2018-3-10
"""
from docopt import docopt
import requests


def cli():
    """command-line interface"""
    arguments = docopt(__doc__)
    #print(arguments)
    from_station = stations.get(arguments['<from>'])
    to_station = stations.get(arguments['<to>'])
    date = arguments['date']
    url = ('https://kyfw.12306.cn/otn/leftTicket/queryO'
            'leftTicketDTO.train_date={}&'
           'leftTicketDTO.from_station={}&leftTicketDTO.to_station={}&purpose_codes=ADULT').format(
                date, from_station, to_station
    )
    r = requests.get(url, verify=False)
    print(r.json())


if __name__ == '__main__':
    cli()



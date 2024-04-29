# A Flask Application

import sys
import flask


def MakeResponseId(args):
    def ResponseIdInternal(id):
        return f'<p>Hello {id}, {args}</p>'
    return ResponseIdInternal


if __name__ == '__main__':
    if len(sys.argv) != 2:
        print(f'Usage: python {sys.argv[0]} port')
    else:
        app = flask.Flask(__name__)

        @app.route('/')
        def index():
            return '<p>Hello world</p>'

        @app.route('/user/<name>', methods=['GET', 'POST'])
        def reponse_user(name):
            return f'<p>Hello {name}</p>'

        fn = MakeResponseId('PPPPPPPPPPP')
        app.add_url_rule('/uid/<int:id>', view_func=fn, provide_automatic_options=True, methods=['GET', 'POST'])
        app.run(debug=True, port=int(sys.argv[1]))
